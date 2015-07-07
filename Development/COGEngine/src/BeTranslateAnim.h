#pragma once

#include "GBehavior.h"
#include "GNode.h"
#include "Enums.h"

/**
* Behavior for translation animation
*/
class BeTranslateAnim : public GBehavior{

private:
	float _speedX;
	float _speedY;
	float _distX;
	float _distY;
	bool _additive;
	ofVec3f _to;
	ofVec3f _from;
	bool _repeat;

	ofVec3f _actual;

	bool _stopped;

public:
	BeTranslateAnim(ofVec3f from, ofVec3f to, float speed, bool additive, bool repeat) : GBehavior(ElemType::MODEL, EnFlags()),
		_additive(additive), _to(to), _from(from), _repeat(repeat){
		this->_actual = ofVec3f(from);
		this->_distX = to.x - from.x;
		this->_distY = to.y - from.y;
		
		float distance = (float)sqrt(_distX*_distX+_distY*_distY);
		_speedX = speed*_distX / distance;
		_speedY = speed*_distY / distance;

		_stopped = false;
	}


	void Update(const uint64 delta, const uint64 absolute){
		if (_stopped) return;

		float diffX = (float)(MEngine.TranslateSpeed(_speedX)*delta);
		float diffY = (float)(MEngine.TranslateSpeed(_speedY)*delta);

		if (ofVec3f(_from - _to).lengthSquared() < ofVec3f(_from - _actual).lengthSquared())
		{
			_actual = ofVec3f(_to);
			if (_repeat)
			{
				auto temp = _from;
				_from = ofVec3f(_to);
				_to = ofVec3f(temp);
				_distX = _to.x - _from.x;
				_distY = _to.y - _from.y;
			}
			else
			{
				_stopped = true;
			}
		}

		_actual.x += diffX;
		_actual.y += diffY;

		EnTransform& transform = owner->GetTransform();
		
		
		// todo: refaktoring needed !!!!
		if(diffX > 0) owner->SetState(States::TO_LEFT);
		else owner->SetState(States::TO_RIGHT);

		if (_additive)
		{
			transform.LocalPos.x += diffX;
			transform.LocalPos.y += diffY;
		}
		else
		{
			transform.LocalPos = (_actual);
		}
	}


};

