
#include "BeRotateAnim.h"
#include "GNode.h"

BeRotateAnim::BeRotateAnim(float from, float to, float speed, bool additive) : ABehavior(ElemType::MODEL, EnFlags()), _from(from), _to(to), _speed(speed), _additive(additive), _stopped(false){

}

void BeRotateAnim::OnMessage(Msg& msg){

}

void BeRotateAnim::Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner){


	if (_stopped) return;

	float diff = (float)((_to == _from ? 1 : (_to - _from)) / 1000 * _speed*delta);
	_actual += diff;

	// if to == from, it is infinite rotation
	if (_to != _from && ((_to >= _from && _actual >= _to) || (_to < _from && _actual < _to))){
		_actual = _to;
		_stopped = true;
	}

	EnTransform& transform = owner->GetTransform();

	if (_additive) transform.Rotation += (diff);
	else transform.Rotation = (_actual);
}

