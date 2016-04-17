#include "SceneSwitchManager.h"
#include "Behavior.h"
#include "Msg.h"
#include "Node.h"
#include "Scene.h"
#include "Tween.h"
#include "EasingFunc.h"

namespace Cog {

	void SceneSwitchManager::OnInit() {
		GlobalSubscribeForMessages(ACT_TWEEN_ENDED);
	}


	void SceneSwitchManager::OnMessage(Msg& msg) {
		if (msg.HasAction(ACT_TWEEN_ENDED) && waitingForTween) {
			COGLOGDEBUG("SceneSwitchManager", "Scene %s switched", to->GetTag().c_str());

			to->SetRunningMode(RUNNING);
		
			waitingForTween = false;

			StopSceneAndNotify();

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
		COGLOGDEBUG("SceneSwitchManager", "Switching from %s to %s", from->GetTag().c_str(), to->GetTag().c_str());

		if (waitingForTween) {
			COGLOGDEBUG("SceneSwitchManager", "-another switch in progress; pushing context");
			PushSceneSwitch(from, to, tweenDir, true);
		}
		else if (!waitingTweens.empty()) {
			COGLOGDEBUG("SceneSwitchManager", "-another switches on the stack; pushing context");
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

		bool isDialog = to->GetScene()->GetSceneType() == SceneType::DIALOG;

		if (tweenDir == TweenDirection::NONE) {
			// switch immediately
			
			to->SetRunningMode(RUNNING);
			// set viewport to proper position
			to->GetScene()->GetViewPortOffset().x = 0;
			to->GetScene()->GetViewPortOffset().y = 0;

			StopSceneAndNotify();


			CheckWaitingTweens();
		}
		else {
			if (isDialog) {
				auto slide = new OverrideTween(tweenDir, to, 1);
				slide->SetFadeFunction(EasingFunc::cosineInOut);
				to->AddBehavior(slide);
			}
			else {
				auto slide = new SlideTween(tweenDir, from, to, 1);
				slide->SetFadeFunction(EasingFunc::cosineInOut);
				to->AddBehavior(slide);
			}
			
			// node will be set visible at first update
			to->SetRunningMode(INVISIBLE);

			// wait for tween
			waitingForTween = true;
		}
	}

	void SceneSwitchManager::StopSceneAndNotify() {
		// run the first scene until message is sent
		from->SetRunningMode(RUNNING);
		auto msg = Msg(ACT_SCENE_SWITCHED, MsgObjectType::COMPONENT, this->id, MsgObjectType::SUBSCRIBERS, to, spt<MsgEvent>());
		to->GetScene()->SendMessage(msg);
		from->GetScene()->SendMessage(msg);

		if (to->GetScene()->GetSceneType() != SceneType::DIALOG) {
			from->SetRunningMode(DISABLED);
			from->GetScene()->Dispose();
		}
		else {
			from->SetRunningMode(PAUSED_ALL);
		}
	}

	void SceneSwitchManager::CheckWaitingTweens() {
		// switch to waiting scenes
		if (!waitingTweens.empty()) {
			COGLOGDEBUG("SceneSwitchManager", "--reading next switch context from the stack");

			TweenContext ctx = waitingTweens.front();
			if (ctx.readyToGo) {
				PopSceneSwitch();
			}
		}
	}

} // namespace