#pragma once

#include "ofxSmartPointer.h"
#include "Component.h"

namespace Cog {

	class Network;

	enum class NetworkComState {
		BROADCASTING,	// for client
		LISTENING,		// for server
		UPDATING_SERVER,
		UPDATING_CLIENT
	};

	class NetworkCommunicator : public Component {
		OBJECT(NetworkCommunicator)

	private:
		int applicationId;
		int port;
		string clientIp;
		Network* network;
		int lastReceivedMsgId = -1;
		bool isServer = false;
		NetworkComState networkState;
		spt<NetMessage> msgToSent = spt<NetMessage>();
		uint64 clientLastCallBack = 0;
		uint64 serverLastUpdate = 0;

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

		void Init(int applicationId, int port, bool isServer);

		void OnMessage(Msg& msg);

		void SendNetworkMessage(spt<NetMessage> msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

	private:

		void ServerUpdate(const uint64 delta, const uint64 absolute);

		void ClientUpdate(const uint64 delta, const uint64 absolute);

	};

}// namespace