#pragma once

#include "ofxCogCommon.h"
#include "Component.h"
#include "ofxNetwork.h"
#include "NetReader.h"
#include "NetWriter.h"
#include "NetMessage.h"

namespace Cog {

	class Network  {
		
	private:
		ofxTCPClient tcpClient;
		ofxTCPServer tcpServer;
		ofxUDPManager udpManager;

		int udpListenPort = 0;

		// udp receiver buffers
		NetReader* bufferStream;
	public:

		Network() {
			udpManager.Create();
		}

		void CloseUDP() {
			udpManager.Close();
		}

		void SetupTCPClient(string ip, int port, string msgDelimiter) {
			tcpClient.setup(ip.c_str(), port);
			tcpClient.setMessageDelimiter(msgDelimiter);
		}

		void SetupTCPServer(int port, string msgDelimiter) {
			tcpServer.setup(port);
			// delimiter is set as soon as a new client is connected
			tcpServer.setMessageDelimiter(msgDelimiter);
		}

		ofxTCPClient& GetTCPClient() {
			return tcpClient;
		}

		ofxTCPServer& GetTCPServer() {
			return tcpServer;
		}

		void SetupUDPSender(string ip, int port, bool nonBlocking) {
			udpManager.Connect(ip.c_str(), port);
			udpManager.SetNonBlocking(nonBlocking);
		}


		ofxUDPManager& GetUDPSender() {
			return udpManager;
		}

		ofxUDPManager& GetUDPReceiver() {
			return udpManager;
		}

		void SetupUDPReceiver(int port, int bufferSize, bool nonBlocking) {
			udpManager.Bind(port);
			udpManager.SetNonBlocking(nonBlocking);
			udpManager.SetReceiveBufferSize(bufferSize);
			this->udpListenPort = port;
			bufferStream = new NetReader(bufferSize);
		}

		void SendUDPMessage(unsigned int applicationId, spt<NetOutputMessage> msg) {
			NetWriter* writer = new NetWriter(msg->GetMessageLength() + 2);
			// write application id and the content
			writer->WriteWord(applicationId);
			msg->SaveToStream(writer);

			auto buffer = writer->GetBuffer();
			udpManager.Send((char*)buffer, writer->GetUsedBites() / 8);
			delete writer;
		}

		void SendUDPMessage(unsigned int applicationId, NetWriter* writer) {
			NetWriter* writer2 = new NetWriter(writer->GetUsedBites()*8+2);
			writer2->WriteWord(applicationId);
			writer2->WriteBytes(writer->GetBuffer(), writer->GetUsedBites() * 8);
			auto buffer = writer2->GetBuffer();
			udpManager.Send((char*)buffer, writer2->GetUsedBites() / 8);
			delete writer2;
		}

		NetReader* ReceiveUDPMessage(unsigned int applicationId, int timeoutSec) {
			auto time = ofGetElapsedTimeMillis();
			int timeOutMillis = timeoutSec * 1000;

			NetReader* filledBuffer = nullptr;

			while (true) {

				bufferStream->Reset();
				int bytesBuff = udpManager.Receive((char*)bufferStream->GetBuffer(), bufferStream->GetBufferBites() / 8);

				if (bytesBuff > 0 && bufferStream->ReadWord() == applicationId) {

					unsigned int size = bytesBuff - 2;
					NetWriter* writer = new NetWriter(size);
					writer->WriteBytes(bufferStream->GetActualPointer(), size);
					filledBuffer = new NetReader(writer->GetBuffer(),size);
					return filledBuffer;
				}
				if ((ofGetElapsedTimeMillis() - time) > timeOutMillis) {
					return nullptr;
				}
			}
		}

		spt<NetInputMessage> ReceiveUDPMessage(unsigned int applicationId, int timeoutSec, bool emptyBuffer) {

			auto time = ofGetElapsedTimeMillis();
			int timeOutMillis = timeoutSec * 1000;

			spt<NetInputMessage> receivedMsg = spt<NetInputMessage>();

			while (true) {

				bufferStream->Reset();
				int bytesBuff = udpManager.Receive((char*)bufferStream->GetBuffer(), bufferStream->GetBufferBites() / 8);

				if (bytesBuff > 0 && bufferStream->ReadWord() == applicationId) {
					
					// size of content
					unsigned int size = bytesBuff - 2;

					// from now, bufferStream1 contains the proper content
					receivedMsg = spt<NetInputMessage>(new NetInputMessage(size));
					receivedMsg->LoadFromStream(bufferStream);
					string ipAddress = "";
					int port = 0;
					udpManager.GetRemoteAddr(ipAddress, port);
					receivedMsg->SetSourceIp(ipAddress);
					receivedMsg->SetSourcePort(port);

					// if emptyBuffer is set to true, the whole UDP buffer will be read and the last message will be returned
					if(!emptyBuffer) return receivedMsg;
					else continue;
				}
				else {
					if (receivedMsg) return receivedMsg;
				}

				if ((ofGetElapsedTimeMillis() - time) > timeOutMillis) {
					return spt<NetInputMessage>();
				}
			}
		}
	};

}// namespace