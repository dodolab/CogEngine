#pragma once

#include "ofxSmartPointer.h"
#include "Component.h"


namespace Cog {

	typedef unsigned char BYTE;
	typedef unsigned int DWORD;
	typedef unsigned short WORD;

	class NetOutputMessage;
	class Network;

	enum class NetworkComState {
		BROADCASTING,	// for client
		LISTENING,		// for server
		UPDATING_SERVER,
		UPDATING_CLIENT
	};

	enum class NetworkComMode {
		CHECKING,		// accept messages but don't answer
		ESTABLISHING	// accept messages and answer
	};

	class NetworkCommunicator : public Component {
		OBJECT(NetworkCommunicator)

	private:
		int applicationId;
		int port;
		string clientIp;
		Network* network;
		BYTE lastReceivedMsgId = -1;
		BYTE lastSentMsgId = 0;
		bool isServer = false;
		NetworkComState networkState;
		spt<NetOutputMessage> msgToSent = spt<NetOutputMessage>();
		uint64 clientLastCallBack = 0;
		uint64 serverLastUpdate = 0;
		map<BYTE, spt<NetOutputMessage>> confirmations;
		NetworkComMode mode = NetworkComMode::ESTABLISHING;

	public:

		int GetApplicationId() {
			return applicationId;
		}

		int GetPort() {
			return port;
		}

		string GetclientIp() {
			return clientIp;
		}

		bool ClientConnected() {
			return !clientIp.empty() && networkState == NetworkComState::UPDATING_SERVER;
		}

		int GetLastReceivedMsgId() {
			return lastReceivedMsgId;
		}

		NetworkComState GetNetworkState() {
			return networkState;
		}

		uint64 GetClientLastCallBack() {
			return clientLastCallBack;
		}

		uint64 GetServerLastUpdate() {
			return serverLastUpdate;
		}

		NetworkComMode GetMode() {
			return mode;
		}

		void SetMode(NetworkComMode mode) {
			this->mode = mode;
		}

		bool IsServer() {
			return isServer;
		}

		void Init(int applicationId, int port, bool isServer);

		void OnMessage(Msg& msg);

		void SendNetworkMessage(spt<NetOutputMessage> msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

	private:

		void ServerUpdate(const uint64 delta, const uint64 absolute);

		void ClientUpdate(const uint64 delta, const uint64 absolute);

	};

}// namespace