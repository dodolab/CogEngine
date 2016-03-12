
#include "MsgListener.h"
#include "Facade.h"
#include "Scene.h"

namespace Cog {

	int MsgListener::idCounter = 0;

	MsgListener::MsgListener() : id(idCounter++) {

	}

	void MsgListener::RegisterGlobalListening(StringHash action1) {
		CogRegisterGlobalListener(action1, this);
	}

	void MsgListener::RegisterGlobalListening(StringHash action1, StringHash action2) {
		CogRegisterGlobalListener(action1, this);
		CogRegisterGlobalListener(action2, this);
	}

	void MsgListener::RegisterGlobalListening(StringHash action1, StringHash action2, StringHash action3) {
		CogRegisterGlobalListener(action1, this);
		CogRegisterGlobalListener(action2, this);
		CogRegisterGlobalListener(action3, this);
	}

	void MsgListener::RegisterGlobalListening(StringHash action1, StringHash action2, StringHash action3, StringHash action4) {
		CogRegisterGlobalListener(action1, this);
		CogRegisterGlobalListener(action2, this);
		CogRegisterGlobalListener(action3, this);
		CogRegisterGlobalListener(action4, this);
	}

	void MsgListener::RegisterListening(Scene* scene, StringHash action1) {
		scene->RegisterListener(action1, this);
	}

	void MsgListener::RegisterListening(Scene* scene, StringHash action1, StringHash action2) {
		RegisterListening(scene, action1);
		RegisterListening(scene, action2);
	}

	void MsgListener::RegisterListening(Scene* scene, StringHash action1, StringHash action2, StringHash action3) {
		RegisterListening(scene, action1);
		RegisterListening(scene, action2);
		RegisterListening(scene, action3);
	}

	void MsgListener::RegisterListening(Scene* scene, StringHash action1, StringHash action2, StringHash action3, StringHash action4) {
		RegisterListening(scene, action1);
		RegisterListening(scene, action2);
		RegisterListening(scene, action3);
		RegisterListening(scene, action4);
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