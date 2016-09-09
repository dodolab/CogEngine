#pragma once

#include "Behavior.h"
#include "Msg.h"
#include "Node.h"

/**x
* Behavior for simple movement
*/
class Move : public Behavior{
protected:
	float speed;
public:

	/**
	* Creates a new behavior for simple movement
	* @param speed movement speed
	*/
	Move(float speed) : speed(speed){

	}

	void Init(){
		if (!owner->HasAttr(Attrs::VELOCITY)){
			owner->AddAttr(Attrs::VELOCITY, ofVec3f());
		}
	}

	void Update(const uint64 delta, const uint64 absolute){
		Transform& transform = owner->GetTransform();

		ofVec3f velocity = owner->GetAttr<ofVec3f>(Attrs::VELOCITY);

		transform.localPos.x += COGTranslateSpeed(speed)*velocity.x * delta;
		transform.localPos.y += COGTranslateSpeed(speed)*velocity.y * delta;
	}

};