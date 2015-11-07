#pragma once

#include "ofxAreMain.h"
#include "ofxAreNode.h"

/**
* Collision entity
*/
class ofxAraCollision{
public:
	// first collided node
	ofxAreNode* first;
	// second collided node
	ofxAreNode* second;


	/**
	* Creates a new collision entity
	* @param first first collided node
	* @param second second collided node
	*/
	ofxAraCollision(ofxAreNode* first, ofxAreNode* second){
		this->first = first;
		this->second = second;
	}
	
};

