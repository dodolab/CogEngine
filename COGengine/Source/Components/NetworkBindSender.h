#pragma once

#include "ofxSmartPointer.h"
#include "Component.h"

namespace Cog {

	class Network;

	class NetworkBindSender : public Component {
		OBJECT(NetworkBindSender)

	private:
		int param1;
		int param2;
		int port;
		string ip;
		Network* network;

	public:

		void Init(int param1, int param2, string ip, int port);

		void OnMessage(Msg& msg);

		void SendNetworkMessage(spt<NetMessage> msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

	private:

	};

}// namespace