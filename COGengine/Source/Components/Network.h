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

		void SetupTCPClient(string ip, int port, bool verbose) {
			tcpClient.setup(ip.c_str(), port);
			tcpClient.setVerbose(verbose);
		}

		void SetupTCPServer(int port) {
			tcpServer.setup(port);
		}

		void SetupUDPSender(string ip, int port, bool nonBlocking) {
			udpSender.Create();
			udpSender.Connect(ip.c_str(), port);
			udpSender.SetNonBlocking(nonBlocking);
		}

		void SetupUDPReceiver(int port, bool nonBlocking) {
			udpReceiver.Create();
			udpReceiver.Bind(port);
			udpReceiver.SetNonBlocking(nonBlocking);
		}

	};

}// namespace