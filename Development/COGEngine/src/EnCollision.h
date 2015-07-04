#include "GNode.h"
#ifndef ENCOLLISION_H
#define ENCOLLISION_H

/**
* EnCollision - Entity for collisions
*/
class EnCollision{
public:
	GNode* first;
	GNode* second;


	EnCollision(GNode* first, GNode* second){
		this->first = first;
		this->second = second;
	}
	
};


#endif

