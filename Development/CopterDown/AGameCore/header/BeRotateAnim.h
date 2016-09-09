
#ifndef BEROTATE_ANIM_H
#define BEROTATE_ANIM_H

#include "ABehavior.h"

/**
* Behavior for rotation animation
*/
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

	void OnMessage(Msg& msg);
	void Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner);
};



#endif