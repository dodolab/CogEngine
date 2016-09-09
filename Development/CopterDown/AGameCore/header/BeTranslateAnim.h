
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
	void Update(int delta, int absolute);
};


#endif
