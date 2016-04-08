#pragma once

#include "Component.h"
#include "ofxNetwork.h"
#include "NetReader.h"
#include "NetWriter.h"
#include "NetMessage.h"

namespace Cog {

	class NetReader;

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

		void SetupTCPClient(string ip, int port, string msgDelimiter);

		void SetupTCPServer(int port, string msgDelimiter);

		ofxTCPClient& GetTCPClient() {
			return tcpClient;
		}

		ofxTCPServer& GetTCPServer() {
			return tcpServer;
		}

		void SetupUDPSender(string ip, int port, bool nonBlocking);

		ofxUDPManager& GetUDPSender() {
			return udpManager;
		}

		ofxUDPManager& GetUDPReceiver() {
			return udpManager;
		}

		void SetupUDPReceiver(int port, int bufferSize, bool nonBlocking);

		void SendUDPMessage(unsigned int applicationId, spt<NetOutputMessage> msg);

		void SendUDPMessage(unsigned int applicationId, NetWriter* writer);

		NetReader* ReceiveUDPMessage(unsigned int applicationId, int timeoutSec);

		spt<NetInputMessage> ReceiveUDPMessage(unsigned int applicationId, int timeoutSec, bool emptyBuffer);
	};

}// namespace