
#include "MsgListener.h"
#include "Facade.h"

namespace Cog {

	int MsgListener::idCounter = 0;

	MsgListener::MsgListener() : id(idCounter++) {

	}

	void MsgListener::RegisterListening(StringHash action1) {
		CogRegisterListener(action1, this);
	}

	void MsgListener::RegisterListening(StringHash action1, StringHash action2) {
		CogRegisterListener(action1, this);
		CogRegisterListener(action2, this);
	}

	void MsgListener::RegisterListening(StringHash action1, StringHash action2, StringHash action3) {
		CogRegisterListener(action1, this);
		CogRegisterListener(action2, this);
		CogRegisterListener(action3, this);
	}

	void MsgListener::RegisterListening(StringHash action1, StringHash action2, StringHash action3, StringHash action4) {
		CogRegisterListener(action1, this);
		CogRegisterListener(action2, this);
		CogRegisterListener(action3, this);
		CogRegisterListener(action4, this);
	}

	void MsgListener::SendMessage(BubblingType bubblingType, StringHash action, int subaction, MsgEvent* data, Node* source) const {
		Msg msg(bubblingType, action, subaction, id, source, data);
		CogSendMessage(msg, source);
	}

	void MsgListener::SendMessageNoBubbling(StringHash action, int subaction, MsgEvent* data, Node* source) const {
		CogSendDirectMessage(action, subaction, data, source, id);
	}

	void MsgListener::SendDirectMessage(StringHash action, int subaction, MsgEvent* data, Node* source, int targetId) const {
		CogSendDirectMessageToListener(action, subaction, data, source, targetId, id);
	}


}// namespace