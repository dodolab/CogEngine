#pragma once

#include "Behavior.h"
#include "Node.h"

namespace Cog {

	/**x
	* Behavior for translation animation
	*/
	class TranslateAnim : public Behavior {

	private:
		// speed in X axis
		float speedX;
		// speed in Y axis
		float speedY;
		// distance in X axis
		float distX;
		// distance in Y axis
		float distY;
		// if true, animation will be additive
		bool additive;
		// starting position
		ofVec3f to;
		// ending position
		ofVec3f from;

		// actual position
		ofVec3f actual;

		FadeFunction fadeFunction;


	public:

		/**
		* Creates a new behavior for translation animation
		* @param from starting position
		* @param to ending position
		* @param speed in display width per second
		* @param additive if true, position will be additive (not overriden)
		*/
		TranslateAnim(ofVec3f from, ofVec3f to, float speed, bool additive) :
			additive(additive), to(to), from(from) {
			this->actual = ofVec3f(from);
			this->distX = to.x - from.x;
			this->distY = to.y - from.y;

			float distance = (float)sqrt(distX*distX + distY*distY);
			speedX = speed*distX / distance;
			speedY = speed*distY / distance;
			fadeFunction = nullptr;

		}

		/**
		* Creates a new behavior for translation animation with fade function
		* @param from starting position
		* @param to ending position
		* @param speed in display width per second
		* @param fadeFunction fading function that accepts float <0..1> and returns fading parameter <0..1>
		*/
		TranslateAnim(ofVec3f from, ofVec3f to, float speed, FadeFunction fadeFunction) :
			additive(additive), to(to), from(from) {
			this->actual = ofVec3f(from);
			this->distX = to.x - from.x;
			this->distY = to.y - from.y;

			float distance = (float)sqrt(distX*distX + distY*distY);
			speedX = speed*distX / distance;
			speedY = speed*distY / distance;

			this->fadeFunction = fadeFunction;
			this->additive = false;
		}


		void Update(const uint64 delta, const uint64 absolute) {

			// calculate differencial
			float diffX = (float)(CogTranslateSpeed(speedX)*delta);
			float diffY = (float)(CogTranslateSpeed(speedY)*delta);
			bool finished = false;

			actual.x += diffX;
			actual.y += diffY;

			if (ofVec3f(from - to).lengthSquared() < ofVec3f(from - actual).lengthSquared())
			{
				actual = ofVec3f(to);
				finished = true;
			}

			Trans& transform = owner->GetTransform();

			// change position
			if (additive)
			{
				transform.localPos.x += diffX;
				transform.localPos.y += diffY;
			}
			else
			{
				float lengthFromTo = ofVec3f(from - to).length();
				float lengthFromActual = ofVec3f(from - actual).length();
				float percent = lengthFromActual / lengthFromTo;

				if (fadeFunction != nullptr) percent = fadeFunction(percent);

				ofVec3f actualAmount = (to - from)*(percent);
				transform.localPos = from + actualAmount;
			}

			if (finished) Finish();
		}

	};

}// namespace