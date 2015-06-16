#ifndef BETRANSLATE_ANIM
#define BETRANSLATE_ANIM

#include "GBehavior.h"
#include "GNode.h"
#include "Enums.h"

/**
* Behavior for translation animation
*/
class BeTranslateAnim : public GBehavior{

private:
	float _speed;
	float _distX;
	float _distY;
	bool _additive;
	Vectorf3 _to;
	Vectorf3 _from;
	bool _repeat;

	Vectorf3 _actual;

	bool _stopped;

public:
	BeTranslateAnim(Vectorf3 from, Vectorf3 to, float speed, bool additive, bool repeat) : GBehavior(ElemType::MODEL, EnFlags()), _speed(speed),
		_additive(additive), _to(to), _from(from), _repeat(repeat){
		this->_actual = Vectorf3(from);
		this->_distX = to.x - from.x;
		this->_distY = to.y - from.y;

		_stopped = false;
	}

	void OnMessage(GMsg& msg){

	}

	void Update(const uint64 delta, const uint64 absolute, const ofMatrix4x4& absMatrix, GNode* owner){
		if (_stopped) return;

		float diffX = (float)(_distX / 1000 * _speed*delta);
		float diffY = (float)(_distY / 1000 * _speed*delta);

		if (Vectorf3(_from - _to).lengthSquared() < Vectorf3(_from - _actual).lengthSquared())
		{
			_actual = Vectorf3(_to);
			if (_repeat)
			{
				auto temp = _from;
				_from = Vectorf3(_to);
				_to = Vectorf3(temp);
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



#endif