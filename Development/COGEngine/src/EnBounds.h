#ifndef ENBOUNDS_H
#define ENBOUNDS_H

#include "GNode.h"

/**
* EnBounds - Entity for object physical bounds
*/
class EnBounds{
private:

public:

	EnBounds(){

	}


	bool Collides(GNode& first, GNode& second) const;

};




#endif