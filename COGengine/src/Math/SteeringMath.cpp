#include "SteeringMath.h"

namespace Cog {

	ofVec2f SteeringMath::Seek(Trans& transform, Movement& movement, ofVec2f dest, float maxAcceleration) {
		ofVec2f direction = dest - transform.localPos;
		direction = direction.normalize();
		return direction*maxAcceleration;
	}

	ofVec2f SteeringMath::Arrive(Trans& transform, Movement& movement, ofVec2f dest, float decelerationSpeed, float stopDistance) {

		ofVec2f direction = dest - transform.localPos;
		float distance = direction.length();

		if (distance < stopDistance) {
			movement.Stop();
			return ofVec2f(INT_MIN);
		}
		// calculate speed
		float speed = distance / (1.0f / (decelerationSpeed/10));
		ofVec2f desiredVelocity = direction * speed / distance;

		auto force =  decelerationSpeed * (desiredVelocity - movement.GetVelocity());
		
		if (distance < 1) {
			force *= distance;
		}

		return force;
	}

	ofVec2f SteeringMath::Flee(Trans& transform, Movement& movement, ofVec2f dest, float fleeDistance, float maxAcceleration) {
		ofVec2f distance = dest - transform.localPos;
		float length = distance.length();
		distance.normalize();
		ofVec2f desiredSpeed;

		if (length > fleeDistance) desiredSpeed = ofVec2f(0);
		else desiredSpeed = -distance*maxAcceleration;

		ofVec2f acceleration = desiredSpeed - movement.GetVelocity();
		return acceleration;
	}

	ofVec2f SteeringMath::Follow(Trans& transform, Movement& movement, Path* path, float& currentPathPoint, 
		float pointTolerance, float finalPointTolerance, float maxAcceleration) {
		
		// find point on the path that is closest to the owner position
		int segment = 0;
		float newPathPoint;

		path->CalcPathPoint(currentPathPoint, transform.localPos, segment, newPathPoint);

		currentPathPoint = newPathPoint;

		float tolerance = pointTolerance;
		auto segments = path->GetSegments().size();

		// last segment
		if (segment == (segments - 1)) {
			tolerance = finalPointTolerance;
		}

		if (newPathPoint == -1 || (segment == (segments - 1) &&
			path->GetPathLength() <= (currentPathPoint + finalPointTolerance))) {
			// nowhere to go
			return ofVec2f(INT_MIN);
		}

		// find new destination point
		ofVec2f destination = path->CalcPathPosition(currentPathPoint + tolerance);

		// use arrive for moving
		auto force = Arrive(transform, movement, destination, maxAcceleration, -100000); // no stop distance

		if (force == ofVec2f() && segment == (segments - 1)) {
			return ofVec2f(INT_MIN);
		}

		return force;
	}

	ofVec2f SteeringMath::Wander(Trans& transform, Movement& movement, ofVec2f& wanderTarget, float wanderRadius, float wanderDistance,
		
		float wanderJitter, uint64 deltaTime) {
		ofVec2f randomVec = ofVec2f(ofRandomf(), ofRandomf());
		wanderTarget += randomVec*(deltaTime*wanderJitter);
		wanderTarget.normalize();
		wanderTarget *= wanderRadius;

		ofVec2f direction = movement.GetVelocity().normalize();
		ofVec2f shift = wanderTarget + (direction*wanderDistance);
		return shift;
	}

} // namespace