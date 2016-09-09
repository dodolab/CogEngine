#pragma once

#include "ofxAreBehavior.h"
#include "ofxAreMsg.h"
#include "ofxAreNode.h"
#include "ofxArbTween.h"

/**x
* Behavior scene manager
*/
class ofxArbSceneManager : public ofxAreBehavior{
public:
	// scene to switch to
	ofxAreNode* switchedScene;
	// actually displayed scene
	ofxAreNode* actualScene;
	// indicator, if this behavior is waiting until tween ends
	bool waitingForTween;

	/**
	* Creates a new behavior for scene managing
	*/
	ofxArbSceneManager() {
		actualScene = nullptr;
		waitingForTween = false;
	}

	void Init(){
		RegisterListening(Actions::TWEEN_ENDED);
	}


	void OnMessage(ofxAreMsg& msg){
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
	void SwitchToScene(ofxAreNode* scene){

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
	void SwitchToScene(ofxAreNode* scene, TweenDirection tweenDir){

		// don't switch ofxArbTween the same scene
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