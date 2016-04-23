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
			COGLOGDEBUG("SceneSwitch", "Scene %s switched", to->GetName().c_str());

			// resume actual scene
			to->GetSceneNode()->SetRunningMode(RUNNING);
		
			waitingForTween = false;

			StopPreviousSceneAndNotify();
			CheckWaitingTweens();
		}
	}

	void SceneSwitchManager::PushSceneSwitch(Scene* from, Scene* to, TweenDirection tweenDir, bool autoSwitch) {
		TweenContext context = TweenContext();
		context.to = to;
		context.dir = tweenDir;
		context.readyToGo = autoSwitch;
		waitingTweens.push(context);
	}

	bool SceneSwitchManager::PopSceneSwitch() {
		if (waitingTweens.empty()) return false;

		// set waiting scene as ready for switch
		waitingTweens.front().readyToGo = true;

		// if there is at least one scene which just switches,
		// skip this step; readyToGo was set to true so as soon as the previous scene is
		// finished, manager will automatically tween to the next scene
		if (!waitingForTween) {
			TweenContext ctx = waitingTweens.front();
			waitingTweens.pop();
			ctx.readyToGo = true;

			// switch from the last "to" scene
			this->ExecuteSwitch(to, ctx.to, ctx.dir);
		}

		return true;
	}

	void SceneSwitchManager::SwitchToScene(Scene* from, Scene* to, TweenDirection tweenDir) {
		COGLOGDEBUG("SceneSwitch", "Switching from %s to %s", from->GetName().c_str(), to->GetName().c_str());

		if (waitingForTween) {
			COGLOGDEBUG("SceneSwitch", "-another switch in progress; pushing context");
			PushSceneSwitch(from, to, tweenDir, true);
		}
		else if (!waitingTweens.empty()) {
			COGLOGDEBUG("SceneSwitch", "-another switch on the stack; pushing context");
			PushSceneSwitch(from, to, tweenDir, true);
		}
		else {
			ExecuteSwitch(from, to, tweenDir);
		}
	}

	void SceneSwitchManager::ExecuteSwitch(Scene* from, Scene* to, TweenDirection tweenDir) {
		this->from = from;
		this->to = to;

		// pause the scene which moves back
		from->GetSceneNode()->SetRunningMode(PAUSED_ALL);

		// initialize new scene
		to->Init();

		bool isDialog = to->GetSceneType() == SceneType::DIALOG;

		if (tweenDir == TweenDirection::NONE) {
			// switch immediately
			
			to->GetSceneNode()->SetRunningMode(RUNNING);
			// set viewport to the topleft corner
			to->GetViewPortOffset().x = 0;
			to->GetViewPortOffset().y = 0;

			StopPreviousSceneAndNotify();

			CheckWaitingTweens();
		}
		else {
			if (isDialog) {
				// display dialog in front of the current scene
				auto slide = new OverrideTween(tweenDir, to->GetSceneNode(), 1);
				slide->SetFadeFunction(EasingFunc::cosineInOut);
				slide->SetRemoveWhenFinish(true);
				to->GetSceneNode()->AddBehavior(slide);
			}
			else {
				auto slide = new SlideTween(tweenDir, from->GetSceneNode(), to->GetSceneNode(), 1);
				slide->SetFadeFunction(EasingFunc::cosineInOut);
				slide->SetRemoveWhenFinish(true);
				to->GetSceneNode()->AddBehavior(slide);
			}
			
			// node will be set as visible at first update of
			// the tween behavior (it prevents flashing)
			to->GetSceneNode()->SetRunningMode(INVISIBLE);

			// wait for tween
			waitingForTween = true;
		}
	}

	void SceneSwitchManager::StopPreviousSceneAndNotify() {
		// set the moved scene as running just for a while because a message has to be sent
		from->GetSceneNode()->SetRunningMode(RUNNING);
		// send message to both scenes
		auto msg = Msg(ACT_SCENE_SWITCHED, MsgObjectType::COMPONENT, this->id, MsgObjectType::SUBSCRIBERS, to->GetSceneNode(), spt<MsgPayload>());
		to->SendMessage(msg);
		from->SendMessage(msg);

		if (to->GetSceneType() != SceneType::DIALOG) {
			// disable and dispose moved scene
			from->GetSceneNode()->SetRunningMode(DISABLED);
			from->Dispose();
		}
		else {
			// for dialogs, just pause the moved scene
			from->GetSceneNode()->SetRunningMode(PAUSED_ALL);
		}
	}

	void SceneSwitchManager::CheckWaitingTweens() {
		if (!waitingTweens.empty()) {
			COGLOGDEBUG("SceneSwitch", "--reading next switch context from the stack");

			TweenContext ctx = waitingTweens.front();
			if (ctx.readyToGo) {
				PopSceneSwitch();
			}
		}
	}

} // namespace