
#include "NetworkBinder.h"
#include "NetMessage.h"

namespace Cog {

	void NetworkBinder::Init() {
		RegisterGlobalListening(ACT_NET_MESSAGE_RECEIVED);
		
	}

	void NetworkBinder::OnMessage(Msg& msg) {
		if (msg.GetAction() == StringHash(ACT_NET_MESSAGE_RECEIVED)) {

			// received net message
			NetworkMsgEvent* msgEvent = msg.GetDataS<NetworkMsgEvent>();
			spt<NetMessage> netMsg = msgEvent->msg;
			
			switch (netMsg->GetMsgType()) {
				case NetMsgType::UPDATE:
					AcceptDeltaUpdate(netMsg);
				break;
			}
		}
	}

	void NetworkBinder::AcceptDeltaUpdate(NetMessage* msg) {
		if (!gotFirstMessage) {
			gotFirstMessage = true;
			this->parameter1 = msg->GetFloatParameter1();
			this->parameter2 = msg->GetFloatParameter2();
			this->parameter3 = msg->GetFloatParameter3();
			this->parameterTime = msg->GetMsgTime();
			this->previousParameter1 = msg->GetFloatParameter1();
			this->previousParameter2 = msg->GetFloatParameter2();
			this->previousParameter3 = msg->GetFloatParameter3();
			this->previousParameterTime = msg->GetMsgTime();
		}
		else if (!gotSecondMessage) {
			gotSecondMessage = true;
			this->nextParameter1 = msg->GetFloatParameter1();
			this->nextParameter2 = msg->GetFloatParameter2();
			this->nextParameter3 = msg->GetFloatParameter3();
			this->nextParameterTime = msg->GetMsgTime();
		}
		else {
			this->previousParameter1 = this->nextParameter1;
			this->previousParameter2 = this->nextParameter2;
			this->previousParameter3 = this->nextParameter3;
			this->previousParameterTime = this->nextParameterTime;

			this->nextParameter1 = msg->GetFloatParameter1();
			this->nextParameter2 = msg->GetFloatParameter2();
			this->nextParameter3 = msg->GetFloatParameter3();
			this->nextParameterTime = msg->GetMsgTime();
		}
	}

	void NetworkBinder::Update(const uint64 delta, const uint64 absolute) {
		if (gotFirstMessage && gotSecondMessage) {

			parameterTime += (int)(delta*deltaSpeed);

			if (parameterTime - nextParameterTime < extrapolationTimeout) {

				if (this->parameterTime < this->previousParameterTime) {
					deltaSpeed *= 1.1f;
				}
				else {
					deltaSpeed = 1;
				}

				auto diffTime = this->nextParameterTime - this->previousParameterTime;
				auto diffParam1 = this->nextParameter1 - this->previousParameter1;
				auto diffParam2 = this->nextParameter2 - this->previousParameter2;
				auto diffParam3 = this->nextParameter3 - this->previousParameter3;

				auto diffLow = this->parameterTime - this->previousParameterTime;

				this->parameter1 = this->previousParameter1 + diffParam1*(((float)diffLow) / diffTime);
				this->parameter2 = this->previousParameter2 + diffParam2*(((float)diffLow) / diffTime);
				this->parameter3 = this->previousParameter3 + diffParam3*(((float)diffLow) / diffTime);
			}
		}
	}


}// namespace