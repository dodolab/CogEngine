#pragma once

#include "Component.h"
#include "DeltaInfo.h"

namespace Cog {

	/**
	* Component that receives delta messages from network and interpolates 
	* contiguous values
	*
	* Delta updates may be received automatically by messages 
	* or by calling the AcceptDeltaUpdate method
	*/
	class DeltaUpdate : public Component {
	private:
		// number of received messages
		int messagesReceived = 0;
		// initialization time
		uint64 initTime = 0;
		// previous message
		spt<DeltaInfo> previous;
		// actual received message used for interpolation
		spt<DeltaInfo> actual;
		// the newest received message
		spt<DeltaInfo> next;
		// speed of delta updates, set automatically according to the situation
		float deltaSpeed = 1;
		// number of seconds after which deltas are discarded and reinitialized
		int deltaDelayThreshold = 10000;
		// indicator whether delta messages should be accepted automatically
		// by messages
		bool acceptAutomatically = true;

	public:
		
		void OnInit();

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
		spt<DeltaInfo> GetActualDelta() {
			return actual;
		}

		/**
		* Gets actual speed of delta updates
		*/
		float GetDeltaSpeed() const {
			return deltaSpeed;
		}

		/**
		* Gets number of seconds after which deltas are discarded and reinitialized
		*/
		int GetDeltaDelayThreshold() const {
			return deltaDelayThreshold;
		}

		/**
		* Sets number of seconds after which deltas are discarded and reinitialized
		*/
		void SetDeltaDelayThreshold(int threshold) {
			this->deltaDelayThreshold = threshold;
		}

		/**
		* Gets indicator whether the deltas should be accepted 
		* automatically by messages
		*/
		bool AcceptAutomatically() const {
			return acceptAutomatically;
		}

		/**
		* Sets indicator whether the deltas should be accepted
		* automatically by messages
		*/
		void SetAcceptAutomatically(bool accept) {
			this->acceptAutomatically = accept;
		}

		/**
		* Accepts delta update
		*/
		void AcceptDeltaUpdate(spt<DeltaInfo> msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

	};

}// namespace