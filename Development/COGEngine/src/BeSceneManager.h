#pragma once

#include "GBehavior.h"
#include "GMsg.h"
#include "GNode.h"
#include "BeTween.h"

class BeSceneManager : public GBehavior{
public:
	GNode* switchedScene;
	GNode* actualScene;
	bool waitingForTween;

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
			actualScene->GetTransform().LocalPos.z = 1;
			switchedScene->SetRunningMode(INVISIBLE);
			waitingForTween = false;
		}
	}
	
	void SwitchToScene(GNode* scene){
		if (actualScene != nullptr){
			actualScene->SetRunningMode(INVISIBLE);
		}
		actualScene = scene;
		scene->SetRunningMode(RUNNING);
		scene->GetTransform().LocalPos.x = COGGetWidth() / 2;
		scene->GetTransform().LocalPos.y = COGGetHeight() / 2;
	}

	void SwitchToScene(GNode* scene, TweenDirection tweenDir){
		actualScene->SetRunningMode(RUNNING);
		scene->SetRunningMode(RUNNING);
		owner->AddBehavior(new BeSlideTween(tweenDir, scene, actualScene,2));
		scene->GetTransform().LocalPos.z = 0;
		// throw it away
		scene->GetTransform().LocalPos.x = COGGetWidth()*10;

		actualScene->GetTransform().LocalPos.z = 1;
		
		switchedScene = actualScene;
		actualScene = scene;

		waitingForTween = true;
	}

	void Update(const uint64 delta, const uint64 absolute){

	}

};