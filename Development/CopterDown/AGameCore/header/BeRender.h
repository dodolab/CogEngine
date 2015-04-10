
#ifndef BERENDER_H
#define BERENDER_H

#include "ABehavior.h"

class BeRender : public ABehavior{
public:
	BeRender();
	void OnMessage(Msg msg);
	void Update(uint64 delta, uint64 absolute);
};



BeRender::BeRender() : ABehavior(ElemType::VIEW, EnFlags()){

}

void BeRender::OnMessage(Msg msg){

}

void BeRender::Update(uint64 delta, uint64 absolute){

}

#endif