
#include "DelayAction.h"
#include "Node.h"
#include "Scene.h"

namespace Cog {

	void DelayAction::Update(const uint64 delta, const uint64 absolute) {
		if (absolute > deliverTime) {
			owner->GetScene()->SendMessage(this->message);
			Finish();
		}
	}


}// namespace