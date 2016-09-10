#pragma once
#include "ofMain.h"

namespace Cog {

	class Movement {
	private:
		ofVec2f acceleration = ofVec2f(0);
		ofVec2f velocity = ofVec2f(0);
		float angularSpeed = 0;

	public:
		/**
		* Gets the acceleration vector
		*/
		ofVec2f GetAcceleration() {
			return acceleration;
		}

		/**
		* Sets the acceleration vector
		*/
		void SetAcceleration(ofVec2f acceleration) {
			this->acceleration = acceleration;
		}

		/**
		* Gets the velocity vector
		*/
		ofVec2f GetVelocity() {
			return velocity;
		}

		/**
		* Sets the velocity vector
		*/
		void SetVelocity(ofVec2f velocity) {
			this->velocity = velocity;
		}

		float GetAngularSpeed() {
			return angularSpeed;
		}

		/**
		* Sets the angular speed
		*/
		void SetAngularSpeed(float angularSpeed) {
			this->angularSpeed = angularSpeed;
		}

		void Stop() {
			this->velocity = ofVec2f(0);
			this->acceleration = ofVec2f(0);
			this->angularSpeed = 0;
		}
	};

}// namespace