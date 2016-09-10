#pragma once

#include "ofMain.h"
#include "CogEngine.h"
#include "ofxCogMain.h"
#include "StateMachine.h"
#include "State.h"
#include "StringHash.h"


#include "catch.hpp"
using namespace Cog;

class MiningState : public State {
public:

	MiningState(StringHash state) : State(state) {

	}


	virtual void Update(const uint64 delta, const uint64 absolute) {
		int progress = owner->GetAttr<int>(StringHash("PROGRESS"));
		progress += 10;
		cout << "mining ..." << endl;
		if (progress >= 100) {
			cout << "mining finished" << endl;
			this->GetParent()->ChangeState(StringHash("REST"));
		}

		owner->ChangeAttr(StringHash("PROGRESS"), progress);
	}
};

class RestState : public State {
public:

	RestState(StringHash state) : State(state) {

	}

	virtual void Update(const uint64 delta, const uint64 absolute) {

	}
};

class StateSwitcher : public State {
public:
	StringHash nextState;

	StateSwitcher(StringHash state, StringHash nextState) : State(state), nextState(nextState) {

	}

	virtual void Update(const uint64 delta, const uint64 absolute) {
		this->GetParent()->ChangeState(nextState);
	}
};

TEST_CASE("StateMachineTest", "[class]")
{
	SECTION("Miner test")
	{
		// 1. init engine
		CogEngine::GetInstance().SetFps(20);
		CogEngine::GetInstance().Init();
		// 2. create scene
		Scene* scene = new Scene("main", false);
		CogEngine::GetInstance().stage->AddScene(scene, true);
		// 3. define nodes and behaviors
		Node* node = new Node("node");
		StateMachine* stMach = new StateMachine();
		
		// add MINER state and set it as an actual state
		MiningState* miner = new MiningState(StringHash("MINER"));
		stMach->ChangeState(miner);
		// add REST state
		stMach->AddLocalState(new RestState(StringHash("REST")));
		node->AddBehavior(stMach);
		// add PROGRESS attribute
		node->AddAttr(StringHash("PROGRESS"), (int)0);

		// 4. add main node to the scene
		scene->GetSceneNode()->AddChild(node);
		// 5. submit changes
		CogEngine::GetInstance().stage->GetRootObject()->SubmitChanges(true);
		
		for (int i = 0; i < 10; i++) {
			// simulate update 10x
			CogEngine::GetInstance().Update(10, 10);
		}
		
		// check if it is in resting state
		REQUIRE(stMach->GetCurrentState()->GetState() == StringHash("REST"));
	}


	SECTION("Switching test")
	{
		// 1. init engine
		CogEngine::GetInstance().SetFps(20);
		CogEngine::GetInstance().Init();
		// 2. create scene
		Scene* scene = new Scene("main", false);
		CogEngine::GetInstance().stage->AddScene(scene, true);
		// 3. define nodes and behaviors
		Node* node = new Node("node");
		StateMachine* stMach = new StateMachine();

		// add 11 states that will be switching
		for (int i = 0; i <= 10; i++) {
			StateSwitcher* switcher = new StateSwitcher(StringHash(i), StringHash(i + 1));
			stMach->AddLocalState(switcher);
			if (i == 0) stMach->ChangeState(switcher);
		}

		node->AddBehavior(stMach);
		// 4. add main node to the scene
		scene->GetSceneNode()->AddChild(node);
		// 5. submit changes
		CogEngine::GetInstance().stage->GetRootObject()->SubmitChanges(true);

		for (int i = 0; i < 10; i++) {
			// simulate update 10x
			CogEngine::GetInstance().Update(10, 10);
		}

		// check if it is in 10th state
		REQUIRE(stMach->GetCurrentState()->GetState() == StringHash(10));
	}
}

