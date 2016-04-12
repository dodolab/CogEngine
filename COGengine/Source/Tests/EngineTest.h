#pragma once

#include "ofMain.h"
#include "CogEngine.h"
#include "ofxCogMain.h"
#include "catch.hpp"
using namespace Cog;

#include "EngineTestAssets.h"

TEST_CASE("COG Engine test")
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

	SECTION("Behavior by its type")
	{
		// 1. init engine
		CogEngine::GetInstance().SetFps(20);
		CogEngine::GetInstance().Init();
		// 2. create scene
		Scene* scene = new Scene("main", false);
		CogEngine::GetInstance().stage->AddScene(scene, true);
		// 3. define nodes and behaviors

		Node* node = new Node("node");
		MaxCountBehavior* unique = new MaxCountBehavior(1);
		node->AddBehavior(unique);


		// 4. add main node to the scene
		scene->GetSceneNode()->AddChild(node);
		// 5. submit changes
		CogEngine::GetInstance().stage->GetRootObject()->SubmitChanges(true);

		// simulate update
		CogEngine::GetInstance().Update(16, 16);

		// check if it works
		auto beh =node->GetBehavior<MaxCountBehavior>();
		bool allright = beh != nullptr && beh->maxCount == 1;
		REQUIRE(allright);
	}

	SECTION("Behavior can't be stored")
	{
		// 1. init engine
		CogEngine::GetInstance().SetFps(20);
		CogEngine::GetInstance().Init();
		// 2. create scene
		Scene* scene = new Scene("main", false);
		CogEngine::GetInstance().stage->AddScene(scene, true);
		// 3. define nodes and behaviors

		Node* node = new Node("node");
		MaxCountBehavior* unique = new MaxCountBehavior(1);
		node->AddBehavior(unique);
		node->AddBehavior(unique);

		// 4. add main node to the scene
		scene->GetSceneNode()->AddChild(node);
		CogEngine::GetInstance().stage->GetRootObject()->SubmitChanges(true);

		// only one behavior stored, because max number 1 is allowed
		REQUIRE(node->GetBehaviors().size() == 1);
	}
}
