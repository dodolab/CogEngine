
#include "Behavior.h"

namespace Cog {

	int Behavior::idCounter = 0;

	Behavior::Behavior() : behState(BehState::ACTIVE_ALL),
		ended(false), id(idCounter++) {

	}

	void Behavior::RegisterListening(int action1) {
		RegisterListening(action1, -1, -1, -1);
	}

	void Behavior::RegisterListening(int action1, int action2) {
		RegisterListening(action1, action2, -1, -1);
	}

	void Behavior::RegisterListening(int action1, int action2, int action3) {
		RegisterListening(action1, action2, action3, -1);
	}

	void Behavior::RegisterListening(int action1, int action2, int action3, int action4) {
		if (action1 != -1) CogRegisterListener(action1, this);
		if (action2 != -1) CogRegisterListener(action2, this);
		if (action3 != -1) CogRegisterListener(action3, this);
		if (action4 != -1) CogRegisterListener(action4, this);
	}


	void Behavior::SendMessage(BubblingType bubblingType, int action, int subaction, void* data, Node* source) const {
		Msg msg(bubblingType, action, subaction, id, source, data);
		CogSendMessage(msg, source);
	}

	void Behavior::SendMessageNoBubbling(int action, int subaction, void* data, Node* source) const {
		Msg msg(BubblingType(Scope::DIRECT_NO_TRAVERSE, true, true), action, subaction, id, source, data);
		CogSendMessage(msg, source);
	}

	void Behavior::SendDirectMessage(int action, int subaction, void* data, Node* source, int targetId) const {
		Msg msg(BubblingType(Scope::DIRECT_NO_TRAVERSE, true, true), action, subaction, id, source, data);
		CogSendDirectMessageToBehavior(msg, targetId);
	}


}