#pragma once

#include "ofxNetwork.h"
#include "ofURLFileLoader.h"
#include "Component.h"
#include "NetMessage.h"

namespace Cog {

	class NetReader;
	class NetWriter;

	/**
	* Container for network protocols such as TCP, UDP and HTTP
	*/
	class Network {

	private:
		ofxTCPManager tcpManager;
		ofxUDPManager udpManager;

		int tcpListenPort = 0;
		int udpListenPort = 0;

		// receive buffers
		NetReader* tcpBufferStream;
		NetReader* udpBufferStream;
	public:

		Network() {
			udpManager.Create();
			tcpManager.Create();
		}

		// ======================================= TCP ========================================

		/**
		* Configures TCP sender
		* @param ip ip address of the destination point
		* @param port port of the destination service
		* @param nonBlocking if true, thread won'b be blocked while waiting for messages
		*/
		void SetupTCPSender(string ip, int port, bool nonBlocking);

		/**
		* Configures TCP receiver
		* @param port port of the destination service
		* @param bufferSize size of buffer for incoming messages
		* @param nonBlocking if true, thread won't be blocked while waiting for messages
		*/
		void SetupTCPReceiver(int port, int bufferSize, bool nonBlocking);

		/**
		* Gets TCP sender
		*/
		ofxTCPManager& GetTCPSender() {
			return tcpManager;
		}

		/**
		* Gets TCP receiver
		*/
		ofxTCPManager& GetTCPReceiver() {
			return tcpManager;
		}

		/**
		* Sends TCP message
		* @param applicationId application identifier
		* @param msg msg to send
		*/
		void SendTCPMessage(unsigned int applicationId, spt<NetOutputMessage> msg);

		/**
		* Sends TCP message
		* @param applicationId application identifier
		* @param writer data stream containing bytes of the message
		*/
		void SendTCPMessage(unsigned int applicationId, NetWriter* writer);

		/**
		* Receives TCP message
		* @param applicationId application identifier
		* @param timeoutSec number of seconds the receiver should wait for a new message (set 0 for simple check)
		*/
		NetReader* ReceiveTCPMessage(unsigned int applicationId, int timeoutSec);

		/**
		* Receives TCP message
		* @param applicationId application identifier
		* @param timeoutSec number of seconds the receiver should wait for a new message (set 0 for simple check)
		* @param emptyBuffer if true, all messages already waiting in the TCP buffer to be withdrawn will be discarded
		* and only the last message will be returned; this is appropriate for the case where this method is called only
		* several times per second, so it is necessary to keep all buffers empty, otherwise they begin to fulfill the buffer
		*/
		spt<NetInputMessage> ReceiveTCPMessage(unsigned int applicationId, int timeoutSec, bool emptyBuffer);

		/**
		* Closes TCP
		*/
		void CloseTCP() {
			tcpManager.Close();
		}

		// ======================================= UDP ========================================

		/**
		* Configures UDP sender
		* @param ip ip address of the destination point
		* @param port port of the destination service
		* @param nonBlocking if true, thread won'b be blocked while waiting for messages
		*/
		void SetupUDPSender(string ip, int port, bool nonBlocking);

		/**
		* Configures UDP receiver
		* @param port port of the destination service
		* @param bufferSize size of buffer for incoming messages
		* @param nonBlocking if true, thread won't be blocked while waiting for messages
		*/
		void SetupUDPReceiver(int port, int bufferSize, bool nonBlocking);

		/**
		* Gets UDP sender
		*/
		ofxUDPManager& GetUDPSender() {
			return udpManager;
		}

		/**
		* Gets UDP receiver
		*/
		ofxUDPManager& GetUDPReceiver() {
			return udpManager;
		}

		/**
		* Sends UDP message
		* @param applicationId application identifier
		* @param msg msg to send
		*/
		void SendUDPMessage(unsigned int applicationId, spt<NetOutputMessage> msg);

		/**
		* Sends UDP message
		* @param applicationId application identifier
		* @param writer data stream containing bytes of the message
		*/
		void SendUDPMessage(unsigned int applicationId, NetWriter* writer);

		/**
		* Receives UDP message
		* @param applicationId application identifier
		* @param timeoutSec number of seconds the receiver should wait for a new message (set 0 for simple check)
		*/
		NetReader* ReceiveUDPMessage(unsigned int applicationId, int timeoutSec);

		/**
		* Receives UDP message
		* @param applicationId application identifier
		* @param timeoutSec number of seconds the receiver should wait for a new message (set 0 for simple check)
		* @param emptyBuffer if true, all messages already waiting in the UDP buffer to be withdrawn will be discarded
		* and only the last message will be returned; this is appropriate for the case where this method is called only
		* several times per second, so it is necessary to keep all buffers empty, otherwise they begin to fulfill the buffer
		*/
		spt<NetInputMessage> ReceiveUDPMessage(unsigned int applicationId, int timeoutSec, bool emptyBuffer);

		/**
		* Closes UDP
		*/
		void CloseUDP() {
			udpManager.Close();
		}

		// ======================================= HTTP ========================================
		ofHttpResponse HttpGet(string url);

		ofHttpResponse HttpGet(ofHttpRequest& request);

	protected:

		/**
		* Prepares message for sending
		*/
		NetWriter* PrepareMessage(unsigned int applicationId, spt<NetOutputMessage> msg);

		/**
		* Prepares message for sending by copying the writer and storing application id
		*/
		NetWriter* PrepareMessage(unsigned int applicationId, NetWriter* writer);

#define MANAGER_UDP 0
#define MANAGER_TCP 1

		/**
		* Tries to receive message as a NetReader
		*/
		NetReader* ReceiveMessage(unsigned int applicationId, int timeoutSec, int managerType);

		/**
		* Tries to receive message
		*/
		spt<NetInputMessage> ReceiveMessage(unsigned int applicationId, int timeoutSec, bool emptyBuffer, int managerType);

	};

}// namespace