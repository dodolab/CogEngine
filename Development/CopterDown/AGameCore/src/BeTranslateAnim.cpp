
#include "BeTranslateAnim.h"




BeTranslateAnim::BeTranslateAnim(CIwFVec2 from, CIwFVec2 to, float speed, bool additive, bool repeat) : ABehavior(ElemType::MODEL, EnFlags()), _speed(speed),
_additive(additive), _to(to), _from(from), _repeat(repeat){
	this->_actual = CIwFVec2(from);
	this->_distX = to.x - from.x;
	this->_distY = to.y - from.y;
}

void BeTranslateAnim::OnMessage(Msg& msg){

}

void BeTranslateAnim::Update(const uint64 delta, const uint64 absolute){

}

