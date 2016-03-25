#pragma once

#include "Behavior.h"

namespace Cog {

	/*! Tween direction Tween scenes */
	enum class TweenDirection {
		NONE, LEFT, RIGHT, UP, DOWN
	};

	/**x
	* Behavior for scene tweening
	*/
	class Tween : public Behavior {
		
	protected:
		// scene that will be tweened in
		Node* to= nullptr;
		// scene that will be tweened out
		Node* from = nullptr;
		// tweening speed (in display width per second)
		float speed = 0;
		// actual tween position
		float actual = 0;
		// fade function
		FadeFunction fadeFunction = nullptr;

	public:

		Tween() {

		}

		/**
		* Creates a new behavior for tweening animation Tween scenes
		* @param from scene that will be tweened out
		* @param to scene that will be tweened to
		* @param speed tweening speed
		*/
		Tween(Node* from, Node* to, float speed) : from(from), to(to), speed(speed){

		}
	};

	/**
	* Behavior for tweening with override (usable for dialogs)
	*/
	class OverrideTween : public Tween {
		OBJECT_PROTOTYPE(OverrideTween)
	private:
		// tween direction
		TweenDirection direction;

	public:

		/**
		* Creates a new behavior for slide tweening
		* @param direction tween direction
		* @param to scene that will be tweened inm
		* @param speed tweening speed
		*/
		OverrideTween(TweenDirection direction, Node* to, float speed) : Tween(nullptr, to, speed), direction(direction) {

		}


		void SetFadeFunction(FadeFunction func) {
			this->fadeFunction = func;
		}


		virtual void Update(const uint64 delta, const uint64 absolute);
	};

	/**
	* Behavior for slide tweening
	*/
	class SlideTween : public Tween {
		OBJECT_PROTOTYPE(SlideTween)
	private:
		// tween direction
		TweenDirection direction;

	public:

		/**
		* Creates a new behavior for slide tweening
		* @param direction tween direction
		* @param from scene that will be tweened out
		* @param to scene that will be tweened inm
		* @param speed tweening speed
		*/
		SlideTween(TweenDirection direction, Node* from, Node* to, float speed) : Tween(from, to, speed), direction(direction) {

		}


		void SetFadeFunction(FadeFunction func) {
			this->fadeFunction = func;
		}


		virtual void Update(const uint64 delta, const uint64 absolute);
	};

}// namespace