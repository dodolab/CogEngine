#pragma once

#include "ofxCogCommon.h"
#include "Component.h"
#include "ofxNetwork.h"

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

	};

}// namespace