#pragma once

#include "CogBehavior.h"
#include "CogTranslateAnim.h"
#include "CogMsg.h"

namespace Cog {

	/**x
	* Behavior that removes object with delay
	*/
	class CogDelayRemove : public CogBehavior {
	protected:
		// number of milliseconds for delay
		int millis;
		// actual time
		int actual;
		// if true, object will be erased
		bool erase;

	public:

		/**
		* Creates a new behavior that removes object with delay
		* @param millis number of milliseconds for delay
		* @param erase if true, object will be erased
		*/
		CogDelayRemove(int millis, bool erase) : millis(millis), actual(0), erase(erase) {

		}

		virtual void Update(const uint64 delta, const uint64 absolute) {
			actual += delta;

			if (actual > millis) {
				owner->GetParent()->RemoveChild(owner, erase);
				Finish();
			}
		}


	};

}