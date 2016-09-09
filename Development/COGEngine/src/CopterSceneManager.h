#pragma once

#include "GBehavior.h"
#include "GNode.h"
#include "CopterFactory.h"

/**
* Behavior for copter scene manager
*/
class CopterSceneManager : public GBehavior{
private: 
	CopterFactory* factory;
	int actualScene;
	int finishingDelay;
	bool finishingMode;

public:
	CopterSceneManager(CopterFactory* factory) : GBehavior(ElemType::MODEL, EnFlags(Actions::OBJECT_HIT, Actions::PLAYER_LOOSE)), factory(factory){
		_behState = BehState::ACTIVE_ALL;
		actualScene = 1;
		finishingMode = false;
	}

	void OnMessage(GMsg& msg){
		if (actualScene == 1 && msg.GetAction() == Actions::OBJECT_HIT && msg.GetSourceObject()->GetSubType() == 222){
			factory->SwitchToScene(0);
			actualScene = 0;
		}
		else if (msg.GetAction() == Actions::PLAYER_LOOSE){
			finishingMode = true;
			finishingDelay = 0;
			GNode* scene = msg.GetSourceObject()->GetSceneRoot();
			GNode* skull = factory->CreateSkull(scene);
			scene->AddChild(skull);
			scene->SetRunningMode(RunningMode::PAUSED_CHILDREN);
			
		}
		else if (msg.GetAction() == Actions::GAME_FINISHED){
			finishingMode = false;
			factory->SwitchToScene(1);
			actualScene = 1;
		}
	}

	void Update(const uint64 delta, const uint64 absolute){
		if (finishingMode){
			finishingDelay += delta;

			if (finishingDelay > 2000){
				finishingMode = false;
				factory->SwitchToScene(1);
				actualScene = 1;
			}
		}
	}
};
