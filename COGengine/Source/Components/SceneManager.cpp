#include "SceneManager.h"
#include "Behavior.h"
#include "Msg.h"
#include "Node.h"
#include "Scene.h"
#include "Tween.h"


namespace Cog {

	void SceneManager::Init() {
		RegisterGlobalListening(ACT_TWEEN_ENDED);
	}


	void SceneManager::OnMessage(Msg& msg) {
		if (msg.GetAction() == ACT_TWEEN_ENDED && waitingForTween) {
			MLOGDEBUG("SceneManager", "Scene %s switched", to->GetTag().c_str());

			to->SetRunningMode(RUNNING);

			from->SetRunningMode(DISABLED);
			waitingForTween = false;

			SendMessageNoBubbling(ACT_SCENE_SWITCHED, 0, nullptr, to);

			// switch to waiting scenes
			if (!waitingTweens.empty()) {
				MLOGDEBUG("SceneManager", "--reading next switch context from the stack");

				TweenContext ctx = waitingTweens.front();
				if (ctx.readyToGo) {
					PopSceneSwitch();
				}
			}
		}
	}

	void SceneManager::PushSceneSwitch(Node* from, Node* to, TweenDirection tweenDir, bool autoSwitch) {
		TweenContext context = TweenContext();
		context.to = to;
		context.dir = tweenDir;
		context.readyToGo = autoSwitch;
		waitingTweens.push(context);
	}

	bool SceneManager::PopSceneSwitch() {
		if (waitingTweens.empty()) return false;


		waitingTweens.front().readyToGo = true;

		// if there is at least one scene that is being tweened,
		// skip this step; readyToGot was set to true so when the previous scene is
		// finished, manager will automatically tween to the next scene
		if (!waitingForTween) {
			TweenContext ctx = waitingTweens.front();
			waitingTweens.pop();
			ctx.readyToGo = true;

			// switch from the last "to" node
			this->ExecuteSwitch(to, ctx.to, ctx.dir);
		}

		return true;
	}

	/**
	* Switches to another scene with tweening
	* @param scene scene to switch to
	* @param tweenDir tween direction
	*/
	void SceneManager::SwitchToScene(Node* from, Node* to, TweenDirection tweenDir) {
		MLOGDEBUG("SceneManager", "Switching from %s to %s", from->GetTag().c_str(), to->GetTag().c_str());

		if (waitingForTween) {
			MLOGDEBUG("SceneManager", "-another switch in progress; pushing context");
			PushSceneSwitch(from, to, tweenDir, true);
		}
		else if (!waitingTweens.empty()) {
			MLOGDEBUG("SceneManager", "-another switches on the stack; pushing context");
			PushSceneSwitch(from, to, tweenDir, true);
		}
		else {
			ExecuteSwitch(from, to, tweenDir);
		}
	}

	void SceneManager::ExecuteSwitch(Node* from, Node* to, TweenDirection tweenDir) {
		this->from = from;
		this->to = to;

		from->SetRunningMode(PAUSED_ALL);

		if (tweenDir == TweenDirection::NONE) {
			// switch immediately
			from->SetRunningMode(DISABLED);
			to->SetRunningMode(RUNNING);
			// set viewport to proper position
			to->GetScene()->GetViewPortOffset().x = 0;
			to->GetScene()->GetViewPortOffset().y = 0;
			SendMessageNoBubbling(ACT_SCENE_SWITCHED, 0, nullptr, to);
		}
		else {
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

} // namespace