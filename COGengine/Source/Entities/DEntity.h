#pragma once

#include <string>
using namespace std;

#include "Definitions.h"
#include "ofxXmlSettings.h"

namespace Cog {

	/**
	* Base class for all description entities
	* Description entity is an entity, loaded from xml in general, that contains informations required 
	* to construct the real entity (e.g. Behavior)
	*/
	class DEntity {
	public:
		// name of the entity (should be unique)
		string name;

		DEntity() : name("") {

		}

		virtual ~DEntity() {

		}
	};

}// namespace


