#pragma once

#include "StrId.h"
#include "Component.h"
#include "SteeringMath.h"
#include "ArkanoidConstants.h"
#include "Scene.h"
#include "ArkanoidModel.h"
#include "Behavior.h"

using namespace Cog;

/**
 * Component that reacts on life lost and removes appropriate paddle icon
 */
class ArkanoidLifeComponent : public Behavior {
public:
	ArkanoidModel* model;

	virtual void OnInit() {
		SubscribeForMessages(ARK_EVENT_LIFE_LOST);
		model = owner->GetSceneRoot()->GetAttr<ArkanoidModel*>(ARKANOID_MODEL);
	}

	virtual void OnMessage(Msg& msg) {
		if(msg.GetAction() == ARK_EVENT_LIFE_LOST) {
			// each icon has its own name, according to the life number
			string lifeName = string_format("life_%d", model->currentLives + 1); // +1, because we have already lost this life
			auto life = owner->GetScene()->FindNodeByTag(lifeName);
			life->RemoveFromParent(true);
		}
	}

	virtual void Update(uint64 delta, uint64 absolute) {
		
	}
};