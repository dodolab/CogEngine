#include "SceneSwitchManager.h"
#include "Behavior.h"
#include "Msg.h"
#include "Node.h"
#include "Scene.h"
#include "Tween.h"


namespace Cog {

	void SceneSwitchManager::Init() {
		RegisterGlobalListening(ACT_TWEEN_ENDED);
	}


	void SceneSwitchManager::OnMessage(Msg& msg) {
		if (msg.GetAction() == ACT_TWEEN_ENDED && waitingForTween) {
			MLOGDEBUG("SceneSwitchManager", "Scene %s switched", to->GetTag().c_str());

			to->SetRunningMode(RUNNING);
			from->SetRunningMode(DISABLED);
			from->GetScene()->Dispose();

			waitingForTween = false;

			SendMessageNoBubbling(ACT_SCENE_SWITCHED, 0, nullptr, to);
			CheckWaitingTweens();
		}
	}

	void SceneSwitchManager::PushSceneSwitch(Node* from, Node* to, TweenDirection tweenDir, bool autoSwitch) {
		TweenContext context = TweenContext();
		context.to = to;
		context.dir = tweenDir;
		context.readyToGo = autoSwitch;
		waitingTweens.push(context);
	}

	bool SceneSwitchManager::PopSceneSwitch() {
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
	void SceneSwitchManager::SwitchToScene(Node* from, Node* to, TweenDirection tweenDir) {
		MLOGDEBUG("SceneSwitchManager", "Switching from %s to %s", from->GetTag().c_str(), to->GetTag().c_str());

		if (waitingForTween) {
			MLOGDEBUG("SceneSwitchManager", "-another switch in progress; pushing context");
			PushSceneSwitch(from, to, tweenDir, true);
		}
		else if (!waitingTweens.empty()) {
			MLOGDEBUG("SceneSwitchManager", "-another switches on the stack; pushing context");
			PushSceneSwitch(from, to, tweenDir, true);
		}
		else {
			ExecuteSwitch(from, to, tweenDir);
		}
	}

	void SceneSwitchManager::ExecuteSwitch(Node* from, Node* to, TweenDirection tweenDir) {
		this->from = from;
		this->to = to;

		from->SetRunningMode(PAUSED_ALL);

		to->GetScene()->Init();

		if (tweenDir == TweenDirection::NONE) {
			// switch immediately
			from->SetRunningMode(DISABLED);
			to->SetRunningMode(RUNNING);
			// set viewport to proper position
			to->GetScene()->GetViewPortOffset().x = 0;
			to->GetScene()->GetViewPortOffset().y = 0;
			SendMessageNoBubbling(ACT_SCENE_SWITCHED, 0, nullptr, to);

			CheckWaitingTweens();
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

	void SceneSwitchManager::CheckWaitingTweens() {
		// switch to waiting scenes
		if (!waitingTweens.empty()) {
			MLOGDEBUG("SceneSwitchManager", "--reading next switch context from the stack");

			TweenContext ctx = waitingTweens.front();
			if (ctx.readyToGo) {
				PopSceneSwitch();
			}
		}
	}

} // namespace