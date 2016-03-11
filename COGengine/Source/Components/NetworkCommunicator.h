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

	public:

		void Init(int applicationId, int port, bool isServer);

		void OnMessage(Msg& msg);

		void SendNetworkMessage(spt<NetMessage> msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

	private:

		void ServerUpdate(const uint64 delta, const uint64 absolute);

		void ClientUpdate(const uint64 delta, const uint64 absolute);

	};

}// namespace