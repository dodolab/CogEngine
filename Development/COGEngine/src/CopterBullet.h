#ifndef COPTER_BULLET_H
#define COPTER_BULLET_H

#include "GBehavior.h"
#include "GNode.h"
#include "SmartPointer.h"
#include "EnPolygon.h"
#include "CopterFactory.h"

/**
* Behavior for copter bullet
*/
class CopterBullet : public GBehavior{
private:
	CopterFactory* factory;

public:
	CopterBullet(CopterFactory* factory) : GBehavior(ElemType::MODEL, EnFlags()){
		this->factory = factory;
	}

	virtual void OnMessage(GMsg& msg){

	}


	virtual void Update(const uint64 delta, const uint64 absolute, GNode* owner){
		
		ofVec2f velocity = owner->GetAttr<ofVec2f>(Attrs::VELOCITY);

		owner->GetTransform().LocalPos.x += velocity.x;
		owner->GetTransform().LocalPos.y += velocity.y;

		if (owner->GetTransform().AbsPos.x < -20 || owner->GetTransform().AbsPos.x > MEngine.environmentCtrl->GetWidth() + 20 ||
			owner->GetTransform().AbsPos.y < -20 || owner->GetTransform().AbsPos.y > MEngine.environmentCtrl->GetHeight() + 20){
			// delete
			owner->GetParent()->RemoveChild(owner);
		}

	}


	
};


#endif