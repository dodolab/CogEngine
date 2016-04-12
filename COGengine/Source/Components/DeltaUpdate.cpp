
#include "DeltaUpdate.h"
#include "NetMessage.h"
#include "DeltaMessage.h"

namespace Cog {

	void DeltaUpdate::OnInit() {
		GlobalSubscribeForMessages(ACT_DELTA_MESSAGE_RECEIVED);
		actual = spt<DeltaInfo>(new DeltaInfo());
	}

	void DeltaUpdate::OnMessage(Msg& msg) {

		if (msg.HasAction(ACT_DELTA_MESSAGE_RECEIVED)) {
			// received delta message
			CommonEvent<DeltaInfo>* msgEvent = msg.GetData<CommonEvent<DeltaInfo>>();
			spt<DeltaInfo> netMsg = msgEvent->value;
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