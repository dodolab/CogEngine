#pragma once

#include "Component.h"
#include "GameValues.h"
#include "Dynamics.h"
#include "Behavior.h"

using namespace Cog;

/**
 * Component that controls movement of the projectile
 */
class ProjectileComponent : public Behavior {
public:

	virtual void Update(uint64_t delta, uint64_t absolute) {

		// update location
		auto dynamics = owner->GetAttr<Dynamics*>(ATTR_MOVEMENT);
		dynamics->UpdateVelocity(delta, CogGetGameSpeed());
		auto deltaPos = dynamics->CalcDelta(delta, CogGetGameSpeed());

		auto& trans = owner->GetTransform();
		trans.localPos.x += deltaPos.x;
		trans.localPos.y += deltaPos.y;

		auto virtualWidth = CogGetVirtualWidth();
		auto virtualHeight = CogGetVirtualHeight();

		// check boundaries
		auto& absPos = owner->GetTransform().absPos;

		if (absPos.x < 0 || absPos.x > virtualWidth || absPos.y < 0 || absPos.y > virtualHeight) {
			// we are behind the screen -> remove projectile
			owner->RemoveFromParent(true);
		}
	}
};
