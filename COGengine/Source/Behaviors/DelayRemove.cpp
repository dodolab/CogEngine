
#include "DelayRemove.h"
#include "CogEngine.h"

namespace Cog {

	void DelayRemove::Update(const uint64 delta, const uint64 absolute) {
		actual += (int)delta;

		if (actual > millis) {
			owner->GetParent()->RemoveChild(owner, erase);
			Finish();
		}
	}

}// namespace