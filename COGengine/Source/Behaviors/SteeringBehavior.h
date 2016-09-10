#pragma once

#include "Behavior.h"
#include "Movement.h"
#include "Path.h"
#include "SteeringMath.h"

namespace Cog {

	/**x
	* Steering behavior
	*/
	class SteeringBehavior : public Behavior {
	protected:
		SteeringMath steeringMath;

		float clampAngle(float x);

		void SetRotationDirection(Movement& movement, Trans& transform, ofVec2f destination, float maxAcceleration, uint64 delta);

	public:

		SteeringBehavior() {

		}

		virtual void Update(const uint64 delta, const uint64 absolute) {

		}
	};

	class SeekBehavior : public SteeringBehavior {
	private:
		float maxAcceleration = 0;
		StrId forceId;
	
	public:
		SeekBehavior(float maxAcceleration) :maxAcceleration(maxAcceleration){
			forceId = StrId(this->GetId());
		}

		void OnStart();

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

	class ArriveBehavior : public SteeringBehavior {
	private:
		float decelerationSpeed = 0;
		float rotationSpeed = 0;
		float pointTolerance = 0;
		StrId forceId;
	public:

		ArriveBehavior(float decelerationSpeed, float rotationSpeed, float pointTolerance) :
			decelerationSpeed(decelerationSpeed),
			rotationSpeed(rotationSpeed),
			pointTolerance(pointTolerance){
			forceId = StrId(this->GetId());
		}


		void OnStart();
		
		virtual void Update(const uint64 delta, const uint64 absolute);
	};

	class FleeBehavior : public SteeringBehavior {
	private:
		float maxAcceleration = 0;
		float fleeDistance = 0;
		StrId forceId;
	public:
		FleeBehavior(float maxAcceleration, float fleeDistance) :maxAcceleration(maxAcceleration),
	    fleeDistance(fleeDistance){
			forceId = StrId(this->GetId());
		}

		void OnStart();

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

	class FollowBehavior : public SteeringBehavior {
	private:
		Path* path;
		float currentPathPoint = 0;
		float maxAcceleration = 0;
		float pointTolerance = 0;
		float finalPointTolerance = 0;
		StrId forceId;
	public:
		FollowBehavior(Path * path, float maxAcceleration, float pointTolerance, float finalPointTolerance)
			: path(path), maxAcceleration(maxAcceleration), pointTolerance(pointTolerance), finalPointTolerance(finalPointTolerance){
			forceId = StrId(this->GetId());
		}

		void OnStart();

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

	class WanderBehavior : public SteeringBehavior {
	private:
		float wanderRadius = 0;
		float wanderDistance = 0;
		float wanderJitter = 0;
		StrId forceId;
	public:
		WanderBehavior(float wanderRadius, float wanderDistance, float wanderJitter) :
			wanderRadius(wanderRadius), wanderDistance(wanderDistance), wanderJitter(wanderJitter){
			forceId = StrId(this->GetId());
		}

		void OnStart();

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

}// namespace