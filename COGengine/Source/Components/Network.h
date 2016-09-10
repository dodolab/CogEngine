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
		}

		void SendUDPMessage(unsigned int param1, unsigned int param2, unsigned char* data, unsigned int size) {
			NetWriter* writer = new NetWriter(size + 4 + 4 + 4);
			writer->WriteDWord(param1);
			writer->WriteDWord(param2);
			writer->WriteDWord(size);
			writer->WriteData(data, size);
			auto buffer = writer->GetBuffer();
			udpSender.Send((char*)buffer, writer->GetBufferBites()/8);
			delete writer;
		}

		NetReader* ReceiveUDPMessage(unsigned int param1, unsigned int param2, int timeoutSec) {
			auto time = ofGetElapsedTimeMillis();
			int timeOutMillis = timeoutSec * 1000;

			NetReader* bufferStream = new NetReader(100000);
			
			while (true) {
				if ((ofGetElapsedTimeMillis() - time) > timeOutMillis) return nullptr;

				bufferStream->Reset();
				auto mojo = bufferStream->GetBufferBites() / 8;
				udpReceiver.Receive((char*)bufferStream->GetBuffer(), bufferStream->GetBufferBites()/8);

				if (bufferStream->ReadDWord() == param1 && bufferStream->ReadDWord() == param2) {
					// get the right message
					unsigned int size = bufferStream->ReadDWord();
					unsigned char* data = bufferStream->ReadData(size);
					return new NetReader(data, size);
				}
			}

		}
	};

}// namespace