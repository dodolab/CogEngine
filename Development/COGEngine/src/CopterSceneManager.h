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


public:
	CopterSceneManager(CopterFactory* factory) : GBehavior(ElemType::MODEL, EnFlags(Actions::OBJECT_HIT)), factory(factory){
		_behState = BehState::ACTIVE_MESSAGES;
	}

	void OnMessage(GMsg& msg){
		if (msg.GetAction() == Actions::OBJECT_HIT){
			factory->SwitchToScene(0);
		}
	}

	void Update(const uint64 delta, const uint64 absolute, const ofMatrix4x4& absMatrix, GNode* owner){

	}
};


#endif