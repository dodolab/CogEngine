#pragma once

#include "ofMain.h"
#include "CogEngine.h"
#include "ofxCogMain.h"
#include "Goal.h"
#include "StringHash.h"

#include "catch.hpp"
using namespace Cog;

class DummyGoal : public Goal {
 public:
	bool complete;

	DummyGoal(bool complete) : Goal(StringHash()), complete(complete) {

	}

	virtual void Update(const uint64 delta, const uint64 absolute) {
		if(complete) Complete();
		else Fail();
	}
};


TEST_CASE("GoalTest", "[class]")
{
	SECTION("All complete test")
	{
		// 1. init engine
		CogEngine::GetInstance().SetFps(20);
		CogEngine::GetInstance().Init();
		// 2. create scene
		Scene* scene = new Scene("main", false);
		CogEngine::GetInstance().stage->AddScene(scene, true);
		// 3. define nodes and behaviors
		Node* node = new Node("node");
		
		Goal* g1 = new DummyGoal(true);
		Goal* g2 = new DummyGoal(true);
		Goal* g3 = new DummyGoal(true);
		Goal* g4 = new DummyGoal(true);
		Goal* g5 = new DummyGoal(true);
		GoalComposite* composite = new GoalComposite(StringHash(), true);
		composite->AddSubgoal(g1);
		composite->AddSubgoal(g2);
		composite->AddSubgoal(g3);
		composite->AddSubgoal(g4);
		composite->AddSubgoal(g5);

		node->AddBehavior(composite);

		// 4. add main node to the scene
		scene->GetSceneNode()->AddChild(node);
		// 5. submit changes
		CogEngine::GetInstance().stage->GetRootObject()->SubmitChanges(true);
		
		// update once
		CogEngine::GetInstance().Update(10, 10);
		
		// all goals completed
		REQUIRE(g1->IsCompleted());
		REQUIRE(g2->IsCompleted());
		REQUIRE(g3->IsCompleted());
		REQUIRE(g4->IsCompleted());
		REQUIRE(g5->IsCompleted());
		REQUIRE(composite->IsCompleted());
	}

	SECTION("All fail test")
	{
		// 1. init engine
		CogEngine::GetInstance().SetFps(20);
		CogEngine::GetInstance().Init();
		// 2. create scene
		Scene* scene = new Scene("main", false);
		CogEngine::GetInstance().stage->AddScene(scene, true);
		// 3. define nodes and behaviors
		Node* node = new Node("node");

		Goal* g1 = new DummyGoal(true);
		Goal* g2 = new DummyGoal(true);
		Goal* g3 = new DummyGoal(false);
		Goal* g4 = new DummyGoal(true);
		Goal* g5 = new DummyGoal(false);
		GoalComposite* composite = new GoalComposite(StringHash(), false);
		composite->AddSubgoal(g1);
		composite->AddSubgoal(g2);
		composite->AddSubgoal(g3);
		composite->AddSubgoal(g4);
		composite->AddSubgoal(g5);

		node->AddBehavior(composite);

		// 4. add main node to the scene
		scene->GetSceneNode()->AddChild(node);
		// 5. submit changes
		CogEngine::GetInstance().stage->GetRootObject()->SubmitChanges(true);

		// update once
		CogEngine::GetInstance().Update(10, 10);

		// all goals completed
		REQUIRE(g1->IsCompleted());
		REQUIRE(g2->IsCompleted());
		REQUIRE(g3->IsFailed());
		REQUIRE(g4->IsInactive());
		REQUIRE(g5->IsInactive());
		REQUIRE(composite->IsFailed());
	}

}


