#ifndef COPTER_SCENE_MANAGER_H
#define COPTER_SCENE_MANAGER_H

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

public:
	CopterSceneManager(CopterFactory* factory) : GBehavior(ElemType::MODEL, EnFlags(Actions::OBJECT_HIT, Actions::PLAYER_LOOSE)), factory(factory){
		_behState = BehState::ACTIVE_MESSAGES;
		actualScene = 1;
	}

	void OnMessage(GMsg& msg){
		if (actualScene == 1 && msg.GetAction() == Actions::OBJECT_HIT && msg.GetSourceObject()->GetSubType() == 222){
			factory->SwitchToScene(0);
			actualScene = 0;
		}
		else if (msg.GetAction() == Actions::PLAYER_LOOSE){
			factory->SwitchToScene(1);
			actualScene = 1;
		}
	}

	void Update(const uint64 delta, const uint64 absolute, GNode* owner){

	}
};


#endif