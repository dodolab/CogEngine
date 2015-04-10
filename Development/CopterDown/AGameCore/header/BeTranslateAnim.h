
#ifndef BETRANSLATE_ANIM
#define BETRANSLATE_ANIM

#include "ABehavior.h"

class BeTranslateAnim : public ABehavior{

private:
	float _speed;
	float _distX;
	float _distY;
	bool _additive;
	CIwFVec2 _to;
	CIwFVec2 _from;
	bool _repeat;

	CIwFVec2 _actual;

	bool _stopped;

public:
	BeTranslateAnim(CIwFVec2 from, CIwFVec2 to, float speed, bool additive, bool repeat);
	void OnMessage(Msg msg);
	void Update(uint64 delta, uint64 absolute);
};



BeTranslateAnim::BeTranslateAnim(CIwFVec2 from, CIwFVec2 to, float speed, bool additive, bool repeat) : ABehavior(ElemType::MODEL, EnFlags()), _speed(speed),
_additive(additive), _to(to), _from(from), _repeat(repeat){
	this->_actual = CIwFVec2(from);
	this->_distX = to.x - from.x;
	this->_distY = to.y - from.y;
}

void BeTranslateAnim::OnMessage(Msg msg){

}

void BeTranslateAnim::Update(uint64 delta, uint64 absolute){

}

#endif