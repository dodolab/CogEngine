#pragma once

#include "Component.h"
#include "ofImage.h"
#include "CopterComponent.h"
#include "ParatrooperModel.h"
#include "Behavior.h"

using namespace Cog;

/**
 * Component that spawns copters
 */
class CopterSpawner : public Behavior {
	uint64_t lastSpawnTime = 0;
	float spawnFrequency;
	ParatrooperModel* model;
	ParatrooperFactory* factory;
public:

	virtual void OnInit() {
		SubscribeForMessages(UNIT_KILLED);
		model = owner->GetSceneRoot()->GetAttr<ParatrooperModel*>(MODEL);
		// get the initial value
		this->spawnFrequency = model->copterSpawnFrequency;
		factory = GETCOMPONENT(ParatrooperFactory);
	}

	virtual void OnMessage(Msg& msg) {
		// check if the copter was killed and decrease the amount
		if(msg.GetAction() == UNIT_KILLED && msg.GetContextNode()->GetTag() == OBJECT_COPTER) {
			model->coptersCreated--;
		}
	}

	virtual void Update(uint64_t delta, uint64_t absolute) {
		if (CheckTime(lastSpawnTime, absolute, spawnFrequency)) {
			model->coptersCreated++;
			lastSpawnTime = absolute;
			spawnFrequency *= 1.02f; // increase frequency, this is gonna be fun :-)
			factory->CreateCopter(owner, model);
			SendMessage(COPTER_CREATED);
		}
	}
};
