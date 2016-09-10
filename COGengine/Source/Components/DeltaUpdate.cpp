
#include "DeltaUpdate.h"
#include "NetMessage.h"
#include "DeltaMessage.h"

namespace Cog {

	void DeltaUpdate::Init() {
		RegisterGlobalListening(ACT_DELTA_MESSAGE_RECEIVED, ACT_NET_MESSAGE_RECEIVED);
		actual = spt<DeltaInfo>(new DeltaInfo());
	}

	void DeltaUpdate::OnMessage(Msg& msg) {

		if (msg.HasAction(ACT_NET_MESSAGE_RECEIVED)) {
			// todo: message could be various types
			NetworkMsgEvent* msgEvent = msg.GetDataS<NetworkMsgEvent>();
			auto netMsg = msgEvent->msg;
			
			if (netMsg->GetAction() == StringHash(NET_MSG_DELTA_UPDATE)) {
				auto netReader = new NetReader(netMsg->GetData(), netMsg->GetDataLength());
				spt<DeltaMessage> deltaMsg = spt<DeltaMessage>(new DeltaMessage());
				deltaMsg->LoadFromStream(netReader);
				spt<DeltaInfo> deltaInfo = spt<DeltaInfo>(new DeltaInfo());
				deltaInfo->deltas = deltaMsg->deltas;
				deltaInfo->teleports = deltaMsg->teleports;
				deltaInfo->time = netMsg->GetMsgTime();
				AcceptDeltaUpdate(deltaInfo);
			}

		}else if (msg.HasAction(ACT_DELTA_MESSAGE_RECEIVED)) {

			// received delta message
			DeltaMsgEvent* msgEvent = msg.GetDataS<DeltaMsgEvent>();
			spt<DeltaInfo> netMsg = msgEvent->msg;
			AcceptDeltaUpdate(netMsg);
		}
	}

	void DeltaUpdate::AcceptDeltaUpdate(spt<DeltaInfo> msg) {

		if (!previous) {
			initTime = msg->time;
			messagesObtained = 1;
			previous = msg;
			actual->time = msg->time;
		}
		else if (!next) {
			next = msg;
			messagesObtained++;
		}
		else {
			previous = next;
			next = msg;
			messagesObtained++;
		}
	}

	void DeltaUpdate::Update(const uint64 delta, const uint64 absolute) {
		if (previous && next) {

			if (abs((int)((int)actual->time - this->next->time)) > 10000) {
				// disconnected for a long time
				this->actual->time = this->next->time;
				messagesObtained = 1;
				initTime = next->time;
			}

			actual->time += (int)(delta*deltaSpeed);

			float averageFrameDiff = ((int)(next->time - initTime)) / ((float)messagesObtained);

			if ((actual->time < this->next->time) || ((actual->time - this->next->time) < averageFrameDiff*2)) {

				if (this->actual->time < this->previous->time) {
					if(deltaSpeed < 2.5f) deltaSpeed *= 1.1f;

				}
				else if (this->actual->time > this->next->time) {
					if(deltaSpeed > 0.8f) deltaSpeed /= 1.1f;
					//this->actual->time -= (this->actual->time - this->next->time)/2;
					// extrapolation 
					
				}
				else {
					if (deltaSpeed > 1) deltaSpeed /= 1.2f;
					else if (deltaSpeed < 1) deltaSpeed *= 1.2f;
					else deltaSpeed = 1;
				}
			
				int diffTotal = (int)(this->next->time - this->previous->time);
				int diffActual = (int)(this->actual->time - this->previous->time);
				float ratio =diffActual / ((float)diffTotal);

				for (auto& dt : next->deltas) {
					auto prevIt = previous->deltas.find(dt.first);
					
					float nextVal = dt.second;
					float prevVal = prevIt != previous->deltas.end() ? (*prevIt).second : nextVal;

					float actualVal = prevVal + (nextVal - prevVal)*ratio;
					actual->deltas[dt.first] = actualVal;
				}
			}
		}
	}
}// namespace