#ifndef BETRANSLATE_ANIM
#define BETRANSLATE_ANIM

#include "GBehavior.h"
#include "IwGeomFVec2.h"
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
	Vectorf2 _to;
	Vectorf2 _from;
	bool _repeat;

	Vectorf2 _actual;

	bool _stopped = false;

public:
	BeTranslateAnim(Vectorf2 from, Vectorf2 to, float speed, bool additive, bool repeat) : GBehavior(ElemType::MODEL, EnFlags()), _speed(speed),
		_additive(additive), _to(to), _from(from), _repeat(repeat){
		this->_actual = Vectorf2(from);
		this->_distX = to.x - from.x;
		this->_distY = to.y - from.y;
	}

	void OnMessage(GMsg& msg){

	}

	void Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner){
		if (_stopped) return;

		float diffX = (float)(_distX / 1000 * _speed*delta);
		float diffY = (float)(_distY / 1000 * _speed*delta);

		if (Vectorf2(_from - _to).GetLengthSquared() < Vectorf2(_from - _actual).GetLengthSquared())
		{
			_actual = Vectorf2(_to);
			if (_repeat)
			{
				auto temp = _from;
				_from = Vectorf2(_to);
				_to = Vectorf2(temp);
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