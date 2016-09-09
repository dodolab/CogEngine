#pragma once

#include "ofxCogMain.h"
#include "CogNode.h"

/**
* Collision entity
*/
class CogCollision{
public:
	// first collided node
	CogNode* first;
	// second collided node
	CogNode* second;


	/**
	* Creates a new collision entity
	* @param first first collided node
	* @param second second collided node
	*/
	CogCollision(CogNode* first, CogNode* second){
		this->first = first;
		this->second = second;
	}
	
};

