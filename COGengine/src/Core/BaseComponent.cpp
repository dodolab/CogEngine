
#include "BaseComponent.h"
#include "Facade.h"
#include "Scene.h"

namespace Cog {

	// first id is always 1
	int BaseComponent::idCounter = 1;

	BaseComponent::BaseComponent() : id(idCounter++) {

	}

	void BaseComponent::GlobalSubscribeForMessages(StrId action1) {
		CogRegisterGlobalListener(action1, this);
	}

	void BaseComponent::GlobalSubscribeForMessages(StrId action1, StrId action2) {
		CogRegisterGlobalListener(action1, this);
		CogRegisterGlobalListener(action2, this);
	}

	void BaseComponent::GlobalSubscribeForMessages(StrId action1, StrId action2, StrId action3) {
		CogRegisterGlobalListener(action1, this);
		CogRegisterGlobalListener(action2, this);
		CogRegisterGlobalListener(action3, this);
	}

	void BaseComponent::GlobalSubscribeForMessages(StrId action1, StrId action2, StrId action3, StrId action4) {
		CogRegisterGlobalListener(action1, this);
		CogRegisterGlobalListener(action2, this);
		CogRegisterGlobalListener(action3, this);
		CogRegisterGlobalListener(action4, this);
	}

	void BaseComponent::SubscribeForMessages(Scene* scene, StrId action1) {
		scene->RegisterListener(action1, this);
	}

	void BaseComponent::SubscribeForMessages(Scene* scene, StrId action1, StrId action2) {
		SubscribeForMessages(scene, action1);
		SubscribeForMessages(scene, action2);
	}

	void BaseComponent::SubscribeForMessages(Scene* scene, StrId action1, StrId action2, StrId action3) {
		SubscribeForMessages(scene, action1);
		SubscribeForMessages(scene, action2);
		SubscribeForMessages(scene, action3);
	}

	void BaseComponent::SubscribeForMessages(Scene* scene, StrId action1, StrId action2, StrId action3, StrId action4) {
		SubscribeForMessages(scene, action1);
		SubscribeForMessages(scene, action2);
		SubscribeForMessages(scene, action3);
		SubscribeForMessages(scene, action4);
	}


}// namespace