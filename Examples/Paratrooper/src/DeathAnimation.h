#pragma once

#include "Component.h"
#include "TransformBuilder.h"
#include "GameManager.h"
#include "Behavior.h"

using namespace Cog;

/**
 * Very simple flickering animation that constantly enables and disables visibility of target mesh
 */
class DeathAnimation : public Behavior {
	uint64_t lastSwitch = 0;
	int totalSw = 0;
public:
	virtual void Update(uint64_t delta, uint64_t absolute) {
		
		if(CheckTime(lastSwitch, absolute, 50)) {
			lastSwitch = absolute;
			owner->GetMesh()->SetIsVisible(!owner->GetMesh()->IsVisible());
			if(totalSw++ > 4) {
				this->Finish();
				SendMessage(ANIMATION_ENDED);
			}
		}
	}
};
