
#ifndef ENBOUNDS_H
#define ENBOUNDS_H

#include "GNode.h"

class EnBounds{
private:
	float _width;
	float _height;

public:

	EnBounds(float width, float height);

	float GetWidth();
	void SetWidth(float val);

	float GetHeight();
	void SetHeight(float val);

	bool Collides(GNode* first, GNode* second);

};

#endif
