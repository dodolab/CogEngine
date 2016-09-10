#pragma once

#include "ofxCogCommon.h"
#include "Component.h"
#include "ofxNetwork.h"
#include "NetReader.h"
#include "NetWriter.h"

namespace Cog {

	class Network : public  Component {
		OBJECT(Network)

	private:
		ofxTCPClient tcpClient;
		ofxTCPServer tcpServer;
		ofxUDPManager udpSender;
		ofxUDPManager udpReceiver;

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
			udpSender.Create();
			udpSender.Connect(ip.c_str(), port); 
			udpSender.SetNonBlocking(nonBlocking);
		}

		ofxUDPManager& GetUDPSender() {
			return udpSender;
		}

		ofxUDPManager& GetUDPReceiver() {
			return udpReceiver;
		}

		void SetupUDPReceiver(int port, bool nonBlocking) {
			udpReceiver.Create();
			udpReceiver.Bind(port);
			udpReceiver.SetNonBlocking(nonBlocking); 
			udpReceiver.SetReceiveBufferSize(10000);
		}

		void SendUDPMessage(unsigned int param1, unsigned int param2, unsigned char* data, unsigned int size) {
			NetWriter* writer = new NetWriter(size + 4 + 4 + 4);
			// write two parameters and content length
			writer->WriteDWord(param1);
			writer->WriteDWord(param2);
			writer->WriteDWord(size);
			writer->WriteBytes(data, size);
			auto buffer = writer->GetBuffer();
			udpSender.Send((char*)buffer, writer->GetBufferBites()/8);
			delete writer;
		}

		NetReader* ReceiveUDPMessage(unsigned int param1, unsigned int param2, int timeoutSec) {
			auto time = ofGetElapsedTimeMillis();
			int timeOutMillis = timeoutSec * 1000;

			NetReader* bufferStream1 = new NetReader(100000);
			NetReader* bufferStream2 = new NetReader(100000);
			
			while (true) {
				
				int bytesBuff1 = 0;
				int bytesBuff2 = 0;

				do {
					bufferStream1->Reset();
					bufferStream2->Reset();
					bytesBuff1 = udpReceiver.Receive((char*)bufferStream1->GetBuffer(), bufferStream1->GetBufferBites() / 8);
					bytesBuff2 = udpReceiver.Receive((char*)bufferStream2->GetBuffer(), bufferStream2->GetBufferBites() / 8);
				} while (bytesBuff1 > 0 && bytesBuff2 > 0);

				if (bytesBuff2 > 0) {
					delete bufferStream1;
					bufferStream1 = bufferStream2;
					bufferStream2 = nullptr;
				}
				if (bufferStream1->ReadDWord() == param1 && bufferStream1->ReadDWord() == param2) {
					// get the right message
					unsigned int size = bufferStream1->ReadDWord();
					unsigned char* data = bufferStream1->ReadBytes(size);
					delete bufferStream1;
					delete bufferStream2;
					return new NetReader(data,size);
				}

			
				if ((ofGetElapsedTimeMillis() - time) > timeOutMillis) {
					delete bufferStream1;
					delete bufferStream2;
					return nullptr;
				}
			}

		}

		virtual void Update(const uint64 delta, const uint64 absolute) {
		}
	};

}// namespace