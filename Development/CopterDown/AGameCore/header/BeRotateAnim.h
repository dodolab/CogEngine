
#ifndef BEROTATE_ANIM_H
#define BEROTATE_ANIM_H

#include "ABehavior.h"

class BeRotateAnim : public ABehavior{

private:
	float _from;
	float _to;
	float _speed;
	bool _additive;
	float _actual;
	bool _stopped;

public:
	BeRotateAnim(float from, float to, float speed, bool additive);

	void OnMessage(Msg msg);
	void Update(int delta, int absolute);
};


#endif
