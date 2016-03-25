
#include "DelayAction.h"
#include "CogEngine.h"

namespace Cog {

	void DelayAction::Update(const uint64 delta, const uint64 absolute) {
		actual += (int)delta;

		if (actual > millis) {
			SendDirectMessage(ACT_DELAY_ACTION, 0, data, owner, targetId);
			Finish();
		}
	}


}// namespace