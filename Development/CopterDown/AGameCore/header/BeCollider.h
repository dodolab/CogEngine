
#ifndef BECOLLIDER_H
#define BECOLLIDER_H

#include "ABehavior.h"
#include "EnState.h"

class BeCollider : public ABehavior{
protected:
	State* _firstColGroups;
	State* _secondColGroups;

public:
	BeCollider(State* firstColGroups, State* secondColGroups);
	BeCollider(int colGroup);
	BeCollider(int firstColGroup, int secondColGroup);

	void OnMessage(Msg msg);
	void Update(int delta, int absolute);
};

#endif
