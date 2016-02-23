#pragma once

#include "ofMain.h"
#include "CogEngine.h"
#include "ofxCogMain.h"


#ifdef TESTING

using namespace Cog;

class CheckBehavior : public Behavior {
	OBJECT_PROTOTYPE(CheckBehavior)

	void Init() {
	}

	void OnMessage(Msg& msg) {

	}

public:
	int updateCounter = 0;
	virtual void Update(const uint64 delta, const uint64 absolute) {
		updateCounter++;
	}
};

class MessageBehavior : public Behavior {

	bool send;

public:
	MessageBehavior(bool send) : send(send) {

	}

	void Init() {
		if (!send) this->RegisterListening(owner->GetScene(), StringHash("MESSAGE_TEST"));
	}

	void OnMessage(Msg& msg) {
		if (!send && msg.GetAction() == "MESSAGE_TEST") {
			acceptedMessage = true;
		}
	}

public:
	bool acceptedMessage = false;
	virtual void Update(const uint64 delta, const uint64 absolute) {
		if (send) this->SendMessageNoBubbling("MESSAGE_TEST", 0, nullptr, owner);
	}
};



TEST_CASE("COG Engine test", "[class]")
{
	SECTION("Engine run")
	{
		// 1. init engine
		CogEngine::GetInstance().SetFps(20);
		CogEngine::GetInstance().Init();
		// 2. create scene
		Scene* scene = new Scene("main", false);
		CogEngine::GetInstance().stage->AddScene(scene, true);
		// 3. define nodes and behaviors
		Node* node = new Node("node");
		CheckBehavior* check = new CheckBehavior();
		node->AddBehavior(check);
		// 4. add main node to the scene
		scene->GetSceneNode()->AddChild(node);
		// 5. submit changes
		CogEngine::GetInstance().stage->GetRootObject()->SubmitChanges(true);

		// simulate update
		CogEngine::GetInstance().Update(16,16);

		REQUIRE(check->updateCounter == 1);
	}

	SECTION("Engine message")
	{
		// 1. init engine
		CogEngine::GetInstance().SetFps(20);
		CogEngine::GetInstance().Init();
		// 2. create scene
		Scene* scene = new Scene("main", false);
		CogEngine::GetInstance().stage->AddScene(scene, true);
		// 3. define nodes and behaviors

		// node 1 sends message to its child
		Node* node1 = new Node("node1");
		Node* node2 = new Node("node2");
		node1->AddChild(node2);
		MessageBehavior* sender = new MessageBehavior(true);
		MessageBehavior* recipient = new MessageBehavior(false);

		node1->AddBehavior(sender);
		node2->AddBehavior(recipient);


		// 4. add main node to the scene
		scene->GetSceneNode()->AddChild(node1);
		// 5. submit changes
		CogEngine::GetInstance().stage->GetRootObject()->SubmitChanges(true);

		// simulate update
		CogEngine::GetInstance().Update(16, 16);

		// check if recipient has obtained the message
		REQUIRE(recipient->acceptedMessage == true);
	}
}

#endif

