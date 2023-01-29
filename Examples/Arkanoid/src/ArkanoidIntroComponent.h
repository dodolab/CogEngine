#pragma once

#include "StrId.h"
#include "SteeringMath.h"
#include "ArkanoidConstants.h"
#include "Scene.h"
#include "ArkanoidModel.h"
#include "Behavior.h"
#include "ArkanoidFactory.h"

using namespace Cog;

/**
 * Component that only waits for a certain amount of time and switches from the intro to the first level
 */
class ArkanoidIntroComponent : public Behavior {
private:
	int introShowDelay = 5000;
	int introShowTime = 0;
	ArkanoidModel* model;
	ArkanoidFactory* factory;
public:

	virtual void OnInit() {
		model = owner->GetSceneRoot()->GetAttr<ArkanoidModel*>(ARKANOID_MODEL);
		SendMessage(ARK_EVENT_GAME_STARTED); // notify other components that the game has just started
		factory = GETCOMPONENT(ArkanoidFactory);
	}

	virtual void Update(uint64 delta, uint64 absolute) {
		introShowTime += delta;

		if (introShowTime > introShowDelay) {
			// go to the first level
			model->currentLevel = 1;
			factory->ResetGame();
		}
	}
};