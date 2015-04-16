
#ifndef BERENDER_H
#define BERENDER_H

#include "ABehavior.h"

class BeRender : public ABehavior{
public:
	BeRender();
	void OnMessage(Msg& msg);
	void Update(const uint64 delta, const uint64 absolute);
};



BeRender::BeRender() : ABehavior(ElemType::VIEW, EnFlags()){

}

void BeRender::OnMessage(Msg& msg){

}

void BeRender::Update(const uint64 delta, const uint64 absolute){

}

#endif