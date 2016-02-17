#pragma once

#include "ofxCogCommon.h"
#include "DEntity.h"
#include "Settings.h"

namespace Cog {


	class AttrAnimSetEnt : public DEntity{
	public:

		AttrAnimSetEnt() {
			this->name = "";
		}

		AttrAnimSetEnt(string name) : AttrAnimSetEnt() {
			this->name = name;
		}

		~AttrAnimSetEnt() {

		}

		float speed = 0.0f;
		int repeat = 1;
		bool inverted = false;
		

		void LoadFromXml(spt<ofxXml> xml, Setting& set) {
			this->name = xml->getAttributex("name", "");
			
		}

	};


}// namespace


