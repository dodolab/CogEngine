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
		Tween(Node* from, Node* to, float speed) : from(from), to(to), speed(speed), actual(0) {

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
		* @param from scene that will be tweened out
		* @param to scene that will be tweened inm
		* @param speed tweening speed
		*/
		SlideTween(TweenDirection direction, Node* from, Node* to, float speed) : Tween(from, to, speed), direction(direction) {

		}


		void SetFadeFunction(FadeFunction func) {
			this->fadeFunction = func;
		}


		virtual void Update(const uint64 delta, const uint64 absolute) {

			int width = CogGetScreenWidth();
			int height = CogGetScreenHeight();

			// calculate actual position
			actual += 1.0f / width * CogTranslateSpeed(speed)*delta;

			if (actual > 1) {
				actual = 1;
			}

			float fadeValue;

			if (fadeFunction != nullptr) fadeValue = fadeFunction(actual);
			else {
				fadeValue = (float)sin(actual*PI / 2);
			}

			float widthActual = width*fadeValue;
			float heightActual = height*fadeValue;

			// change position according to the tweening direction
			if (direction == TweenDirection::RIGHT) {
				from->GetTransform().localPos.x = (widthActual);
				to->GetTransform().localPos.x = -width  + widthActual;
			}
			else if (direction == TweenDirection::LEFT) {
				from->GetTransform().localPos.x = (- widthActual);
				to->GetTransform().localPos.x = width  - widthActual;
			}
			else if (direction == TweenDirection::UP) {
				to->GetTransform().localPos.x = 0;
				from->GetTransform().localPos.y = (float)((-heightActual));
				to->GetTransform().localPos.y = (float)(height - heightActual);
			}
			else if (direction == TweenDirection::DOWN) {
				to->GetTransform().localPos.x = (float)(0);
				from->GetTransform().localPos.y = (float)((heightActual));
				to->GetTransform().localPos.y = (float)(-height  + heightActual);
			}

			if (actual >= 1.0f) {
				Finish();
				SendMessageNoBubbling(ACT_TWEEN_ENDED, 0, nullptr, to);
			}
		}
	};

}// namespace