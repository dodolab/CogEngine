
#ifndef BERENDER_H
#define BERENDER_H

#include "ABehavior.h"

/**
* Behavior for rendering
*/
class BeRender : public ABehavior{
public:
	BeRender();
	void OnMessage(Msg& msg);
	void Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner);
};




#endif