#pragma once

#include "GBehavior.h"
#include "GMsg.h"
#include "GNode.h"

/**x
* Behavior for simple movement
*/
class BeMove : public GBehavior{
protected:
	float speed;
public:

	/**
	* Creates a new behavior for simple movement
	* @param speed movement speed
	*/
	BeMove(float speed) : speed(speed){

	}

	void Init(){
		if (!owner->HasAttr(Attrs::VELOCITY)){
			owner->AddAttr(Attrs::VELOCITY, ofVec3f());
		}
	}

	void Update(const uint64 delta, const uint64 absolute){
		EnTransform& transform = owner->GetTransform();

		ofVec3f velocity = owner->GetAttr<ofVec3f>(Attrs::VELOCITY);

		transform.localPos.x += COGTranslateSpeed(speed)*velocity.x * delta;
		transform.localPos.y += COGTranslateSpeed(speed)*velocity.y * delta;
	}

};