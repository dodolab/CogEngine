#pragma once

#include "Component.h"
#include "Behavior.h"
#include "ofImage.h"
#include "GameValues.h"
#include "ParatrooperFactory.h"
#include "DeathChecker.h"
#include "ParatrooperModel.h"

using namespace Cog;

/**
 * Base class that controls canon
 */
class CannonController : public Behavior {
private:
	// time of the last shot
	uint64_t lastShot = 0;
	ParatrooperModel* gameModel;
	ParatrooperFactory* factory;
public:
	
	virtual void OnInit() {
		gameModel = owner->GetSceneRoot()->GetAttr<ParatrooperModel*>(MODEL);
		factory = GETCOMPONENT(ParatrooperFactory);
	}

	// abstract
	virtual void Update(uint64_t delta, uint64_t absolute) = 0;

protected:


	/**
	 * Rotates the canon according to the delta time
	 */
	void Rotate(Dir direction, uint64_t delta) {

		auto& trans = owner->GetTransform();

		if (direction == Dir::LEFT) {
			trans.rotation -= 0.1f*delta;
		}
		else {
			trans.rotation += 0.1f*delta;
		}

		// check boundaries
		trans.rotation = max(min(trans.rotation, gameModel->maxCannonAngle), gameModel->minCannonAngle);
	}

	/**
	 * Tries to shoot the projectile according to the fire rate
	 */
	bool TryFire(uint64_t absolute) {
		if (CheckTime(lastShot, absolute, gameModel->cannonFireRate)) {
			lastShot = absolute;
			factory->CreateProjectile(owner, gameModel);
			SendMessage(PROJECTILE_SHOT);
			return true;
		}
		else {
			return false;
		}
	}
};

/**
 * Cannon controller that checks pressed keys
 */
class CannonInputController : public CannonController {
	virtual void Update(uint64_t delta, uint64_t absolute) {
		auto& pressedKeys = CogGetPressedKeys();

		// keys are mapped inside ParatrooperApp
		for (auto key : pressedKeys) {
			if (key->key == OF_KEY_LEFT) {
				this->Rotate(Dir::LEFT, delta);
			}
			if (key->key == OF_KEY_RIGHT) {
				this->Rotate(Dir::RIGHT, delta);
			}
			if (key->key == OF_KEY_ALT) {
				TryFire(absolute);
			}
		}
	}
};