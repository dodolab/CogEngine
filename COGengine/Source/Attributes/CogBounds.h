#pragma once

#include "ofxCogMain.h"
#include "CogNode.h"

namespace Cog {

	/**
	* Entity for object physical bounds; can be overriden to implement
	* custom boundary check
	*/
	class CogBounds {

	public:

		CogBounds() {

		}


		/**
		* Returns true, if object collides with another
		* @param first first object to check
		* @param second second object to check
		*/
		bool Collides(CogNode& first, CogNode& second) const;

	};



}