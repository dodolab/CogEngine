#pragma once

#include "Behavior.h"
#include "Dynamics.h"
#include "Path.h"
#include "SteeringMath.h"

namespace Cog {

	/**
	* Base class for all steering behaviors
	* Steering behavior aim to help autonomous characters move in a realistic manner, by using simple forces
	* For more informations, see http://gamedevelopment.tutsplus.com/series/understanding-steering-behaviors--gamedev-12732
	*/
	class SteeringBehavior : public Behavior {
	protected:
		SteeringMath steeringMath;

		/**
		* Clamps angle to the interval <0,360>
		*/
		float ClampAngle(float x);

		/**
		* Sets rotation direction according to the actual movement forces
		*/
		void SetRotationDirection(Dynamics* dynamics, Trans& transform, ofVec2f destination, float maxAcceleration, uint64 delta);

	public:

		SteeringBehavior() {

		}

		virtual void Update(const uint64 delta, const uint64 absolute) {

		}
	};

	/**
	* Seek behavior: accelerates against destination point
	* Involves two forces: desired velocity and steering
	*/
	class SeekBehavior : public SteeringBehavior {
	private:
		float maxAcceleration = 0;
		StrId forceId;
		StrId seekDest = StrId(ATTR_STEERING_BEH_DEST);
		StrId attrMovement = StrId(ATTR_MOVEMENT);
	public:
		SeekBehavior(float maxAcceleration) :maxAcceleration(maxAcceleration){
			forceId = StrId(this->GetId());
		}

		void OnStart();

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

	/**
	* Arrive behavior: the same as seek with the exception that it prevents
	* the character from moving through the target; the character slows down as
	* it approaches the destination
	*/
	class ArriveBehavior : public SteeringBehavior {
	private:
		float decelerationSpeed = 0;
		float rotationSpeed = 0;
		// point of tolerance that indicates how far from
		// the destination point can the characted stop
		float pointTolerance = 0;
		StrId forceId;
		StrId seekDest = StrId(ATTR_STEERING_BEH_DEST);
		StrId attrMovement = StrId(ATTR_MOVEMENT);
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

	/**
	* Flee behavior, opposite of seek: characted flees away from the destination
	*/
	class FleeBehavior : public SteeringBehavior {
	private:
		float maxAcceleration = 0;
		float fleeDistance = 0;
		StrId forceId;
		StrId seekDest = StrId(ATTR_STEERING_BEH_DEST);
		StrId attrMovement = StrId(ATTR_MOVEMENT);
	public:
		FleeBehavior(float maxAcceleration, float fleeDistance) :maxAcceleration(maxAcceleration),
	    fleeDistance(fleeDistance){
			forceId = StrId(this->GetId());
		}

		void OnStart();

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

	/**
	* Follow behavior - character follows the path, consisting of an array of checkpoints
	*/
	class FollowBehavior : public SteeringBehavior {
	private:
		Path* path;
		int currentPathIndex = 0;
		float maxRadialAcceleration = 0;
		// indicates how far from checkpoint may the character go
		float pointTolerance = 0;
		// indicates how far from the last checkpoint may the character stop
		float finalPointTolerance = 0;
		StrId forceId;
		StrId attrMovement = StrId(ATTR_MOVEMENT);
		bool pathFinished = false;
	public:
		float maxAcceleration = 0;
		float forceStrength = 1;

		/**
		* Creates a new follow behavior
		* @param path path to follow
		* @param maxAcceleration maximum acceleration
		* @param maxRadialAcceleration maximum radial acceleration
		* @param pointTolerance indicates how far from checkpoint may the character go
		* @param finalPointTolerance indicates how far from the last checkpoint may the character stop
		*/
		FollowBehavior(Path* path, float maxAcceleration, float maxRadialAcceleration, float pointTolerance, float finalPointTolerance)
			: path(path), maxAcceleration(maxAcceleration), maxRadialAcceleration(maxRadialAcceleration),
			pointTolerance(pointTolerance), finalPointTolerance(finalPointTolerance) {
			forceId = StrId(this->GetId());
		}

		void ResetPath(Path* path) {
			this->path = path;
			this->currentPathIndex = 0;
			this->pathFinished = false;
		}

		bool PathFinished() const {
			return pathFinished || path->GetSegments().size() < 2;
		}

		void OnStart();

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

	/**
	* Behavior that produces a realistic casual movement which will make the player think
	* that the character is really alive and walking around
	*
	* The wandering is provided by projecting a circle in front of the characted; the movement
	* dependes on the angle between character and a point on the circle which moves along
	* the circumference randomly
	*/
	class WanderBehavior : public SteeringBehavior {
	private:
		// radius of the wandering circle
		float wanderRadius = 0;
		// distance of the wandering circle
		float wanderDistance = 0;
		// randomness of the point on the circle
		float wanderJitter = 0;
		StrId forceId;
		StrId wanderDest = StrId(ATTR_STEERING_BEH_WANDER);
		StrId attrMovement = StrId(ATTR_MOVEMENT);
	public:
		
		/**
		* Creates a new wander behavior
		* @param wanderRadius radius of the wandering circle
		* @param wanderDistance distance of the wandering circle
		* @param wanderJitter randomness of the point on the circle
		*/
		WanderBehavior(float wanderRadius, float wanderDistance, float wanderJitter) :
			wanderRadius(wanderRadius), wanderDistance(wanderDistance), wanderJitter(wanderJitter){
			forceId = StrId(this->GetId());
		}

		void OnStart();

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

}// namespace