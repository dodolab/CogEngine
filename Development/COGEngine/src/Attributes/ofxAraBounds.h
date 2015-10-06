#pragma once

#include "ofxAreMain.h"
#include "ofxAreNode.h"

/**
* Entity for object physical bounds; can be overriden to implement
* custom boundary check
*/
class ofxAraBounds{

public:

	ofxAraBounds(){

	}


	/**
	* Returns true, if object collides with another
	* @param first first object to check
	* @param second second object to check
	*/
	bool Collides(ofxAreNode& first, ofxAreNode& second) const;

};



