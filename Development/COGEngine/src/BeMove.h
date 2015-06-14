#ifndef BEMOVE_H
#define BEMOVE_H

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

	void OnMessage(GMsg& msg){

	}

	void Update(const uint64 delta, const uint64 absolute, const ofMatrix4x4& absMatrix, GNode* owner){
		EnTransform& transform = owner->GetTransform();

		Vectorf3 velocity = owner->GetAttr<Vectorf3>(Attrs::VELOCITY);

		transform.LocalPos.x += velocity.x * delta;
		transform.LocalPos.y += velocity.y * delta;
	}

};



#endif
