#pragma once

#include "GNode.h"

/**
* Collision entity
*/
class EnCollision{
public:
	// first collided node
	GNode* first;
	// second collided node
	GNode* second;


	/**
	* Creates a new collision entity
	* @param first first collided node
	* @param second second collided node
	*/
	EnCollision(GNode* first, GNode* second){
		this->first = first;
		this->second = second;
	}
	
};

