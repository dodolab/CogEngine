#pragma once

#include "Behavior.h"
#include "Bounds.h"
#include "Collision.h"
#include "Shape.h"
#include "Node.h"
#include "Movement.h"
#include "Path.h"
#include "SteeringMath.h"
#include "Scene.h"

namespace Cog {

	/**x
	* Steering behavior
	*/
	class SteeringBehavior : public Behavior {
		OBJECT_PROTOTYPE(SteeringBehavior)
	protected:
		SteeringMath steeringMath;

		float clampAngle(float x) {
			x = fmod(x + 180, 360);
			if (x < 0)
				x += 360;
			return x - 180;
		}

		void SetRotationDirection(Movement& movement, Trans& transform, ofVec2f destination, float maxAcceleration, uint64 delta) {
			float actualRotation = transform.rotation;
			float neededRotation = transform.CalcRotationToPosition(destination);
			
			actualRotation = DEG_TO_RAD*clampAngle(actualRotation);
			neededRotation = DEG_TO_RAD*clampAngle(neededRotation);

			float rotDiff = -RAD_TO_DEG*atan2(sin(actualRotation - neededRotation), cos(actualRotation - neededRotation));

			if (isnan(rotDiff)) rotDiff = 0;

			movement.SetAngularSpeed(rotDiff*maxAcceleration);
		}

	public:

		virtual void Update(const uint64 delta, const uint64 absolute) {

		}
	};

	class SeekBehavior : public SteeringBehavior {
	private:
		float maxAcceleration = 0;
		StringHash forceId;
	
	public:
		SeekBehavior(float maxAcceleration) :maxAcceleration(maxAcceleration){
			forceId = StringHash(this->GetId());
		}

		void Init() {
			if (!owner->HasAttr(ATTR_STEERING_BEH_SEEK_DEST)) {
				owner->AddAttr(ATTR_STEERING_BEH_SEEK_DEST, ofVec2f(0));
			}
		}

		virtual void Update(const uint64 delta, const uint64 absolute) {
			auto& transform = owner->GetTransform();
			Movement& movement = owner->GetAttr<Movement>(ATTR_MOVEMENT);
			ofVec2f dest = owner->GetAttr<ofVec2f>(ATTR_STEERING_BEH_SEEK_DEST);
			ofVec2f force = steeringMath.Seek(transform, movement, dest, maxAcceleration);
			movement.AddForce(forceId, force);
			this->SetRotationDirection(movement, transform, dest, maxAcceleration, delta);
		}
	};

	class ArriveBehavior : public SteeringBehavior {
	private:
		float decelerationSpeed = 0;
		float rotationSpeed = 0;
		StringHash forceId;
	public:

		ArriveBehavior(float decelerationSpeed, float rotationSpeed) :
			decelerationSpeed(decelerationSpeed),
			rotationSpeed(rotationSpeed){
			forceId = StringHash(this->GetId());
		}


		void Init() {
			if (!owner->HasAttr(ATTR_STEERING_BEH_SEEK_DEST)) {
				owner->AddAttr(ATTR_STEERING_BEH_SEEK_DEST, ofVec2f(0));
			}
		}
		ofVec2f wanderTarget = ofVec2f(0);

		virtual void Update(const uint64 delta, const uint64 absolute) {

			auto& transform = owner->GetTransform();
			Movement& movement = owner->GetAttr<Movement>(ATTR_MOVEMENT);
			ofVec2f dest = owner->GetAttr<ofVec2f>(ATTR_STEERING_BEH_SEEK_DEST);
			ofVec2f acceleration = steeringMath.Arrive(transform, movement, dest, decelerationSpeed);
			if (acceleration != ofVec2f(INT_MIN)) {
				movement.AddForce(forceId, acceleration);
				this->SetRotationDirection(movement, transform, dest, rotationSpeed, delta);
			}
			else {
				movement.AddForce(forceId, ofVec2f(0));
			}
		}
	};

	class FleeBehavior : public SteeringBehavior {
	private:
		float maxAcceleration = 0;
		float fleeDistance = 0;
		StringHash forceId;
	public:
		FleeBehavior(float maxAcceleration, float fleeDistance) :maxAcceleration(maxAcceleration),
	    fleeDistance(fleeDistance){
			forceId = StringHash(this->GetId());
		}

		void Init() {
			if (!owner->HasAttr(ATTR_STEERING_BEH_SEEK_DEST)) {
				owner->AddAttr(ATTR_STEERING_BEH_SEEK_DEST, ofVec2f(0));
			}
		}

		virtual void Update(const uint64 delta, const uint64 absolute) {
			auto& transform = owner->GetTransform();
			Movement& movement = owner->GetAttr<Movement>(ATTR_MOVEMENT);

			ofVec2f dest = owner->GetAttr<ofVec2f>(ATTR_STEERING_BEH_SEEK_DEST);
			ofVec2f acceleration = steeringMath.Flee(transform, movement, dest, fleeDistance, maxAcceleration);
			movement.AddForce(forceId,acceleration);

			this->SetRotationDirection(movement, transform, dest, maxAcceleration, delta);
		}
	};

	class FollowBehavior : public SteeringBehavior {
	private:
		Path* path;
		float currentPathPoint = 0;
		float maxAcceleration = 0;
		float pointTolerance = 0;
		StringHash forceId;
	public:
		FollowBehavior(Path * path, float maxAcceleration, float pointTolerance) 
			: path(path), maxAcceleration(maxAcceleration), pointTolerance(pointTolerance){
			forceId = StringHash(this->GetId());
		}

		void Init() {
			if (!owner->HasAttr(ATTR_STEERING_BEH_SEEK_DEST)) {
				owner->AddAttr(ATTR_STEERING_BEH_SEEK_DEST, ofVec2f(0));
			}
		}

		virtual void Update(const uint64 delta, const uint64 absolute) {
			auto& transform = owner->GetTransform();
			Movement& movement = owner->GetAttr<Movement>(ATTR_MOVEMENT);

			ofVec2f force = steeringMath.Follow(transform, movement, path, currentPathPoint,
				pointTolerance, maxAcceleration);

			if (force == ofVec2f(INT_MIN)) {
				Finish();
				return;
			}

			movement.AddForce(forceId, force);
			this->SetRotationDirection(movement, transform, transform.localPos+movement.GetVelocity(), maxAcceleration, delta);
		}
	};

	class WanderBehavior : public SteeringBehavior {
	private:
		float wanderRadius = 0;
		float wanderDistance = 0;
		float wanderJitter = 0;
		StringHash forceId;
	public:
		WanderBehavior(float wanderRadius, float wanderDistance, float wanderJitter) :
			wanderRadius(wanderRadius), wanderDistance(wanderDistance), wanderJitter(wanderJitter){
			forceId = StringHash(this->GetId());
		}

		void Init() {
			if (!owner->HasAttr(ATTR_STEERING_BEH_WANDER)) {
				owner->AddAttr(ATTR_STEERING_BEH_WANDER, ofVec2f(0));
			}
		}

		virtual void Update(const uint64 delta, const uint64 absolute) {
			auto& transform = owner->GetTransform();
			Movement& movement = owner->GetAttr<Movement>(ATTR_MOVEMENT);

			ofVec2f behWander = owner->GetAttr<ofVec2f>(StringHash(ATTR_STEERING_BEH_WANDER));
			ofVec2f force = steeringMath.Wander(transform, movement, behWander, wanderRadius, wanderDistance, wanderJitter,delta);
			owner->ChangeAttr(StringHash(ATTR_STEERING_BEH_WANDER), behWander);

			// debug display
			//Node* pointer2 = owner->GetScene()->FindNodeByTag("pointer2");
			//pointer2->GetTransform().localPos = transform.localPos+force*10;

			// add velocity dependency
			movement.AddForce(forceId, force-movement.GetVelocity()/2);
			this->SetRotationDirection(movement, transform, transform.localPos + movement.GetVelocity(), 4, delta);
		}
	};

}// namespace