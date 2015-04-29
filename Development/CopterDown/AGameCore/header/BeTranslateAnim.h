
#ifndef BETRANSLATE_ANIM
#define BETRANSLATE_ANIM

#include "ABehavior.h"
#include "IwGeomFVec2.h"

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
	void OnMessage(Msg& msg);
	void Update(const uint64 delta, const uint64 absolute);
};




#endif