#include "BehaviorEnt.h"

namespace Cog {

	void BehaviorEnt::LoadFromXml(xml_node& xml, Setting& set) {

		// get reference (not mandatory)
		this->ref = xml.attribute("ref").as_string();

		if (this->ref.empty()) {
			this->name = xml.attribute("name").as_string();
		}
		else this->name = this->ref;

		// get type
		this->type = xml.attribute("type").as_string();

		// load settings
		this->setting = Setting();

		auto settingNode = xml.child("setting");
		if (settingNode) {
			this->setting = Setting();
			this->setting.LoadFromXml(settingNode);
		}

		// there are two ways how to specify settings: as a setting tag or 
		// by using attributes

		// case 1:  <behavior type="MyBehavior"> <setting><item key="myKey" value="myValue"/></setting> </behavior>
		// case 2:  <behavior type="MyBehavior" myKey="myValue" />   -> easier

	
		for (auto attr : xml.attributes()) {
			auto attrName = string(attr.name());
			if (attrName.compare("name") != 0 && attrName.compare("type") != 0 && attrName.compare("ref") != 0) {
				// settings could be specified even as attributes of the behavior tag!
				string val = attr.value();

				this->setting.AddItem(attr.name(), val);
			}
		}
	}

} // namespace