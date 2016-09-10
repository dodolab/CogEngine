#pragma once

#include "Behavior.h"

namespace Cog {

	/**x
	* Behavior for translation animation
	*/
	class TranslateAnim : public Behavior {
	private:
		// speed in X axis
		float speedX = 0;
		// speed in Y axis
		float speedY = 0;
		// distance in X axis
		float distX = 0;
		// distance in Y axis
		float distY = 0;
		// if true, animation will be additive
		bool additive = false;
		// starting position
		ofVec3f to = ofVec3f();
		// ending position
		ofVec3f from = ofVec3f();

		// actual position
		ofVec3f actual = ofVec3f();

		FadeFunction fadeFunction;


	public:

		TranslateAnim() {

		}

		/**
		* Creates a new behavior for translation animation
		* @param from starting position
		* @param to ending position
		* @param speed in display width per second
		* @param additive if true, position will be additive (not overriden)
		*/
		TranslateAnim(ofVec3f from, ofVec3f to, float speed, bool additive);

		/**
		* Creates a new behavior for translation animation with fade function
		* @param from starting position
		* @param to ending position
		* @param speed in display width per second
		* @param fadeFunction fading function that accepts float <0..1> and returns fading parameter <0..1>
		*/
		TranslateAnim(ofVec3f from, ofVec3f to, float speed, FadeFunction fadeFunction);

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

}// namespace