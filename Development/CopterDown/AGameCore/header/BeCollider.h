
#ifndef BECOLLIDER_H
#define BECOLLIDER_H

#include "ABehavior.h"
#include "EnFlags.h"
#include "EnBounds.h"
#include "EnCollision.h"

/**
* Behavior for colliding two objects
*/
class BeCollider : public ABehavior{
protected:
	EnFlags _firstColGroups;
	EnFlags _secondColGroups;

public:
	BeCollider(EnFlags firstColGroups, EnFlags secondColGroups);

	BeCollider(int colGroup);

	BeCollider(int firstColGroup, int secondColGroup);

	void OnMessage(Msg& msg);
	void Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner);
};





#endif