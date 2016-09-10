#pragma once

#include "Component.h"

namespace Cog {

	class Network;

	class NetworkBindReceiver : public Component {
		OBJECT(NetworkBindReceiver)
	
	private:
		int param1;
		int param2;
		int port;
		Network* network;
		int lastReceivedMsgId = -1;

	public:
		
		void Init(int param1, int param2, int port);

		void OnMessage(Msg& msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

	private:

	};

}// namespace