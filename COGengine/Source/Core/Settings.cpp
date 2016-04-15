#include "Settings.h"

namespace Cog {

	// ===================== SettingItem =========================

	void SettingItem::AddValues(string vals) {
		if (vals.find(MULTIVAL_DELIMITER) != -1) {
			// more than one value
			vector<string> multiVals = split_string(vals, MULTIVAL_DELIMITER);
			for (string valpart : multiVals) {
				values.push_back(valpart);
			}
		}
		else {
			values.push_back(vals);
		}
	}

	string SettingItem::GetValStr(string defaultVal) {
		if (values.size() == 0) return defaultVal;
		return values[0];
	}

	double SettingItem::GetValDouble(double defaultVal) {
		if (values.size() == 0) return defaultVal;
		return ofToDouble(values[0]);
	}

	int SettingItem::GetValInt(int defaultVal) {
		if (values.size() == 0) return defaultVal;
		return ofToInt(values[0]);
	}

	float SettingItem::GetValFloat(float defaultVal) {
		if (values.size() == 0) return defaultVal;
		return ofToFloat(values[0]);
	}

	bool SettingItem::GetValBool(bool defaultVal) {
		if (values.size() == 0) return defaultVal;
		return ofToBool(values[0]);
	}

	// ======================= Setting ===========================

	Setting::Setting(const Setting& copy) {
		items = map<string, SettingItem>();
		name = copy.name;

		for (auto i = copy.items.begin(); i != copy.items.end(); ++i) {
			items[i->first] = i->second;
		}
	}

	SettingItem Setting::GetItem(string key) {
		auto itm = items.find(key);
		if (itm != items.end()) {
			return itm->second;
		}
		return SettingItem();
	}

	void Setting::AddItem(string key, string value) {
		SettingItem newItem = SettingItem(key, value);
		items[key] = newItem;
	}

	void Setting::MergeItems(map<string, SettingItem>& newSet) {
		for (auto i = newSet.begin(); i != newSet.end(); ++i) {

			SettingItem copy = i->second;
			items[i->first] = copy;
		}
	}

	void Setting::LoadFromXml(spt<ofxXml> xml) {
		name = xml->getAttribute(":", "name", "");;

		int itemsNum = xml->getNumTags("item");

		for (int j = 0; j < itemsNum; j++) {
			xml->pushTag("item", j);

			SettingItem item = SettingItem();
			item.key = xml->getAttribute(":", "key", "");

			if (xml->attributeExists("value")) {
				// single string  <item key="abc" value="def" />
				item.AddValues(xml->getAttributex("value", ""));
			}
			else {
				/* more values:
				<item key="abc">
				<value>val1</value>
				<value>val2</value>
				</item>
				*/
				int values = xml->getNumTags("value");
				for (int m = 0; m < values; m++) {
					xml->pushTag("value", m);
					string val = xml->getValuex("");
					item.AddValues(val);
					xml->popTag();
				}
			}

			items[item.key] = item;
			xml->popTag();
		}

		// items can be declared as attributes, e.g. <setting name="myset" key1="value1" key2="value2" />
		vector<string> allAttributes;
		xml->getAttributeNames(":", allAttributes);

		for (string attr : allAttributes) {
			if (attr.compare("name") != 0) {
				string val = xml->getAttributex(attr, "");
				items[attr] = SettingItem(attr, val);
			}
		}
	}

	// ======================= Settings ===========================

	Settings::Settings(const Settings& copy) {
		settings = map<string, Setting>();

		for (auto i = copy.settings.begin(); i != copy.settings.end(); ++i) {
			settings[i->first] = i->second;
		}
	}

	void Settings::MergeSettings(map<string, Setting>& newSet) {
		for (auto i = newSet.begin(); i != newSet.end(); ++i) {
			auto existing = settings.find(i->first);

			if (existing != settings.end()) {
				Setting& existingSet = existing->second;
				Setting& newSet = i->second;

				// merge values
				existingSet.MergeItems(newSet.items);
			}
			else {
				Setting copy = i->second;
				settings[i->first] = copy;
			}
		}
	}

	void Settings::MergeSettings(Settings& newSet) {
		MergeSettings(newSet.settings);
	}

	void Settings::LoadFromXml(spt<ofxXml> xml) {

		settings = map<string, Setting>();

		int numOfSettings = xml->getNumTags("setting");

		for (int i = 0; i < numOfSettings; i++) {
			xml->pushTag("setting", i);
			auto set = Setting();
			set.LoadFromXml(xml);
			settings[set.name] = set;
			xml->popTag();
		}
	}

} // namespace