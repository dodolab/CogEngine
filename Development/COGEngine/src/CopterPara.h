#ifndef COPTER_PARA_H
#define COPTER_PARA_H

#include "GBehavior.h"
#include "GNode.h"
#include "SmartPointer.h"
#include "EnPolygon.h"
#include "CopterFactory.h"
#include "BeTempRender.h"
#include "BeDelayRemove.h"

/**
* Behavior for copter para
*/
class CopterPara : public GBehavior{
private:
	CopterFactory* factory;
	bool grounded;
	GNode* lastOwner;
public:
	CopterPara(CopterFactory* factory) : GBehavior(ElemType::MODEL, EnFlags(Actions::HEALTH_CHANGED)){
		this->factory = factory;
		this->grounded = false;
	}

	void OnMessage(GMsg& msg){
		if (msg.GetSourceObject()->GetId() == lastOwner->GetId() && msg.GetAction() == Actions::HEALTH_CHANGED){
			int health = msg.GetSourceObject()->GetAttr<int>(Attrs::HEALTH);

			// add collision effect
			msg.GetSourceObject()->AddBehavior(new BeTempRender(MEngine.resourceCtrl->Get2DImage("images/explosion2.png"), 3));

			// remove copter with delay
			if (health <= 0) msg.GetSourceObject()->AddBehavior(new BeDelayRemove(100));
		}
	}


	virtual void Update(const uint64 delta, const uint64 absolute, GNode* owner){
		lastOwner = owner;
		if (!grounded){

			if (owner->GetTransform().AbsPos.y > 0.8f*MEngine.environmentCtrl->GetHeight()){
				// change picture and do nothing....
				grounded = true;
				owner->ChangeAttr<spt<ofImage>>(Attrs::IMGSOURCE, MEngine.resourceCtrl->Get2DImage("images/soldier.png"));
			}
			else{
				owner->GetTransform().LocalPos.y += 0.4f;
			}
		}
	}


	
};


#endif