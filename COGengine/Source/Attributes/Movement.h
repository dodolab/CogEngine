#pragma once

#include "ofVec2f.h"
#include "StrId.h"

namespace Cog {

	class Movement {
	private:
		// map of forces
		map<StrId, ofVec2f>* forces = nullptr;
		ofVec2f acceleration = ofVec2f(0);
		ofVec2f velocity = ofVec2f(0);
		float angularSpeed = 0;

	public:

		~Movement() {
			delete forces;
		}

		/**
		* Gets the acceleration vector
		*/
		ofVec2f GetAcceleration() {
			return acceleration;
		}

		/**
		* Sets the acceleration vector directly (no forces used)
		*/
		void SetAcceleration(ofVec2f acceleration) {
			this->acceleration = acceleration;
		}

		/**
		* Gets the force
		*/
		void AddForce(StrId key, ofVec2f force) {
			if (forces == nullptr) forces = new map<StrId, ofVec2f>();

			(*forces)[key] = force;
		}

		/**
		* Calculates resultant force according to all stored forces
		*/
		ofVec2f CalcAccelerationForce() {
			if (forces == nullptr) return acceleration;

			ofVec2f result = ofVec2f(0);

			for (auto& key : *forces) {
				result += key.second;
			}

			return result;
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
			if (forces != nullptr) {
				forces->clear();
			}
			this->velocity = ofVec2f(0);
			this->acceleration = ofVec2f(0);
			this->angularSpeed = 0;
		}
	};

}// namespace