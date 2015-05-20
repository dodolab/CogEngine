
#ifndef BERENDER_H
#define BERENDER_H

#include "ABehavior.h"

/*! User input action enumerator */
enum class RenderType{
	IMAGE, RECTANGLE, POLYGON, ARC
};

/**
* Behavior for rendering
*/
class BeRender : public ABehavior{
private:
	RenderType _type;
public:
	BeRender(RenderType type);
	virtual void OnMessage(Msg& msg);
	virtual void Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner);
	void RenderImage(GNode* owner);
	void RenderPolygon(GNode* owner);
	void RenderRectangle(GNode* owner);
	void RenderArc(GNode* owner);
};




#endif