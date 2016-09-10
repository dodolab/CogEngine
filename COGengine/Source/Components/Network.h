#pragma once

#include "ofxCogCommon.h"
#include "Component.h"
#include "ofxNetwork.h"
#include "NetReader.h"
#include "NetWriter.h"
#include "NetMessage.h"

namespace Cog {

	class Network : public  Component {
		OBJECT(Network)

	private:
		ofxTCPClient tcpClient;
		ofxTCPServer tcpServer;
		ofxUDPManager udpManager;

		// udp receiver buffers
		NetReader* bufferStream;
	public:


		/**
		* Initializes controller
		*/
		void Init() {

		}

		/**
		* Initializes controller, using xml
		*/
		void Init(spt<ofxXml> xml) {

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
			udpManager.Create();
			udpManager.Connect(ip.c_str(), port);
			udpManager.SetNonBlocking(nonBlocking);
		}

		ofxUDPManager& GetUDPManager() {
			return udpManager;
		}

		void SetupUDPReceiver(int port, int bufferSize, bool nonBlocking) {
			udpManager.Create();
			udpManager.Bind(port);
			udpManager.SetNonBlocking(nonBlocking);
			udpManager.SetReceiveBufferSize(bufferSize);

			bufferStream = new NetReader(bufferSize);
		}

		void SendUDPMessage(unsigned int applicationId, unsigned int communicationId, unsigned char* data, unsigned int size) {
			NetWriter* writer = new NetWriter(size + 4 + 4 + 4);
			// write two parameters and content length
			writer->WriteDWord(applicationId);
			writer->WriteDWord(communicationId);
			writer->WriteDWord(size);
			writer->WriteBytes(data, size);
			auto buffer = writer->GetBuffer();
			udpManager.Send((char*)buffer, writer->GetBufferBites() / 8);
			delete writer;
		}

		spt<NetMessage> ReceiveUDPMessage(unsigned int applicationId, unsigned int communicationId, int timeoutSec) {

			auto time = ofGetElapsedTimeMillis();
			int timeOutMillis = timeoutSec * 1000;

			NetReader* filledBuffer = nullptr;

			while (true) {

				bufferStream->Reset();
				int bytesBuff = udpManager.Receive((char*)bufferStream->GetBuffer(), bufferStream->GetBufferBites() / 8);

				if (bytesBuff > 0 && bufferStream->ReadDWord() == applicationId && bufferStream->ReadDWord() == communicationId) {

					unsigned int size = bufferStream->ReadDWord();
					// from now, bufferStream1 contains the proper content
					spt<NetMessage> msg = spt<NetMessage>(new NetMessage());
					msg->LoadFromStream(bufferStream);

					string ipAddress = "";
					int port = 0;
					udpManager.GetRemoteAddr(ipAddress, port);
					msg->SetIpAddress(ipAddress);
					msg->SetPort(port);
					cout << "receiver from " << ipAddress << " und port " << port << endl;

					return msg;
				}

				if ((ofGetElapsedTimeMillis() - time) > timeOutMillis) {
					return spt<NetMessage>();
				}
			}
		}

		virtual void Update(const uint64 delta, const uint64 absolute) {
		}
	};

}// namespace