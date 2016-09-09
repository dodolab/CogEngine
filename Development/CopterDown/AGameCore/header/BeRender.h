
#ifndef BERENDER_H
#define BERENDER_H

#include "ABehavior.h"

class BeRender : public ABehavior{
public:
	BeRender();

	void OnMessage(Msg msg);
	void Update(int delta, int absolute);
};



#endif
