#pragma once

#include "GBehavior.h"
#include "GNode.h"

/**
* Behavior for rotation animation
*/
class BeRotateAnim : public GBehavior{

private:
	float _from;
	float _to;
	float _speed;
	bool _additive;
	float _actual;
	bool _stopped;

public:
	BeRotateAnim(float from, float to, float speed, bool additive) : 
		GBehavior(ElemType::MODEL), _from(from), _to(to), _speed(speed), _additive(additive), _stopped(false), _actual(0){

	}


	void Update(const uint64 delta, const uint64 absolute){

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
};
