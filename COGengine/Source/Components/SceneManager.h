#pragma once

#include "TransformMath.h"
#include "Component.h"

namespace Cog {

	class Behavior;
	class Node;
	class Msg;
	class Tween;
	enum class TweenDirection;

	class TweenContext {
	public:
		Node* to;
		TweenDirection dir;
		bool readyToGo; // if true, scene is switched automatically
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

		queue<TweenContext> waitingTweens = queue<TweenContext>();

		void Init();


		void OnMessage(Msg& msg);

		void PushSceneSwitch(Node* from, Node* to, TweenDirection tweenDir, bool autoSwitch);

		bool PopSceneSwitch();

		/**
		* Switches to another scene with tweening
		* @param scene scene to switch to
		* @param tweenDir tween direction
		*/
		void SwitchToScene(Node* from, Node* to, TweenDirection tweenDir);

	private:

		void ExecuteSwitch(Node* from, Node* to, TweenDirection tweenDir);
	};

}// namespace