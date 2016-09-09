
#ifndef BERENDER_H
#define BERENDER_H

#include "ABehavior.h"

/**
* Behavior for rendering
*/
class BeRender : public ABehavior{
public:
	BeRender();
	virtual void OnMessage(Msg& msg);
	virtual void Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner);
};




#endif