
#include "MsgListener.h"
#include "Facade.h"
#include "Scene.h"

namespace Cog {

	// first id is always 1
	int MsgListener::idCounter = 1;

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


}// namespace