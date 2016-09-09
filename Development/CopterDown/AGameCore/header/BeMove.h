
#ifndef BEMOVE_H
#define BEMOVE_H

#include "ABehavior.h"
#include "Msg.h"
#include "IwGeomVec2.h"
#include "IwGeomFMat2D.h"

class BeMove : public ABehavior{
public:
	BeMove();

	void OnMessage(Msg& msg);
	void Update(const uint64 delta, const uint64 absolute);
};



#endif
