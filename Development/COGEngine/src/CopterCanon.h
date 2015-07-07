#pragma once

#include "GBehavior.h"
#include "GNode.h"
#include "SmartPointer.h"
#include "EnPolygon.h"
#include "CopterFactory.h"

/**
* Behavior for copter canon
*/
class CopterCanon : public GBehavior{
private:
	CopterFactory* factory;

public:
	CopterCanon(CopterFactory* factory) : GBehavior(ElemType::MODEL, EnFlags()){
		lastShot = 0;
		this->factory = factory;
	}

	int lastShot;

	virtual void Update(const uint64 delta, const uint64 absolute){
		GNode* scene = owner->GetSceneRoot();
		EnFlags& actions = scene->GetAttr<EnFlags>(Attrs::ACTIONS);
		EnTransform& transform = owner->GetTransform();

		if (actions.HasState((int)Act::LEFT)){
			if (transform.Rotation > -80){
				transform.Rotation -= 5;
			}
		}
		else if (actions.HasState((int)Act::RIGHT)){
			if (transform.Rotation < 80){
				transform.Rotation += 5;
			}
		}

		if (actions.HasState((int)Act::FIRE)){
			if ((absolute - lastShot) > 300){
				lastShot = absolute;

				GNode* bullet = factory->CreateBullet(owner);
				float velocity = 5;
				float angle = transform.Rotation*PI/180.0f;
				float velX = velocity * sin(angle);
				float velY = -velocity * cos(angle);

				bullet->AddAttr(Attrs::VELOCITY, ofVec2f(velX, velY));
				
				scene->AddChild(bullet);
			}
		}
	}
	
};

