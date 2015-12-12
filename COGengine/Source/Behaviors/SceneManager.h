#pragma once

#include "Behavior.h"
#include "Msg.h"
#include "Node.h"
#include "Tween.h"

namespace Cog {

	/**x
	* Behavior scene manager
	*/
	class SceneManager : public Behavior {
	public:
		// scene to switch to
		Node* switchedScene;
		// actually displayed scene
		Node* actualScene;
		// indicator, if this behavior is waiting until tween ends
		bool waitingForTween;

		/**
		* Creates a new behavior for scene managing
		*/
		SceneManager() {
			actualScene = nullptr;
			waitingForTween = false;
		}

		void Init() {
			RegisterListening(ACT_TWEEN_ENDED);
		}


		void OnMessage(Msg& msg) {
			if (msg.GetAction() == ACT_TWEEN_ENDED && waitingForTween) {

				// change zindex back to original value
				actualScene->GetTransform().localPos.z = 1;
				switchedScene->SetRunningMode(DISABLED);
				waitingForTween = false;

				SendMessageNoBubbling(ACT_SCENE_SWITCHED, 0, nullptr, actualScene);
			}
		}

		/**
		* Switches to another scene without tweening
		* @param scene scene to switch to
		*/
		void SwitchToScene(Node* scene) {

			if (actualScene != nullptr) {
				// hide scene immediately
				actualScene->SetRunningMode(DISABLED);
			}

			// translate new scene
			actualScene = scene;
			scene->SetRunningMode(RUNNING);
			scene->GetTransform().localPos.x = CogGetScreenWidth() / 2;
			scene->GetTransform().localPos.y = CogGetScreenHeight() / 2;

			SendMessageNoBubbling(ACT_SCENE_SWITCHED, 0, nullptr, actualScene);
		}

		/**
		* Switches to another scene with tweening
		* @param scene scene to switch to
		* @param tweenDir tween direction
		*/
		void SwitchToScene(Node* scene, TweenDirection tweenDir) {

			// don't switch Tween the same scene
			if (actualScene == scene) return;

			actualScene->SetRunningMode(RUNNING);
			scene->SetRunningMode(RUNNING);
			owner->AddBehavior(new SlideTween(tweenDir, scene, actualScene, 2));

			// add new scene to the front
			scene->GetTransform().localPos.z = 0;
			// move the scene away; tween behavior will set its position itself
			scene->GetTransform().localPos.x = CogGetScreenWidth() * 10;
			// update absolute positions because of drawing thread
			scene->UpdateTransform(true);

			actualScene->GetTransform().localPos.z = 1;

			switchedScene = actualScene;
			actualScene = scene;

			// wait for tween
			waitingForTween = true;
		}

		void Update(const uint64 delta, const uint64 absolute) {

		}

	};

}// namespace