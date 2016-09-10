#pragma once

#include "Behavior.h"
#include "Msg.h"
#include "Node.h"
#include "Tween.h"
#include "TransformMath.h"

namespace Cog {

	/**x
	* Scene manager component
	*/
	class SceneManager : public Component {
		OBJECT(SceneManager)
	public:
		TransformMath math = TransformMath();
		// scene to switch from
		Node* from;
		// scene to switch to
		Node* to;
		// indicator, if this behavior is waiting until tween ends
		bool waitingForTween = false;


		void Init() {
			RegisterListening(ACT_TWEEN_ENDED);
		}


		void OnMessage(Msg& msg) {
			if (msg.GetAction() == ACT_TWEEN_ENDED && waitingForTween) {

				// change zindex back to original value
				to->GetTransform().localPos.z = 1;
				from->SetRunningMode(DISABLED);
				waitingForTween = false;

				SendMessageNoBubbling(ACT_SCENE_SWITCHED, 0, nullptr, to);
			}
		}

		/**
		* Switches to another scene without tweening
		* @param scene scene to switch to
		*/
		void SwitchToScene(Node* from, Node* to) {

			// hide scene immediately
			from->SetRunningMode(DISABLED);

			this->from = from;
			this->to = to;

			to->SetRunningMode(RUNNING);
			to->GetTransform().localPos.x = CogGetScreenWidth() / 2;
			to->GetTransform().localPos.y = CogGetScreenHeight() / 2;

			SendMessageNoBubbling(ACT_SCENE_SWITCHED, 0, nullptr, to);
		}

		/**
		* Switches to another scene with tweening
		* @param scene scene to switch to
		* @param tweenDir tween direction
		*/
		void SwitchToScene(Node* from, Node* to, TweenDirection tweenDir) {
			this->from = from;
			this->to = to;

			to->SetRunningMode(RUNNING);
			from->SetRunningMode(RUNNING);
			auto slide = new SlideTween(tweenDir, from, to, 1);

			auto fadeFunc = [](float x) {
				return x * x * (3.0f - 2.0f * x);
			};

			slide->SetFadeFunction(fadeFunc);

			to->AddBehavior(slide);

			// wait for tween
			waitingForTween = true;
		}

	};

}// namespace