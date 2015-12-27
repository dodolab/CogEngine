#pragma once

#include "Behavior.h"
#include "Msg.h"
#include "Node.h"
#include "Tween.h"
#include "TransformMath.h"

namespace Cog {

	class TweenContext {
	public:
		Node* from;
		Node* to;
		TweenDirection dir;
	};

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

		stack<TweenContext> waitingTweens = stack<TweenContext>();

		void Init() {
			RegisterGlobalListening(ACT_TWEEN_ENDED);
		}


		void OnMessage(Msg& msg) {
			if (msg.GetAction() == ACT_TWEEN_ENDED && waitingForTween) {

				// change zindex back to original value
				to->GetTransform().localPos.z = 1;
				to->SetRunningMode(RUNNING);
				from->SetRunningMode(DISABLED);
				waitingForTween = false;

				SendMessageNoBubbling(ACT_SCENE_SWITCHED, 0, nullptr, to);

				// switch to waiting scenes
				if (!waitingTweens.empty()) {
					TweenContext ctx = waitingTweens.top();
					waitingTweens.pop();
					this->SwitchToScene(ctx.from, ctx.to, ctx.dir);
				}
			}
		}

		/**
		* Switches to another scene without tweening
		* @param scene scene to switch to
		*/
		void SwitchToScene(Node* from, Node* to) {
			MLOGDEBUG("SceneManager", "Switching from %s to %s immediately", from->GetTag().c_str(), to->GetTag().c_str());

			// hide scene immediately
			from->SetRunningMode(DISABLED);

			this->from = from;
			this->to = to;

			to->SetRunningMode(RUNNING);
			to->GetTransform().localPos.x = 0;
			to->GetTransform().localPos.y = 0;

			SendMessageNoBubbling(ACT_SCENE_SWITCHED, 0, nullptr, to);
		}

		/**
		* Switches to another scene with tweening
		* @param scene scene to switch to
		* @param tweenDir tween direction
		*/
		void SwitchToScene(Node* from, Node* to, TweenDirection tweenDir) {
			MLOGDEBUG("SceneManager", "Switching from %s to %s", from->GetTag().c_str(), to->GetTag().c_str());

			if (waitingForTween) {
				MLOGDEBUG("SceneManager", "Adding tween context onto the stack");
				TweenContext context = TweenContext();
				context.from = from;
				context.to = to;
				context.dir = tweenDir;
				waitingTweens.push(context);
			}
			else {
				this->from = from;
				this->to = to;

				// take the other node away before the animation starts
				to->GetTransform().localPos.x = to->GetTransform().absPos.x = 100000000;
				from->SetRunningMode(PAUSED_ALL);

				auto slide = new SlideTween(tweenDir, from, to, 1);

				auto fadeFunc = [](float x) {
					return x * x * (3.0f - 2.0f * x);
				};

				slide->SetFadeFunction(fadeFunc);

				to->AddBehavior(slide);
				// node will be set visible at first update
				to->SetRunningMode(INVISIBLE);

				// wait for tween
				waitingForTween = true;
			}
		}
	};

}// namespace