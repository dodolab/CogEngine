
#include "Behavior.h"

namespace Cog {

	int Behavior::idCounter = 0;

	Behavior::Behavior() : behState(BehState::ACTIVE_ALL),
		ended(false), id(idCounter++) {

	}

	
	void Behavior::RegisterListening(StringHash action1){
		CogRegisterListener(action1, this);
	}

	void Behavior::RegisterListening(StringHash action1, StringHash action2) {
		CogRegisterListener(action1, this);
		CogRegisterListener(action2, this);
	}

	void Behavior::RegisterListening(StringHash action1, StringHash action2, StringHash action3) {
		CogRegisterListener(action1, this);
		CogRegisterListener(action2, this);
		CogRegisterListener(action3, this);
	}

	void Behavior::RegisterListening(StringHash action1, StringHash action2, StringHash action3, StringHash action4) {
		CogRegisterListener(action1, this);
		CogRegisterListener(action2, this);
		CogRegisterListener(action3, this);
		CogRegisterListener(action4, this);
	}

	void Behavior::SendMessage(BubblingType bubblingType, StringHash action, int subaction, void* data, Node* source) const {
		Msg msg(bubblingType, action, subaction, id, source, data);
		CogSendMessage(msg, source);
	}

	void Behavior::SendMessageNoBubbling(StringHash action, int subaction, void* data, Node* source) const {
		Msg msg(BubblingType(Scope::DIRECT_NO_TRAVERSE, true, true), action, subaction, id, source, data);
		CogSendMessage(msg, source);
	}

	void Behavior::SendDirectMessage(StringHash action, int subaction, void* data, Node* source, int targetId) const {
		Msg msg(BubblingType(Scope::DIRECT_NO_TRAVERSE, true, true), action, subaction, id, source, data);
		CogSendDirectMessageToBehavior(msg, targetId);
	}


}// namespace