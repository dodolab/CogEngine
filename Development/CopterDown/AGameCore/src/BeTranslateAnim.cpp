
#include "BeTranslateAnim.h"
#include "GNode.h"
#include "IwGeomFVec2.h"




BeTranslateAnim::BeTranslateAnim(CIwFVec2 from, CIwFVec2 to, float speed, bool additive, bool repeat) : ABehavior(ElemType::MODEL, EnFlags()), _speed(speed),
_additive(additive), _to(to), _from(from), _repeat(repeat){
	this->_actual = CIwFVec2(from);
	this->_distX = to.x - from.x;
	this->_distY = to.y - from.y;
}

void BeTranslateAnim::OnMessage(Msg& msg){

}

void BeTranslateAnim::Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner){
	if (_stopped) return;

	float diffX = (float)(_distX / 1000 * _speed*delta);
	float diffY = (float)(_distY / 1000 * _speed*delta);

	if (CIwFVec2(_from - _to).GetLengthSquared() < CIwFVec2(_from - _actual).GetLengthSquared())
	{
		_actual = CIwFVec2(_to);
		if (_repeat)
		{
			auto temp = _from;
			_from = CIwFVec2(_to);
			_to = CIwFVec2(temp);
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

	CIwFMat2D& transform = owner->GetTransform();


	if (_additive)
	{
		auto trans = transform.GetTrans();
		transform.SetTrans(CIwFVec2(trans.x + diffX, trans.y + diffY));
	}
	else
	{
		transform.SetTrans(_actual);
	}
}

