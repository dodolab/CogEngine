#pragma once

#include "TransformMath.h"
#include "Component.h"

namespace Cog {

	class Behavior;
	class Scene;
	class Msg;
	class Tween;
	enum class TweenDirection;

	/**
	* Context of the tween action
	*/
	class TweenContext {
	public:
		// scene to tween to
		Scene* to;
		// tween direction
		TweenDirection dir;
		// if true, scene is switched automatically
		bool readyToGo; 
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

		void OnInit();

		void OnMessage(Msg& msg);

		/**
		* Pushes a scene switch action to the stack
		* @param from scene to switch from
		* @param to scene to switch to
		* @param tweenDir tween direction
		* @param autoSwitch if true, scenes are switched automatically
		*/
		void PushSceneSwitch(Scene* from, Scene* to, TweenDirection tweenDir, bool autoSwitch);

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
		*/
		void SwitchToScene(Scene* from, Scene* to, TweenDirection tweenDir);

		virtual void Update(const uint64 delta, const uint64 absolute) {
		}

	private:

		/**
		* Executes scene switching
		* @param from scene to switch from
		* @param to scene to switch to
		* @param tweenDir tween direction
		*/
		void ExecuteSwitch(Scene* from, Scene* to, TweenDirection tweenDir);

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