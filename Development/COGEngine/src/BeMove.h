#pragma once

#include "GBehavior.h"
#include "GMsg.h"
#include "GNode.h"

/**
* Behavior for translation
*/
class BeMove : public GBehavior{
public:
	BeMove() : GBehavior(ElemType::MODEL, EnFlags()){

	}


	void Update(const uint64 delta, const uint64 absolute){
		EnTransform& transform = owner->GetTransform();

		ofVec3f velocity = owner->GetAttr<ofVec3f>(Attrs::VELOCITY);

		transform.LocalPos.x += velocity.x * delta;
		transform.LocalPos.y += velocity.y * delta;
	}

};