#pragma once

#include "ofxCogCommon.h"
#include "DEntity.h"
#include "Settings.h"
#include "ResourceCache.h"

namespace Cog {

	/**
	* Behavior entity
	*/
	class BehaviorEnt : public DEntity{
	public:

		BehaviorEnt() {

		}

		BehaviorEnt(string name, Setting setting) : setting(setting){
			this->name = name;
		}

		string type;
		Setting setting;

		void LoadFromXml(spt<ofxXml> xml, Setting& behaviorSetting) {
			this->name = xml->getAttributex("name", "");
			this->type = xml->getAttributex("type", "");
			this->setting = behaviorSetting;
		}

	};
}// namespace


