
#ifndef BEMOVE_H
#define BEMOVE_H

#include "ABehavior.h"
#include "Msg.h"

class BeMove : public ABehavior{
public:
	BeMove();

	void OnMessage(Msg msg);
	void Update(int delta, int absolute);
};


BeMove::BeMove() : ABehavior(ElemType::MODEL, EnFlags()){

}

void BeMove::OnMessage(Msg msg){

}

void BeMove::Update(int delta, int absolute){

}

#endif
