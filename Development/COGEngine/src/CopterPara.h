#pragma once

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

public:
	CopterPara(CopterFactory* factory) : GBehavior(ElemType::MODEL, EnFlags(Actions::HEALTH_CHANGED)){
		this->factory = factory;
		this->grounded = false;
	}

	void OnMessage(GMsg& msg){
		if (msg.GetSourceObject()->GetId() == owner->GetId() && msg.GetAction() == Actions::HEALTH_CHANGED){
			int health = msg.GetSourceObject()->GetAttr<int>(Attrs::HEALTH);

			// add collision effect
			msg.GetSourceObject()->AddBehavior(new BeTempRender(COGGet2DImage("images/explosion2.png"), 3));

			// remove copter with delay
			if (health <= 0){
				SendMessage(Traversation(ScopeType::SCENE, true, true), Actions::PARA_KILLED, nullptr, msg.GetSourceObject());
				msg.GetSourceObject()->AddBehavior(new BeDelayRemove(100));
			}
		}
	}


	virtual void Update(const uint64 delta, const uint64 absolute){

		if (!grounded){

			if (owner->GetTransform().AbsPos.y > 0.8f*COGGetHeight()){
				// change picture and do nothing....
				grounded = true;
				owner->SetTag("para_grounded");
				owner->ChangeAttr<spt<ofImage>>(Attrs::IMGSOURCE, COGGet2DImage("images/soldier.png"));
				SendMessage(Traversation(ScopeType::SCENE, true, true), Actions::PARA_GROUNDED, nullptr, owner);
			}
			else{
				owner->GetTransform().LocalPos.y += 0.4f;
			}
		}
	}


	
};

