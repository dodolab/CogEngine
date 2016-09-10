#pragma once

#include "Behavior.h"

namespace Cog {

	/*! Tween direction Tween scenes */
	enum class TweenDirection {
		LEFT, RIGHT, UP, DOWN
	};

	/**x
	* Behavior for scene tweening
	*/
	class Tween : public Behavior {
		
	protected:
		// scene that will be tweened out
		Node* tweenOut;
		// scene that will be tweened in
		Node* tweenIn;
		// tweening speed (in display width per second)
		float speed;
		// actual tween position
		float actual;

	public:

		Tween() {

		}

		/**
		* Creates a new behavior for tweening animation Tween scenes
		* @param tweenIn scene that will be tweened in
		* @param tweenOut scene that will be tweened out
		* @param speed tweening speed
		*/
		Tween(Node* tweenIn, Node* tweenOut, float speed) : tweenIn(tweenIn), tweenOut(tweenOut), speed(speed), actual(0) {

		}
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
		* @param tweenIn scene that will be tweened in
		* @param tweenOut scene that will be tweened out
		* @param speed tweening speed
		*/
		SlideTween(TweenDirection direction, Node* tweenIn, Node* tweenOut, float speed) : Tween(tweenIn, tweenOut, speed), direction(direction) {

		}


		virtual void Update(const uint64 delta, const uint64 absolute) {

			int width = CogGetScreenWidth();
			int height = CogGetScreenHeight();

			// calculate actual position
			actual += 1.0f / width * CogTranslateSpeed(speed)*delta;

			if (actual > 1) {
				actual = 1;
			}

			// add sinus tweening
			float actSinus = (float)sin(actual*PI / 2);

			float widthActual = width*actSinus;
			float heightActual = height*actSinus;

			// change position according to the tweening direction
			if (direction == TweenDirection::RIGHT) {
				tweenOut->GetTransform().localPos.x = width / 2 + (widthActual);
				tweenIn->GetTransform().localPos.x = -width / 2 + widthActual;
			}
			else if (direction == TweenDirection::LEFT) {
				tweenIn->GetTransform().localPos.x = width / 2 + (width - widthActual);
				tweenOut->GetTransform().localPos.x = width / 2 - widthActual;
			}
			else if (direction == TweenDirection::UP) {
				tweenIn->GetTransform().localPos.x = width / 2;
				tweenIn->GetTransform().localPos.y = height / 2 + (height - heightActual);
				tweenOut->GetTransform().localPos.y = height / 2 - heightActual;
			}
			else if (direction == TweenDirection::DOWN) {
				tweenIn->GetTransform().localPos.x = (float)(width / 2);
				tweenOut->GetTransform().localPos.y = (float)(height / 2 + (heightActual));
				tweenIn->GetTransform().localPos.y = (float)(-height / 2 + heightActual);
			}

			if (actual >= 1.0f) {
				Finish();
				SendMessageNoBubbling(ACT_TWEEN_ENDED, 0, nullptr, tweenIn);
			}
		}
	};

}// namespace