#pragma once

#include "Component.h"
#include "TransformBuilder.h"
#include "CollisionManager.h"
#include "Dynamics.h"
#include "ParatrooperModel.h"
#include "Behavior.h"

using namespace Cog;

/**
 * Paratrooper state
 */
enum class ParaState {
	FALLING, FALLING_PARACHUTE, ON_GROUND, CAPTURING, FALLING_WIHTOUT_PARACHUTE, DEAD
};

/**
 * Game logic for each paratrooper
 */
class ParatrooperComponent : public Behavior {
private:
	Node* ground;
	spt<ofImage> paratrooper;
	spt<ofImage> paratrooperParachute;
	ParaState lastState;
	ParatrooperModel* model;
public:

	virtual void OnInit() {
		ground = owner->GetScene()->FindNodeByTag(OBJECT_GROUND);
		paratrooper = CogGet2DImage(FILE_PARATROOPER);
		paratrooperParachute = CogGet2DImage(FILE_PARATROOPER_PARACHUTE);
		lastState = owner->GetAttr<ParaState>(PARA_STATE);
		model = owner->GetSceneRoot()->GetAttr<ParatrooperModel*>(MODEL);
	}

	virtual void Update(uint64_t delta, uint64_t absolute) {
		auto& trans = owner->GetTransform();
		auto& dynamics = owner->GetAttr<Dynamics*>(ATTR_MOVEMENT);
		auto state = owner->GetAttr<ParaState>(PARA_STATE);

		dynamics->UpdateVelocity(delta, CogGetGameSpeed());
		auto movementDiff = dynamics->CalcDelta(delta, CogGetGameSpeed());

		trans.localPos.x += movementDiff.x;
		trans.localPos.y += movementDiff.y;

		if (lastState != state && state == ParaState::FALLING_WIHTOUT_PARACHUTE) {
			// change acceleration back to gravity
			dynamics->SetAcceleration(ofVec2f(0, model->gravity));
		}

		state = CheckStateChangeBehavior(state, delta);

		CheckStateChangeMesh(state);

		// set the new state
		owner->ChangeAttr(PARA_STATE, state);
		lastState = state;
	}

private:
	void DoCaptureAnimation(uint64_t delta) {

		// very simple animation -> go to the tower, then up and finally to the center
		auto& trans = owner->GetTransform();
		auto tower = owner->GetScene()->FindNodeByTag(OBJECT_TOWER);
		auto& towerBB = tower->GetMesh()->GetBoundingBox();
		auto& thisBB = owner->GetMesh()->GetBoundingBox();

		if (thisBB.bottomRight.x < towerBB.bottomLeft.x) {
			trans.localPos.x += delta * 0.1f * CogGetGameSpeed();
		}
		else if (thisBB.bottomLeft.x > towerBB.bottomRight.x) {
			trans.localPos.x -= delta * 0.1f* CogGetGameSpeed();
		}
		else if (thisBB.bottomRight.y > towerBB.topLeft.y) {
			trans.localPos.y -= delta * 0.1f* CogGetGameSpeed();
		}
		else if (thisBB.bottomRight.x < towerBB.bottomLeft.x + towerBB.GetSize().x / 2) {
			trans.localPos.x += delta * 0.1f* CogGetGameSpeed();
		}
		else if (thisBB.bottomLeft.x > towerBB.bottomLeft.x + towerBB.GetSize().x / 2) {
			trans.localPos.x -= delta * 0.1f* CogGetGameSpeed();
		}
	}


	ParaState CheckStateChangeBehavior(ParaState state, uint64_t delta) {
		auto& groundBB = ground->GetMesh()->GetBoundingBox();
		auto& paraBB = owner->GetMesh()->GetBoundingBox();
		auto& trans = owner->GetTransform();
		auto dynamics = owner->GetAttr<Dynamics*>(ATTR_MOVEMENT);

		switch (state) {
		case ParaState::FALLING:
			if (trans.localPos.y > model->parachuteOpenAltitude) {
				state = ParaState::FALLING_PARACHUTE;
				// parachute opened -> deccelerate
				dynamics->SetAcceleration(ofVec2f(0, -model->parachuteDecceleration));
			}
			break;
		case ParaState::FALLING_PARACHUTE:
			if (dynamics->GetVelocity().y < model->parachuteOpenVelocityThreshold) {
				// deccelerated enough -> reset acceleration and fall with constant velocity
				dynamics->SetAcceleration(ofVec2f(0));
				dynamics->SetVelocity(ofVec2f(0, model->parachuteOpenVelocityThreshold));
			}
			if (paraBB.bottomLeft.y >= groundBB.topLeft.y) {
				// on the ground -> reset both velocity and acceleration
				state = ParaState::ON_GROUND;
				dynamics->SetVelocity(ofVec2f(0));
				dynamics->SetAcceleration(ofVec2f(0));
				SendMessage(UNIT_LANDED, owner);
			}
			break;
		case ParaState::FALLING_WIHTOUT_PARACHUTE:
			if (paraBB.bottomLeft.y >= groundBB.topLeft.y) {
				// kill the paratrooper
				owner->SetState(FLAG_DEAD);
				dynamics->SetVelocity(ofVec2f(0));
				dynamics->SetAcceleration(ofVec2f(0));
				SendMessage(UNIT_KILLED, owner);
				state = ParaState::DEAD;
			}
			break;
		case ParaState::ON_GROUND:
			// do nothing
			break;
		case ParaState::CAPTURING:
			DoCaptureAnimation(delta);
			break;
		}

		return state;
	}

	void CheckStateChangeMesh(ParaState state) const {
		if (lastState != state) {
			// state changed -> decide what to do next

			TransformBuilder transBld;
			auto& paraBB = owner->GetMesh()->GetBoundingBox();
			auto& trans = owner->GetTransform();

			// change mesh according to the current state
			// transformation needs to be recalculated, because the image of paratrooper with opened parachute has different size than the image of a falling paratrooper
			if (lastState == ParaState::FALLING && state == ParaState::FALLING_PARACHUTE) {
				owner->GetMesh<Image>()->SetImage(paratrooperParachute);
				owner->GetTransform().CalcAbsTransform(owner->GetParent()->GetTransform());
				transBld.AbsolutePosition(paraBB.bottomLeft.x + paraBB.GetSize().x / 2, paraBB.bottomLeft.y).Anchor(0.5f, 1).LocalScale(trans.scale.x, trans.scale.y).CheckSceneScale(false).CheckNativeResolution(false).Build(owner);
			}
			else if (lastState == ParaState::FALLING_PARACHUTE && state == ParaState::FALLING_WIHTOUT_PARACHUTE) {
				owner->GetMesh<Image>()->SetImage(paratrooper);
				owner->GetTransform().CalcAbsTransform(owner->GetParent()->GetTransform());
				transBld.AbsolutePosition(paraBB.bottomLeft.x + paraBB.GetSize().x / 2, paraBB.bottomLeft.y).Anchor(0.5f, 1).LocalScale(trans.scale.x, trans.scale.y).CheckSceneScale(false).CheckNativeResolution(false).Build(owner);
			}
			else if (lastState == ParaState::FALLING_PARACHUTE && state == ParaState::ON_GROUND) {
				owner->GetMesh<Image>()->SetImage(paratrooper);
				owner->GetTransform().CalcAbsTransform(owner->GetParent()->GetTransform());
				transBld.AbsolutePosition(paraBB.bottomLeft.x + paraBB.GetSize().x / 2, paraBB.bottomLeft.y).Anchor(0.5f, 1).LocalScale(trans.scale.x, trans.scale.y).CheckSceneScale(false).CheckNativeResolution(false).Build(owner);
			}
		}
	}
};