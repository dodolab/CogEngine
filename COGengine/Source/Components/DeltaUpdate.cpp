
#include "DeltaUpdate.h"
#include "NetMessage.h"
#include "DeltaMessage.h"

namespace Cog {

	void DeltaUpdate::Init() {
		RegisterGlobalListening(ACT_DELTA_MESSAGE_RECEIVED, ACT_NET_MESSAGE_RECEIVED);
		actual = spt<DeltaInfo>(new DeltaInfo());
	}

	void DeltaUpdate::OnMessage(Msg& msg) {

		if (msg.GetAction() == StringHash(ACT_NET_MESSAGE_RECEIVED)) {
			// todo: message could be various types
			NetworkMsgEvent* msgEvent = msg.GetDataS<NetworkMsgEvent>();
			auto netMsg = msgEvent->msg;
			
			if (netMsg->GetAction() == StringHash(NET_MSG_DELTA_UPDATE)) {
				auto netReader = new NetReader(netMsg->GetData(), netMsg->GetDataLength());
				spt<DeltaMessage> deltaMsg = spt<DeltaMessage>(new DeltaMessage());
				deltaMsg->LoadFromStream(netReader);
				spt<DeltaInfo> deltaInfo = spt<DeltaInfo>(new DeltaInfo());
				deltaInfo->deltas = deltaMsg->vals;
				deltaInfo->time = netMsg->GetMsgTime();
				AcceptDeltaUpdate(deltaInfo);
			}

		}else if (msg.GetAction() == StringHash(ACT_DELTA_MESSAGE_RECEIVED)) {

			// received delta message
			DeltaMsgEvent* msgEvent = msg.GetDataS<DeltaMsgEvent>();
			spt<DeltaInfo> netMsg = msgEvent->msg;
			AcceptDeltaUpdate(netMsg);
		}
	}

	void DeltaUpdate::AcceptDeltaUpdate(spt<DeltaInfo> msg) {

		if (!previous) {
			previous = msg;
			actual->time = msg->time;
		}
		else if (!next) {
			next = msg;
			actual->time = previous->time;
		}
		else {
			this->previous = next;
			this->next = msg;
		}
	}

	void DeltaUpdate::Update(const uint64 delta, const uint64 absolute) {
		if (previous && next) {

			actual->time += (int)(delta*deltaSpeed);

			if ((actual->time < this->next->time) || (actual->time - this->next->time < extrapolationTimeout)) {

				if (this->actual->time < this->previous->time) {
					deltaSpeed *= 1.1f;
				}
				else if (this->actual->time > this->next->time) {
					deltaSpeed /= 1.1f;
				}
				else {
					deltaSpeed = 1;
				}
			
				int diffTime = ((int)this->next->time) - this->previous->time;
				int diffLow = ((int)this->actual->time) - this->previous->time;

				for (auto& dt : next->deltas) {
					auto& prevIt = previous->deltas.find(dt.first);
					
					float nextVal = dt.second;
					float prevVal = prevIt != previous->deltas.end() ? (*prevIt).second : nextVal;

					float actualVal = prevVal + (nextVal - prevVal)*(((float)diffLow) / diffTime);
					actual->deltas[dt.first] = actualVal;
				}
			}
		}
	}


}// namespace