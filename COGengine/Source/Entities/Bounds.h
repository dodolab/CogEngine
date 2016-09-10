#pragma once

#include "ofxCogCommon.h"
#include "Node.h"

namespace Cog {

	/**
	* Entity for object physical bounds; can be overriden to implement
	* custom boundary check
	*/
	class Bounds {

	public:

		Bounds() {

		}


		/**
		* Returns true, if object collides with another
		* @param first first object to check
		* @param second second object to check
		*/
		bool Collides(Node& first, Node& second) const;

	};



}// namespace