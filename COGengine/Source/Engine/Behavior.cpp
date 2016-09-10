#include "Behavior.h"
#include "Node.h"

namespace Cog {

	Behavior::Behavior() : ended(false) {

	}

	void Behavior::RegisterListening(StringHash action1) {
		MsgListener::RegisterListening(owner->GetScene(), action1);
	}


	void Behavior::RegisterListening(StringHash action1, StringHash action2) {
		MsgListener::RegisterListening(owner->GetScene(), action1);
		MsgListener::RegisterListening(owner->GetScene(), action2);
	}

	void Behavior::RegisterListening(StringHash action1, StringHash action2, StringHash action3) {
		MsgListener::RegisterListening(owner->GetScene(), action1);
		MsgListener::RegisterListening(owner->GetScene(), action2);
		MsgListener::RegisterListening(owner->GetScene(), action3);
	}

	void Behavior::RegisterListening(StringHash action1, StringHash action2, StringHash action3, StringHash action4) {
		MsgListener::RegisterListening(owner->GetScene(), action1);
		MsgListener::RegisterListening(owner->GetScene(), action2);
		MsgListener::RegisterListening(owner->GetScene(), action3);
		MsgListener::RegisterListening(owner->GetScene(), action4);
	}


}// namespace