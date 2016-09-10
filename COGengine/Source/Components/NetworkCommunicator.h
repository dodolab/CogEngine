#pragma once

#include "ofxSmartPointer.h"
#include "Component.h"


namespace Cog {

	class NetOutputMessage;
	class Network;

	enum class ClientComState {
		DISCOVERING,
		CONNECTING,
		COMMUNICATING
	};

	enum class ServerComState {
		LISTENING,
		COMMUNICATING
	};

	// forward decl
	class NetworkCommunicator;

	class ClientCommunicator {
	private:
		int applicationId;
		int clientPort;
		int serverPort;
		string serverIp = "";
		Network* network;
		NetworkCommunicator* netcom;

		tBYTE lastReceivedMsgId = -1;
		tBYTE lastSentMsgId = 0;

		ClientComState networkState = ClientComState::DISCOVERING;
		spt<NetOutputMessage> msgToSent = spt<NetOutputMessage>();
		uint64 lastReceivedMsgTime = 0;
		map<string, uint64> discoveredServers;
	public:

		ClientCommunicator(int applicationId, int clientPort, int serverPort, Network* network, NetworkCommunicator* netcom)
			: applicationId(applicationId), clientPort(clientPort), serverPort(serverPort), network(network), netcom(netcom) {

		}

		int GetApplicationId() {
			return applicationId;
		}

		int GetClientPort() {
			return clientPort;
		}

		int GetServerPort() {
			return serverPort;
		}

		string GetServerIp() {
			return serverIp;
		}

		int GetLastReceivedMsgId() {
			return lastReceivedMsgId;
		}

		uint64 GetLastMessageTime() {
			return lastReceivedMsgTime;
		}

		map<string, uint64>& GetDiscoveredServers() {
			return discoveredServers;
		}

		void SetMessageForSending(spt<NetOutputMessage> msg);

		void ConnectToServer();

		void ConnectToServer(string ip);

		virtual void Update(const uint64 delta, const uint64 absolute);

	protected:
		void Broadcast();

		void SendCallbackMessage(uint64 time);
	};

	class ServerCommunicator {
	private:
		int applicationId;
		int serverPort;

		int clientPort;
		string clientIp = "";
		Network* network;
		NetworkCommunicator* netcom;

		tBYTE lastReceivedMsgId = -1;
		tBYTE lastSentMsgId = 0;

		ServerComState networkState = ServerComState::LISTENING;
		spt<NetOutputMessage> msgToSent = spt<NetOutputMessage>();
		uint64 lastReceivedMsgTime = 0;
		map<tBYTE, spt<NetOutputMessage>> confirmations;

	public:

		ServerCommunicator(int applicationId, int serverPort, Network* network, NetworkCommunicator* netcom)
			: applicationId(applicationId), serverPort(serverPort), network(network), netcom(netcom) {
			CogLogInfo("NETWORK", "Listening on port %d", serverPort);
		}

		int GetApplicationId() {
			return applicationId;
		}

		int GetPort() {
			return serverPort;
		}

		string GetClientIp() {
			return clientIp;
		}

		uint64 GetLastMessageTime() {
			return lastReceivedMsgTime;
		}

		void SetMessageForSending(spt<NetOutputMessage> msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

	protected:
		void SendUpdateMessage(uint64 time);
	};

	class NetworkCommunicator : public Component {
		OBJECT(NetworkCommunicator)

	private:
		ClientCommunicator* client = nullptr;
		ServerCommunicator* server = nullptr;
		Network* network = nullptr;

	public:

		void InitClient(int applicationId, int clientPort, int serverPort);

		void InitServer(int applicationId, int port);

		ClientCommunicator* GetClient() {
			return client;
		}

		ServerCommunicator* GetServer() {
			return server;
		}

		void CloseClient();

		void CloseServer();

		void OnMessage(Msg& msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

		friend class ClientCommunicator;
		friend class ServerCommunicator;
	};

}// namespace