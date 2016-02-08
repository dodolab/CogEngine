#pragma once

#include "ofxCogCommon.h"

namespace Cog {

	/**
	* Description entity
	* Common class for all entities that are used to store all information needed to reconstruct the real entity
	*/
	class DEntity {
	public:
		DEntity() : name("") {

		}

		virtual ~DEntity() {

		}


		string name;

		virtual void LoadFromXml(spt<ofxXml> xml) {

		}

	};

}// namespace


