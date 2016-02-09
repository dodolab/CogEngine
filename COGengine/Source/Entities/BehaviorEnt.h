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

		BehaviorEnt(string name, string type, Setting setting) : setting(setting), type(type) {
			this->name = name;
		}

		~BehaviorEnt() {

		}

		string type;
		Setting setting;

		void LoadFromXml(spt<ofxXml> xml, Setting& setting) {

			this->name = xml->getAttributex("name", "");
			this->type = xml->getAttributex("type", "");
			
			this->setting = Setting();

			if (xml->pushTagIfExists("setting")) {
				this->setting = Setting();
				this->setting.LoadFromXml(xml);
				xml->popTag();
			}

			vector<string> allAttributes = vector<string>();
			xml->getAttributeNames(":", allAttributes);

			for (string attr : allAttributes) {
				if (attr.compare("name") != 0 && attr.compare("type") != 0) {
					// settings could be specified even as attributes of the behavior tag!
					string val = xml->getAttributex(attr, "");

					this->setting.AddItem(attr,val);
				}
			}
		}

	};
}// namespace


