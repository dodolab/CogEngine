#pragma once

#include "Component.h"
#include "TransformBuilder.h"
#include "GameValues.h"
#include "Dynamics.h"
#include "Behavior.h"

using namespace Cog;

/**
 * Simple animator that switches between two images
 */
class CopterAnimator : public Behavior {
private:
	ofVec2f lastVelocity = ofVec2f(0);
public:

	virtual void Update(uint64_t delta, uint64_t absolute) {
		auto dynamics = owner->GetAttr<Dynamics*>(ATTR_MOVEMENT);
		auto& velocity = dynamics->GetVelocity();

		if (ofSign(velocity.x) != ofSign(lastVelocity.x) || lastVelocity.x == 0) {
			if (velocity.x < 0) {
				// to the left
				owner->GetMesh<Image>()->SetImage(CogGet2DImage(FILE_COPTER_LEFT));
			}
			else {
				// to the right
				owner->GetMesh<Image>()->SetImage(CogGet2DImage(FILE_COPTER_RIGHT));
			}
		}

		this->lastVelocity = velocity;
	}
};