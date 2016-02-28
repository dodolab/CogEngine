#pragma once

#include "Behavior.h"
#include "Bounds.h"
#include "Collision.h"
#include "Shape.h"
#include "Node.h"
#include "Movement.h"
#include "Path.h"

namespace Cog {

	/**x
	* Steering behavior
	*/
	class SteeringBehavior : public Behavior {
		OBJECT_PROTOTYPE(SteeringBehavior)
	protected:

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
			ofVec2f direction = dest - transform.localPos;
			direction = direction.normalize();
			// todo verify the time scale
			movement.AddForce(forceId, direction*maxAcceleration);
			
			this->SetRotationDirection(movement, transform, dest, maxAcceleration, delta);
		}
	};

	class ArriveBehavior : public SteeringBehavior {
	private:
		float accelerationSpeed = 0;
		float rotationSpeed = 0;
		float decelerateDistance = 0;
		StringHash forceId;
	public:

		ArriveBehavior(float accelerationSpeed, float rotationSpeed, float decelerateDistance) :
			accelerationSpeed(accelerationSpeed),
			rotationSpeed(rotationSpeed), decelerateDistance(decelerateDistance){
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
			ofVec2f direction = dest - transform.localPos;
			float distance = direction.length();
			direction = direction.normalize();

			if (distance < 10) {
				movement.Stop();
				return;
			}

			float desiredSpeed = 0;
			if (distance < decelerateDistance) {
				desiredSpeed = accelerationSpeed*distance / decelerateDistance;
			}
			else {
				desiredSpeed = accelerationSpeed;
			}

			ofVec2f acceleration = direction*desiredSpeed - movement.GetVelocity();

			movement.AddForce(forceId, acceleration);
			this->SetRotationDirection(movement, transform, dest, rotationSpeed, delta);
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
			ofVec2f distance = dest - transform.localPos;
			float length = distance.length();
			distance.normalize();
			ofVec2f desiredSpeed;

			if (length > fleeDistance) {
				desiredSpeed = ofVec2f(0);
			}
			else {
				desiredSpeed = -distance*maxAcceleration;
			}
			
			ofVec2f acceleration = desiredSpeed - movement.GetVelocity();

			movement.AddForce(forceId,acceleration);

			this->SetRotationDirection(movement, transform, dest, maxAcceleration, delta);
		}
	};

	// must be with seekBehavior
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

			// find point on the path that is closest to the owner position
			float newPathPoint = path->CalcPathPoint(currentPathPoint, transform.localPos);
			currentPathPoint = newPathPoint;

			if (newPathPoint == -1 || path->pathLength <= (currentPathPoint + pointTolerance)) {
				// nowhere to go
				Finish();
				return;
			}

			// find new destination point
			ofVec2f destination = path->CalcPathPosition(currentPathPoint + pointTolerance);
			cout << destination.x << ", " << destination.y << endl;
			owner->ChangeAttr(ATTR_STEERING_BEH_SEEK_DEST,destination);

			ofVec2f direction = destination - transform.localPos;
			direction = direction.normalize();
			movement.AddForce(forceId, direction*maxAcceleration);

			this->SetRotationDirection(movement, transform, transform.localPos+movement.GetVelocity(), maxAcceleration, delta);
		}
	};

}// namespace