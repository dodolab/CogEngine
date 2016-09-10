#pragma once

#include <string>
#include <map>
#include "ofUtils.h"
#include "Transform.h"
#include "Movement.h"
namespace Cog
{

	class SteeringMath {
	public:
		ofVec2f Seek(Trans& transform, Movement& movement, ofVec2f dest, float maxAcceleration) {
			ofVec2f direction = dest - transform.localPos;
			direction = direction.normalize();
			return direction*maxAcceleration;
		}

		ofVec2f Arrive(Trans& transform, Movement& movement, ofVec2f dest, float decelerationSpeed, float stopDistance) {

			decelerationSpeed /= 10; // scale deceleration so that it will have the same effect as for acceleration
			ofVec2f direction = dest - transform.localPos;
			float distance = direction.length();

			if (distance < stopDistance){
				movement.Stop();
				return ofVec2f(INT_MIN);
			}

			float speed = distance / (1.0f/decelerationSpeed);
			ofVec2f desiredVelocity = direction * speed / distance;

			return decelerationSpeed*10*(desiredVelocity - movement.GetVelocity());
		}

		ofVec2f Flee(Trans& transform, Movement& movement, ofVec2f dest, float fleeDistance, float maxAcceleration) {
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
			return acceleration;
		}

		ofVec2f Follow(Trans& transform, Movement& movement, Path* path, float& currentPathPoint, float pointTolerance, float finalPointTolerance, float maxAcceleration) {
			// find point on the path that is closest to the owner position
			int segment = 0;
			float newPathPoint = path->CalcPathPoint(currentPathPoint, transform.localPos, segment);

			currentPathPoint = newPathPoint;

			float tolerance = pointTolerance;

			// last segment
			if (segment == (path->segments.size() - 1)) {
				tolerance = finalPointTolerance;
			}

			if (newPathPoint == -1 || (segment == (path->segments.size() - 1) && 
				path->pathLength <= (currentPathPoint + finalPointTolerance))) {
				// nowhere to go
				return ofVec2f(INT_MIN);
			}

			// find new destination point
			ofVec2f destination = path->CalcPathPosition(currentPathPoint + tolerance);

			auto force = Arrive(transform, movement, destination, maxAcceleration, -100000); // no stop distance
			//return Seek(transform, movement, destination, maxAcceleration);

			if (force == ofVec2f() && segment == (path->segments.size() - 1)) {
				return ofVec2f(INT_MIN);
			}

			return force;
		}

		ofVec2f Wander(Trans& transform, Movement& movement, ofVec2f& wanderTarget, float wanderRadius, float wanderDistance, 
			float wanderJitter, uint64 deltaTime) {
			ofVec2f randomVec = ofVec2f(ofRandomf(), ofRandomf());
			wanderTarget += randomVec*(deltaTime*wanderJitter);
			wanderTarget.normalize();
			wanderTarget *= wanderRadius;
			
			
			ofVec2f direction = movement.GetVelocity().normalize();
			ofVec2f shift = wanderTarget + (direction*wanderDistance);
			return shift;
		}
	};


} // namespace
