
#ifndef BECOLLIDER_H
#define BECOLLIDER_H

#include "ABehavior.h"
#include "EnFlags.h"

class BeCollider : public ABehavior{
protected:
	EnFlags _firstColGroups;
	EnFlags _secondColGroups;

public:
	BeCollider(EnFlags firstColGroups, EnFlags secondColGroups);

	BeCollider(int colGroup);

	BeCollider(int firstColGroup, int secondColGroup);

	void OnMessage(Msg msg);
	void Update(int delta, int absolute);
};



BeCollider::BeCollider(EnFlags firstColGroups, EnFlags secondColGroups) : ABehavior(ElemType::MODEL, EnFlags()){

}

BeCollider::BeCollider(int colGroup) : BeCollider(EnFlags(colGroup), EnFlags(colGroup)){

}

BeCollider::BeCollider(int firstColGroup, int secondColGroup) : BeCollider(EnFlags(firstColGroup), EnFlags(secondColGroup)){

}

void BeCollider::OnMessage(Msg msg){

}

void BeCollider::Update(int delta, int absolute){

}

#endif