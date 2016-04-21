#pragma once

#include "Behavior.h"

namespace Cog {

	/** Tweening direction */
	enum class TweenDirection {
		NONE,			/** direct tween */ 
		LEFT,			/** move to the left*/
		RIGHT,			/** move to the right */
		UP,				/** move up*/
		DOWN			/** move down */
	};

	/**
	* Behavior for scene tweening
	*/
	class Tween : public Behavior {
		
	protected:
		// scene node that will come up
		Node* to= nullptr;
		// scene node that will be moved off
		Node* from = nullptr;
		// tweening speed (1 is for 1s, 2 for 0.5s and so on)
		float speed = 0;
		// actual position of the tweened node
		float actual = 0;
		// fade function
		FadeFunction fadeFunction = nullptr;

	public:

		Tween() {

		}

		/**
		* Creates a new behavior for scene tweening
		* @param from scene node that will be moved off
		* @param to scene node that will come up
		* @param speed tweening speed (1 is for 1s, 2 for 0.5s and so on)
		*/
		Tween(Node* from, Node* to, float speed) 
			: from(from), to(to), speed(speed){

		}
	};

	/**
	* Behavior that moves one scene in front of the current,
	* keeping both scenes visible (usable for dialogs)
	*/
	class OverrideTween : public Tween {
	private:
		// tween direction
		TweenDirection direction;

	public:

		/**
		* Creates a new behavior for override tweening
		* @param direction tween direction
		* @param to scene node that will come up
		* @param speed tweening speed (1 is for 1s, 2 for 0.5s and so on)
		*/
		OverrideTween(TweenDirection direction, Node* to, float speed) 
			: Tween(nullptr, to, speed), direction(direction) {

		}

		void SetFadeFunction(FadeFunction func) {
			this->fadeFunction = func;
		}


		virtual void Update(const uint64 delta, const uint64 absolute);
	};

	/**
	* Behavior that moves the current scene off and slide the new
	*/
	class SlideTween : public Tween {
	private:
		// tween direction
		TweenDirection direction;

	public:
		SlideTween() {

		}

		/**
		* Creates a new behavior for slide tweening
		* @param direction tween direction
		* @param from scene node that will be moved off
		* @param to scene that will come up
		* @param speed tweening speed
		*/
		SlideTween(TweenDirection direction, Node* from, Node* to, float speed) 
			: Tween(from, to, speed), direction(direction) {

		}


		void SetFadeFunction(FadeFunction func) {
			this->fadeFunction = func;
		}


		virtual void Update(const uint64 delta, const uint64 absolute);
	};

}// namespace