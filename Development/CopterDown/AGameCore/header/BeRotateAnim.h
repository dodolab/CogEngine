
#ifndef BEROTATE_ANIM_H
#define BEROTATE_ANIM_H

#include "ABehavior.h"

class BeRotateAnim : public ABehavior{

private:
	float _from;
	float _to;
	float _speed;
	bool _additive;
	float _actual;
	bool _stopped;

public:
	BeRotateAnim(float from, float to, float speed, bool additive);

	void OnMessage(Msg& msg);
	void Update(const uint64 delta, const uint64 absolute);
};


BeRotateAnim::BeRotateAnim(float from, float to, float speed, bool additive) : ABehavior(ElemType::MODEL, EnFlags()), _from(from), _to(to), _speed(speed), _additive(additive), _stopped(false){

}

void BeRotateAnim::OnMessage(Msg& msg){

}

void BeRotateAnim::Update(const uint64 delta, const uint64 absolute){

}

#endif