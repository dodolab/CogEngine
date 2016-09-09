#ifndef ENBOUNDS_H
#define ENBOUNDS_H

#include "GNode.h"

/**
* EnBounds - Entity for object physical bounds
*/
class EnBounds{
private:
	float _width;
	float _height;

public:

	EnBounds(){

	}

	EnBounds(float width, float height);

	float GetWidth() const;

	void SetWidth(float val);

	float GetHeight() const;

	void SetHeight(float val);

	bool Collides(GNode& first, GNode& second) const;

};




#endif