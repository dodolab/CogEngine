#pragma once

#include "ofMain.h"
#include "ofxCogEngine.h"

#include "catch.hpp"
using namespace Cog;

#include "GoalTestAssets.h"



TEST_CASE("GoalTest")
{
	SECTION("All complete test")
	{
		// 1. init engine
		ofxCogEngine::GetInstance().SetFps(20);
		ofxCogEngine::GetInstance().Init();
		// 2. create scene
		Scene* scene = new Scene("main", false);
		ofxCogEngine::GetInstance().stage->AddScene(scene, true);
		// 3. define nodes and behaviors
		Node* node = new Node("node");
		
		Goal* g1 = new DummyGoal(true);
		Goal* g2 = new DummyGoal(true);
		Goal* g3 = new DummyGoal(true);
		Goal* g4 = new DummyGoal(true);
		Goal* g5 = new DummyGoal(true);
		GoalComposite* composite = new GoalComposite(StrId(), GoalCompositeType::SERIALIZER);
		composite->AddSubgoal(g1);
		composite->AddSubgoal(g2);
		composite->AddSubgoal(g3);
		composite->AddSubgoal(g4);
		composite->AddSubgoal(g5);

		node->AddBehavior(composite);

		// 4. add main node to the scene
		scene->GetSceneNode()->AddChild(node);
		// 5. submit changes
		ofxCogEngine::GetInstance().stage->GetRootObject()->SubmitChanges(true);
		
		// update once
		ofxCogEngine::GetInstance().Update(10, 10);
		
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
		ofxCogEngine::GetInstance().SetFps(20);
		ofxCogEngine::GetInstance().Init();
		// 2. create scene
		Scene* scene = new Scene("main", false);
		ofxCogEngine::GetInstance().stage->AddScene(scene, true);
		// 3. define nodes and behaviors
		Node* node = new Node("node");

		Goal* g1 = new DummyGoal(true);
		Goal* g2 = new DummyGoal(true);
		Goal* g3 = new DummyGoal(false);
		Goal* g4 = new DummyGoal(true);
		Goal* g5 = new DummyGoal(false);
		GoalComposite* composite = new GoalComposite(StrId(), GoalCompositeType::SEQUENCER);
		composite->AddSubgoal(g1);
		composite->AddSubgoal(g2);
		composite->AddSubgoal(g3);
		composite->AddSubgoal(g4);
		composite->AddSubgoal(g5);

		node->AddBehavior(composite);

		// 4. add main node to the scene
		scene->GetSceneNode()->AddChild(node);
		// 5. submit changes
		ofxCogEngine::GetInstance().stage->GetRootObject()->SubmitChanges(true);

		// update once
		ofxCogEngine::GetInstance().Update(10, 10);

		// all goals completed
		REQUIRE(g1->IsCompleted());
		REQUIRE(g2->IsCompleted());
		REQUIRE(g3->IsFailed());
		REQUIRE(g4->IsInactive());
		REQUIRE(g5->IsInactive());
		REQUIRE(composite->IsFailed());
	}

}


