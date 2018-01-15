#pragma once

#include "TransformMath.h"
#include "Component.h"

namespace Cog {

	class Behavior;
	class Scene;
	class Msg;
	class Tween;

	class SceneSwitchInfo {
	public:
		SceneSwitchInfo(bool finishedOld) : finishedOld(finishedOld) {
		}

		// if true, the old scene will be finished
		bool finishedOld;
	};

	/**
	* Context of the tween action
	*/
	class TweenContext {
	public:
		// scene to tween to
		Scene* to;
		// tween direction
		stenum dir;
		// if true, scene is switched automatically
		bool readyToGo;
		// if true, the old scene will be finished
		bool finishOld;
	};

	/**
	* Manager that switches two scenes with tweening animation
	*/
	class SceneSwitchManager : public Component {
	public:
		TransformMath math;
		// scene to switch from
		Scene* from;
		// scene to switch to
		Scene* to;
		// indicator whether the manager is waiting for a current tween
		bool waitingForTween = false;
		// queue of waiting tweens
		queue<TweenContext> waitingTweens;
		// inidicator whether old scene should be finished
		bool finishOld = false;

		void OnInit();

		void OnMessage(Msg& msg);

		/**
		* Pushes a scene switch action to the stack
		* @param from scene to switch from
		* @param to scene to switch to
		* @param tweenDir tween direction
		* @param autoSwitch if true, scenes are switched automatically
		* @param finishOld if true, the old scene will be finished
		*/
		void PushSceneSwitch(Scene* from, Scene* to, stenum tweenDir, bool autoSwitch, bool finishOld = false);

		/**
		* Pops scene switch action from the stack
		* @return true if there is a switch to pop
		*/
		bool PopSceneSwitch();

		/**
		* Switches between two scenes
		* @param from scene to switch from
		* @param to scene to switch to
		* @param tweenDir tween direction
		* @param finishOld if true, the old scene will be finished
		*/
		void SwitchToScene(Scene* from, Scene* to, stenum tweenDir, bool finishOld = false);

		virtual void Update(const uint64 delta, const uint64 absolute) {
		}

	private:

		/**
		* Executes scene switching
		* @param from scene to switch from
		* @param to scene to switch to
		* @param tweenDir tween direction
		*/
		void ExecuteSwitch(Scene* from, Scene* to, stenum tweenDir, bool finishOld);

		/**
		* Stops the previous scene and notifies behaviors about it
		*/
		void StopPreviousSceneAndNotify();

		/**
		* Checks if there is a scene switch in the stack
		* If so, a waiting scene is switched
		*/
		void CheckWaitingTweens();
	};

}// namespace