#pragma once

#include "ofxCogMain.h"
#include "Node.h"

namespace Cog {

	/**
	* Collision entity
	*/
	class Collision {
	public:
		// first collided node
		Node* first;
		// second collided node
		Node* second;


		/**
		* Creates a new collision entity
		* @param first first collided node
		* @param second second collided node
		*/
		Collision(Node* first, Node* second) {
			this->first = first;
			this->second = second;
		}

	};

}// namespace