#ifndef BEMOVE_H
#define BEMOVE_H

#include "GBehavior.h"
#include "GMsg.h"
#include "IwGeomVec2.h"
#include "IwGeomFMat2D.h"
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

	void Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner){
		EnTransform& transform = owner->GetTransform();

		CIwVec2 velocity = owner->GetAttr<CIwVec2>(Attrs::VELOCITY);

		transform.LocalPos.x += velocity.x * delta;
		transform.LocalPos.y += velocity.y * delta;
	}

};



#endif
