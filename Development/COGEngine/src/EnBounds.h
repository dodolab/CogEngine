#pragma once

#include "GNode.h"

/**
* Entity for object physical bounds; can be overriden to implement
* custom boundary check
*/
class EnBounds{

public:

	EnBounds(){

	}


	/**
	* Returns true, if object collides with another
	* @param first first object to check
	* @param second second object to check
	*/
	bool Collides(GNode& first, GNode& second) const;

};



