#pragma once

#include "GNode.h"
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



	void Update(const uint64 delta, const uint64 absolute){

		GNode* scoreInfo = COGFindGameObjectByTag("score");
		int score = scoreInfo->HasAttr(Attrs::SCORE) ? scoreInfo->GetAttr<int>(Attrs::SCORE) : 0;
		
		int maximumCopters = score == 0 ? 1 : (1+log10(score)*log10(score/10));

		if ((absolute - lastSpawn) > 10 && ((int)ofRandom(0,200)) == 10 && COGGetGameObjectsCountByTag("copter") <= maximumCopters){
			lastSpawn = absolute; 
			// spawn each 5 seconds
			GNode* scene = owner->GetSceneRoot();
			scene->AddChild(factory->CreateHelicopter(scene));
		}
	}


};

