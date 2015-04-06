
#ifndef BEMOVE_H
#define BEMOVE_H

#include "ABehavior.h"

class BeMove : public ABehavior{
public:
	BeMove();

	void OnMessage(Msg msg);
	void Update(int delta, int absolute);
};


#endif
