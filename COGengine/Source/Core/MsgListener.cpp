
#include "MsgListener.h"
#include "Facade.h"
#include "Scene.h"

namespace Cog {

	int MsgListener::idCounter = 0;

	MsgListener::MsgListener() : id(idCounter++) {

	}

	void MsgListener::GlobalSubscribeForMessages(StrId action1) {
		CogRegisterGlobalListener(action1, this);
	}

	void MsgListener::GlobalSubscribeForMessages(StrId action1, StrId action2) {
		CogRegisterGlobalListener(action1, this);
		CogRegisterGlobalListener(action2, this);
	}

	void MsgListener::GlobalSubscribeForMessages(StrId action1, StrId action2, StrId action3) {
		CogRegisterGlobalListener(action1, this);
		CogRegisterGlobalListener(action2, this);
		CogRegisterGlobalListener(action3, this);
	}

	void MsgListener::GlobalSubscribeForMessages(StrId action1, StrId action2, StrId action3, StrId action4) {
		CogRegisterGlobalListener(action1, this);
		CogRegisterGlobalListener(action2, this);
		CogRegisterGlobalListener(action3, this);
		CogRegisterGlobalListener(action4, this);
	}

	void MsgListener::SubscribeForMessages(Scene* scene, StrId action1) {
		scene->RegisterListener(action1, this);
	}

	void MsgListener::SubscribeForMessages(Scene* scene, StrId action1, StrId action2) {
		SubscribeForMessages(scene, action1);
		SubscribeForMessages(scene, action2);
	}

	void MsgListener::SubscribeForMessages(Scene* scene, StrId action1, StrId action2, StrId action3) {
		SubscribeForMessages(scene, action1);
		SubscribeForMessages(scene, action2);
		SubscribeForMessages(scene, action3);
	}

	void MsgListener::SubscribeForMessages(Scene* scene, StrId action1, StrId action2, StrId action3, StrId action4) {
		SubscribeForMessages(scene, action1);
		SubscribeForMessages(scene, action2);
		SubscribeForMessages(scene, action3);
		SubscribeForMessages(scene, action4);
	}

	void MsgListener::SendMessage(HandlingType handlingType, StrId action, int subaction, MsgEvent* data, Node* source) const {
		Msg msg(handlingType, action, subaction, id, source, data);
		CogSendMessage(msg, source);
	}

	void MsgListener::SendMessageToListeners(StrId action, int subaction, MsgEvent* data, Node* source) const {
		CogSendMessageToListeners(action, subaction, data, source, id);
	}

	void MsgListener::SendDirectMessage(StrId action, int subaction, MsgEvent* data, Node* source, int targetId) const {
		CogSendDirectMessageToListener(action, subaction, data, source, targetId, id);
	}


}// namespace