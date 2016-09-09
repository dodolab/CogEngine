#pragma once

#include "CogBehavior.h"
#include "CogMsg.h"
#include "CogNode.h"

/**x
* Behavior for simple movement
*/
class CogMove : public CogBehavior{
protected:
	float speed;
public:

	/**
	* Creates a new behavior for simple movement
	* @param speed movement speed
	*/
	CogMove(float speed) : speed(speed){

	}

	void Init(){
		if (!owner->HasAttr(Attrs::VELOCITY)){
			owner->AddAttr(Attrs::VELOCITY, ofVec3f());
		}
	}

	void Update(const uint64 delta, const uint64 absolute){
		CogTrans& transform = owner->GetTransform();

		ofVec3f velocity = owner->GetAttr<ofVec3f>(Attrs::VELOCITY);

		transform.localPos.x += CogTranslateSpeed(speed)*velocity.x * delta;
		transform.localPos.y += CogTranslateSpeed(speed)*velocity.y * delta;
	}

};