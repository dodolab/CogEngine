
#include "CogEngine.h"
#include "ofxCogCommon.h"
#include "NetworkBindReceiver.h"
#include "Network.h"
#include "NetMessage.h"

namespace Cog {

	void NetworkBindReceiver::Init(int param1, int param2, int port) {
		this->port = port;
		this->param1 = param1;
		this->param2 = param2;

		network = GETCOMPONENT(Network);
		network->SetupUDPReceiver(port, 10000, true);
	}

	void NetworkBindReceiver::OnMessage(Msg& msg) {

	}

	void NetworkBindReceiver::Update(const uint64 delta, const uint64 absolute) {
		auto message = network->ReceiveUDPMessage(param1, param2, 0);

		if (message) {
			if (message->GetId() > this->lastReceivedMsgId) {
				// dispose old messages
				this->lastReceivedMsgId = message->GetId();
				SendMessageNoBubbling(StringHash(ACT_NET_MESSAGE_RECEIVED), 0, new NetworkMsgEvent(message), nullptr);
			}
		}
	}

} // namespace