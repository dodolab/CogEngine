
#include "Interpolator.h"
#include "NetMessage.h"
#include "UpdateMessage.h"

namespace Cog {

	void Interpolator::OnInit() {
		GlobalSubscribeForMessages(ACT_UPDATE_MESSAGE_RECEIVED);
		Restart();
	}

	void Interpolator::Restart() {
		messagesReceived = 0;
		initTime = 0;
		previous = spt<UpdateInfo>();
		next = spt<UpdateInfo>();
		updateSpeed = 1;
		actual = spt<UpdateInfo>(new UpdateInfo());
	}

	void Interpolator::OnMessage(Msg& msg) {
		if (acceptAutomatically) {
			if (msg.HasAction(ACT_UPDATE_MESSAGE_RECEIVED)) {
				// received update message
				spt<CommonEvent<UpdateInfo>> msgEvent = msg.GetData<CommonEvent<UpdateInfo>>();
				spt<UpdateInfo> netMsg = msgEvent->value;
				AcceptUpdateMessage(netMsg);
			}
		}
	}

	void Interpolator::AcceptUpdateMessage(spt<UpdateInfo> msg) {

		if (!previous) {
			// set first receved message
			initTime = msg->time;
			messagesReceived = 1;
			previous = msg;
			actual->time = msg->GetTime();
		}
		else if (!next) {
			// set second received message
			next = msg;
			messagesReceived++;
		}
		else {
			previous = next;
			next = msg;
			messagesReceived++;
		}
	}

	void Interpolator::Update(const uint64 delta, const uint64 absolute) {

		if (previous && next) {

			if (abs((int)((int)actual->time - this->next->time)) > updateDelayThreshold) {
				// disconnected for a long time
				this->actual->time = this->next->time;
				messagesReceived = 1;
				initTime = next->time;
			}

			actual->time += (int)(delta*updateSpeed);

			// calculate average interval between frames
			float averageFrameDiff = ((int)(next->time - initTime)) / ((float)messagesReceived);

			if ((actual->time < this->next->time) || ((actual->time - this->next->time) < averageFrameDiff*2)) {
				// use interpolation

				if (this->actual->time < this->previous->time) {
					// accelerate speed little bit
					if(updateSpeed < 2.5f) updateSpeed *= 1.1f;

				}
				else if (this->actual->time > this->next->time) {
					// deccelerate speed little bit
					if(updateSpeed > 0.8f) updateSpeed /= 1.1f;
					
				}
				else {
					// converge to 1
					if (updateSpeed > 1) updateSpeed /= 1.2f;
					else if (updateSpeed < 1) updateSpeed *= 1.2f;
					else updateSpeed = 1;
				}
			
				int diffTotal = (int)(this->next->time - this->previous->time);
				int diffActual = (int)(this->actual->time - this->previous->time);
				
				// calculate percentage position between previous and next frame
				float ratio =diffActual / ((float)diffTotal);

				// calculate actual values based on linear interpolation
				for (auto& dt : next->continuousValues) {
					auto prevIt = previous->continuousValues.find(dt.first);
					
					float nextVal = dt.second;
					float prevVal = prevIt != previous->continuousValues.end() ? (*prevIt).second : nextVal;

					float actualVal = prevVal + (nextVal - prevVal)*ratio;
					actual->continuousValues[dt.first] = actualVal;
				}
			}
		}
	}
}// namespace