
#include "SteeringBehavior.h"
#include "CogEngine.h"
#include "Node.h"

namespace Cog {


	float SteeringBehavior::clampAngle(float x) {
		x = fmod(x + 180, 360);
		if (x < 0)
			x += 360;
		return x - 180;
	}

	void SteeringBehavior::SetRotationDirection(Movement& movement, Trans& transform, ofVec2f destination, float maxAcceleration, uint64 delta) {
		float actualRotation = transform.rotation;
		float neededRotation = transform.CalcAngle(destination);

		actualRotation = DEG_TO_RAD*clampAngle(actualRotation);
		neededRotation = DEG_TO_RAD*clampAngle(neededRotation);

		float rotDiff = -RAD_TO_DEG*atan2(sin(actualRotation - neededRotation), cos(actualRotation - neededRotation));

		if (isnan(rotDiff)) rotDiff = 0;

		movement.SetAngularSpeed(rotDiff*maxAcceleration);
	}


	void SeekBehavior::OnStart() {
		if (!owner->HasAttr(ATTR_STEERING_BEH_SEEK_DEST)) {
			owner->AddAttr(ATTR_STEERING_BEH_SEEK_DEST, ofVec2f(0));
		}
	}

	void SeekBehavior::Update(const uint64 delta, const uint64 absolute) {
		auto& transform = owner->GetTransform();
		Movement& movement = owner->GetAttr<Movement>(ATTR_MOVEMENT);
		ofVec2f dest = owner->GetAttr<ofVec2f>(ATTR_STEERING_BEH_SEEK_DEST);
		ofVec2f force = steeringMath.Seek(transform, movement, dest, maxAcceleration);
		movement.SetForce(forceId, force);
		this->SetRotationDirection(movement, transform, dest, maxAcceleration, delta);
	}


	void ArriveBehavior::OnStart() {
		if (!owner->HasAttr(ATTR_STEERING_BEH_SEEK_DEST)) {
			owner->AddAttr(ATTR_STEERING_BEH_SEEK_DEST, ofVec2f(0));
		}

		if (!owner->HasAttr(ATTR_MOVEMENT)) {
			owner->AddAttr(ATTR_MOVEMENT, Movement());
		}
	}

	void ArriveBehavior::Update(const uint64 delta, const uint64 absolute) {

		auto& transform = owner->GetTransform();
		Movement& movement = owner->GetAttr<Movement>(ATTR_MOVEMENT);
		ofVec2f dest = owner->GetAttr<ofVec2f>(ATTR_STEERING_BEH_SEEK_DEST);
		ofVec2f acceleration = steeringMath.Arrive(transform, movement, dest, decelerationSpeed, pointTolerance);
		if (acceleration != ofVec2f(INT_MIN)) {
			movement.SetForce(forceId, acceleration);
			this->SetRotationDirection(movement, transform, dest, rotationSpeed, delta);
		}
		else {
			movement.SetForce(forceId, ofVec2f(0));
			Finish();
		}
	}


	void FleeBehavior::OnStart() {
		if (!owner->HasAttr(ATTR_STEERING_BEH_SEEK_DEST)) {
			owner->AddAttr(ATTR_STEERING_BEH_SEEK_DEST, ofVec2f(0));
		}
	}

	void FleeBehavior::Update(const uint64 delta, const uint64 absolute) {
		auto& transform = owner->GetTransform();
		Movement& movement = owner->GetAttr<Movement>(ATTR_MOVEMENT);

		ofVec2f dest = owner->GetAttr<ofVec2f>(ATTR_STEERING_BEH_SEEK_DEST);
		ofVec2f acceleration = steeringMath.Flee(transform, movement, dest, fleeDistance, maxAcceleration);
		movement.SetForce(forceId, acceleration);

		this->SetRotationDirection(movement, transform, dest, maxAcceleration, delta);
	}

	void FollowBehavior::OnStart() {
		if (!owner->HasAttr(ATTR_STEERING_BEH_SEEK_DEST)) {
			owner->AddAttr(ATTR_STEERING_BEH_SEEK_DEST, ofVec2f(0));
		}

		if (!owner->HasAttr(ATTR_MOVEMENT)) {
			owner->AddAttr(ATTR_MOVEMENT, Movement());
		}
	}

	void FollowBehavior::Update(const uint64 delta, const uint64 absolute) {
		auto& transform = owner->GetTransform();
		Movement& movement = owner->GetAttr<Movement>(ATTR_MOVEMENT);

		ofVec2f force = steeringMath.Follow(transform, movement, path, currentPathPoint,
			pointTolerance, finalPointTolerance, maxAcceleration);

		if (force == ofVec2f(INT_MIN)) {
			movement.Stop();
			Finish();
			return;
		}

		movement.SetForce(forceId, force);
		this->SetRotationDirection(movement, transform, transform.localPos + movement.GetVelocity(), maxAcceleration, delta);
	}


	void WanderBehavior::OnStart() {
		if (!owner->HasAttr(ATTR_STEERING_BEH_WANDER)) {
			owner->AddAttr(ATTR_STEERING_BEH_WANDER, ofVec2f(0));
		}
	}

	void WanderBehavior::Update(const uint64 delta, const uint64 absolute) {
		auto& transform = owner->GetTransform();
		Movement& movement = owner->GetAttr<Movement>(ATTR_MOVEMENT);

		ofVec2f behWander = owner->GetAttr<ofVec2f>(StrId(ATTR_STEERING_BEH_WANDER));
		ofVec2f force = steeringMath.Wander(transform, movement, behWander, wanderRadius, wanderDistance, wanderJitter, delta);
		owner->ChangeAttr(StrId(ATTR_STEERING_BEH_WANDER), behWander);

		// debug display
		//Node* pointer2 = owner->GetScene()->FindNodeByTag("pointer2");
		//pointer2->GetTransform().localPos = transform.localPos+force*10;

		// add velocity dependency
		movement.SetForce(forceId, force - movement.GetVelocity() / 2);
		this->SetRotationDirection(movement, transform, transform.localPos + movement.GetVelocity(), 4, delta);
	}

}// namespace