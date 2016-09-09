#ifndef COPTER_HELIMANAGER_H
#define COPTER_HELIMANAGER_H

#include "GNode.h"
#include "MGameFactory.h"
#include "MGameEngine.h"
#include "Enums.h"
#include "ofBaseTypes.h"
#include "BeTempRender.h"
#include "BeDelayRemove.h"
#include "CopterFactory.h"

class CopterHeliManager : public GBehavior{

private:
	int lastSpawn;
	CopterFactory* factory;
public:
	CopterHeliManager(CopterFactory* factory) : GBehavior(ElemType::MODEL, EnFlags()){
		lastSpawn = 0;
		this->factory = factory;
	}

	void OnMessage(GMsg& msg){

	}

	void Update(const uint64 delta, const uint64 absolute, GNode* owner){
		if ((absolute - lastSpawn) > 10000){
			lastSpawn = absolute;
			// spawn each 5 seconds
			GNode* scene = owner->GetSceneRoot();
			scene->AddChild(factory->CreateHelicopter(scene));
		}
	}


};


#endif