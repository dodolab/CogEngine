#pragma once

#include "ofxCogMain.h"
#include "Node.h"

/**
* Entity for object physical bounds; can be overriden to implement
* custom boundary check
*/
class Bounds{

public:

	Bounds(){

	}


	/**
	* Returns true, if object collides with another
	* @param first first object to check
	* @param second second object to check
	*/
	bool Collides(Node& first, Node& second) const;

};



