#include "BehaviorEnt.h"

namespace Cog {

	void BehaviorEnt::LoadFromXml(spt<ofxXml> xml, Setting& set) {

		// get reference (not mandatory)
		this->ref = xml->getAttributex("ref", "");

		if (this->ref.empty()) {
			this->name = xml->getAttributex("name", "");
		}
		else this->name = this->ref;

		// get type
		this->type = xml->getAttributex("type", "");

		// load settings
		this->setting = Setting();

		if (xml->pushTagIfExists("setting")) {
			this->setting = Setting();
			this->setting.LoadFromXml(xml);
			xml->popTag();
		}

		// there are two ways how to specify settings: as a setting tag or 
		// by using attributes

		// case 1:  <behavior type="MyBehavior"> <setting><item key="myKey" value="myValue"/></setting> </behavior>
		// case 2:  <behavior type="MyBehavior" myKey="myValue" />   -> easier

		vector<string> allAttributes;
		xml->getAttributeNames(":", allAttributes);

		for (string attr : allAttributes) {
			if (attr.compare("name") != 0 && attr.compare("type") != 0 && attr.compare("ref") != 0) {
				// settings could be specified even as attributes of the behavior tag!
				string val = xml->getAttributex(attr, "");

				this->setting.AddItem(attr, val);
			}
		}
	}

} // namespace