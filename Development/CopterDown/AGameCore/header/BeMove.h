
#ifndef BEMOVE_H
#define BEMOVE_H

#include "ABehavior.h"
#include "Msg.h"

class BeMove : public ABehavior{
public:
	BeMove();

	void OnMessage(Msg msg);
	void Update(uint64 delta, uint64 absolute);
};


BeMove::BeMove() : ABehavior(ElemType::MODEL, EnFlags()){

}

void BeMove::OnMessage(Msg msg){

}

void BeMove::Update(uint64 delta, uint64 absolute){
	CIwFMat2D transform =_node->GetTransform();
	CIwVec2 velocity = _node->FindAtt<CIwVec2>(Attrs::VELOCITY).GetValue();

	transform.t.x += velocity.x * delta;
	transform.t.y += velocity.y * delta;
}

#endif
