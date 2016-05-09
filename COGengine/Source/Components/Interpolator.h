#pragma once

#include "Component.h"
#include "UpdateInfo.h"

namespace Cog {

	/**
	* Component that receives messages from network and interpolates 
	* continuous values
	*
	* Updates may be received automatically by messages 
	* or by calling the AcceptUpdateMessage method
	*/
	class Interpolator : public Component {
	private:
		// number of received messages
		int messagesReceived = 0;
		// initialization time
		uint64 initTime = 0;
		// previous message
		spt<UpdateInfo> previous;
		// actual received message used for interpolation
		spt<UpdateInfo> actual;
		// the newest received message
		spt<UpdateInfo> next;
		// speed of updates, set automatically according to the situation
		float updateSpeed = 1;
		// number of seconds after which updates are discarded and reinitialized
		int updateDelayThreshold = 10000;
		// indicator whether update messages should be accepted automatically
		// by messaging system
		bool acceptAutomatically = true;

	public:
		
		void OnInit();

		void Restart();

		void OnMessage(Msg& msg);

		/**
		* Gets initialization time
		*/
		uint64 GetInitTime() const {
			return initTime;
		}

		/**
		* Gets actual received message used for interpolation
		*/
		spt<UpdateInfo> GetActualUpdate() {
			return actual;
		}

		/**
		* Gets actual speed of updates
		*/
		float GetUpdateSpeed() const {
			return updateSpeed;
		}

		/**
		* Gets number of seconds after which updates are discarded and reinitialized
		*/
		int GetUpdateDelayThreshold() const {
			return updateDelayThreshold;
		}

		/**
		* Sets number of seconds after which updates are discarded and reinitialized
		*/
		void SetUpdateDelayThreshold(int threshold) {
			this->updateDelayThreshold = threshold;
		}

		/**
		* Gets indicator whether the messages should be accepted 
		* automatically by messages
		*/
		bool AcceptAutomatically() const {
			return acceptAutomatically;
		}

		/**
		* Sets indicator whether the messages should be accepted
		* automatically by messaging system
		*/
		void SetAcceptAutomatically(bool accept) {
			this->acceptAutomatically = accept;
		}

		/**
		* Accepts update message
		*/
		void AcceptUpdateMessage(spt<UpdateInfo> msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

	};

}// namespace