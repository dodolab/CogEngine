#pragma once

#include "GBehavior.h"
#include "GMsg.h"
#include "GNode.h"
#include "BeTween.h"

/**x
* Behavior scene manager
*/
class BeSceneManager : public GBehavior{
public:
	// scene to switch to
	GNode* switchedScene;
	// actually displayed scene
	GNode* actualScene;
	// indicator, if this behavior is waiting until tween ends
	bool waitingForTween;

	/**
	* Creates a new behavior for scene managing
	*/
	BeSceneManager() : GBehavior(ElemType::MODEL){
		actualScene = nullptr;
		waitingForTween = false;
	}

	void Init(){
		RegisterListening(Actions::TWEEN_ENDED);
	}


	void OnMessage(GMsg& msg){
		if (msg.GetAction() == Actions::TWEEN_ENDED && waitingForTween){

			// change zindex back to original value
			actualScene->GetTransform().localPos.z = 1;
			switchedScene->SetRunningMode(DISABLED);
			waitingForTween = false;
			
			SendMessageNoBubbling(Actions::SCENE_SWITCHED, 0, nullptr, actualScene);
		}
	}
	
	/**
	* Switches to another scene without tweening
	* @param scene scene to switch to
	*/
	void SwitchToScene(GNode* scene){

		if (actualScene != nullptr){
			// hide scene immediately
			actualScene->SetRunningMode(DISABLED);
		}

		// translate new scene
		actualScene = scene;
		scene->SetRunningMode(RUNNING);
		scene->GetTransform().localPos.x = COGGetScreenWidth() / 2;
		scene->GetTransform().localPos.y = COGGetScreenHeight() / 2;

		SendMessageNoBubbling(Actions::SCENE_SWITCHED, 0, nullptr, actualScene);
	}

	/**
	* Switches to another scene with tweening
	* @param scene scene to switch to
	* @param tweenDir tween direction
	*/
	void SwitchToScene(GNode* scene, TweenDirection tweenDir){

		// don't switch between the same scene
		if (actualScene == scene) return;

		actualScene->SetRunningMode(RUNNING);
		scene->SetRunningMode(RUNNING);
		owner->AddBehavior(new BeSlideTween(tweenDir, scene, actualScene,2));
		
		// add new scene to the front
 		scene->GetTransform().localPos.z = 0;
		// move the scene away; tween behavior will set its position itself
		scene->GetTransform().localPos.x = COGGetScreenWidth()*10;
		// update absolute positions because of drawing thread
		scene->UpdateTransform(true);

		actualScene->GetTransform().localPos.z = 1;
		
		switchedScene = actualScene;
		actualScene = scene;

		// wait for tween
		waitingForTween = true;
	}

	void Update(const uint64 delta, const uint64 absolute){

	}

};