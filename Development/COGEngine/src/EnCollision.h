#pragma once

#include "GNode.h"

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

