
#include "NetworkBinder.h"
#include "NetMessage.h"

namespace Cog {

	void NetworkBinder::Init() {
		RegisterGlobalListening(ACT_NET_MESSAGE_RECEIVED);
		this->parameter = 0;
	}

	void NetworkBinder::OnMessage(Msg& msg) {
		if (msg.GetAction() == StringHash(ACT_NET_MESSAGE_RECEIVED)) {

			// received net message
			NetworkMsgEvent* msgEvent = msg.GetDataS<NetworkMsgEvent>();
			spt<NetMessage> netMsg = msgEvent->msg;
			
			switch (netMsg->GetMsgType()) {
				case NetMsgType::DELTA_UPDATE:
					AcceptDeltaUpdate(netMsg);
				break;
			}
		}
	}

	void NetworkBinder::AcceptDeltaUpdate(NetMessage* msg) {
		if (!gotFirstMessage) {
			gotFirstMessage = true;
			this->parameter = msg->GetParameter();
			this->parameterTime = msg->GetMsgTime();
			this->previousParameter = msg->GetParameter();
			this->previousParameterTime = msg->GetMsgTime();
		}
		else if (!gotSecondMessage) {
			gotSecondMessage = true;
			this->nextParameter = msg->GetParameter();
			this->nextParameterTime = msg->GetMsgTime();
		}
		else {
			this->previousParameter = this->nextParameter;
			this->previousParameterTime = this->nextParameterTime;

			this->nextParameter = msg->GetParameter();
			this->nextParameterTime = msg->GetMsgTime();
		}
	}

	void NetworkBinder::Update(const uint64 delta, const uint64 absolute) {
		if (gotFirstMessage && gotSecondMessage) {
			this->parameterTime += delta*deltaSpeed;

			if (this->parameterTime < this->previousParameterTime) {
				deltaSpeed *= 1.1f;
			}
			else {
				deltaSpeed = 1;
			}

			auto diffTime = this->nextParameterTime - this->previousParameterTime;
			auto diffParam = this->nextParameter - this->previousParameter;

			auto diffLow = this->parameterTime - this->previousParameterTime;
		
			this->parameter = this->previousParameter + diffParam*(((float)diffLow)/diffTime);
		}
	}


}// namespace