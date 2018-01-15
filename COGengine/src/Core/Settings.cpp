#include "Settings.h"

namespace Cog {

	// ===================== SettingItem =========================

	void SettingItem::AddValues(string vals) {
		if (vals.find(MULTIVAL_DELIMITER) != -1) {
			// more than one value
			vector<string> multiVals; 
			split_string(vals, MULTIVAL_DELIMITER, multiVals);
			
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

	void Setting::LoadFromXml(xml_node& node) {
		name = node.attribute("name").as_string("");

		for (auto itemNode : node.children("item")) {
			
			SettingItem item = SettingItem();
			item.key = itemNode.attribute("key").as_string("");

			auto value = itemNode.attribute("value");

			if (value) {
				// single string  <item key="abc" value="def" />
				item.AddValues(value.as_string(""));
			}
			else {
				/* more xml elements:
				<item key="abc">
				<value>val1</value>
				<value>val2</value>
				</item>
				*/
				for (auto valueNode : itemNode.children("value")) {
					string val = valueNode.value();
					item.AddValues(val);
				}
			}

			items[item.key] = item;
		}

		// items may be declared as attributes, e.g. <setting name="myset" key1="value1" key2="value2" />
		for (auto attrNode : node.attributes()) {
			auto attrName = attrNode.name();
			if (string(attrName).compare("name") != 0) {
				string val = attrNode.value();
				items[attrName] = SettingItem(attrName, val);
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

	void Settings::LoadFromXml(xml_node& node) {

		settings = map<string, Setting>();

		for (auto settingNode : node.children("setting")) {
			auto set = Setting();
			set.LoadFromXml(settingNode);
			settings[set.name] = set;
		}
	}


	/**
	* Gets value of a setting with given name as a string.
	* If there is no such Setting or key-value pair, it returns empty string
	*/
	template<>
	string Settings::GetSettingVal<string>(string setName) {
		if (settings.count(setName) == 0) return "";
		else return GetSetting(setName).GetItemVal("value");
	}

	/**
	* Gets value of a setting of given name and given key as a string.
	* If there is no such Setting or key-value pair, it returns empty string
	*/
	template<>
	string Settings::GetSettingVal<string>(string setName, string itemKey) {
		if (settings.count(setName) == 0) return "";
		else return GetSetting(setName).GetItemVal(itemKey);
	}

	/**
	* Gets value of a setting with given name and given key as an integer.
	* If there is no such Setting or key-value pair, it returns 0
	*/
	template<>
	int Settings::GetSettingVal<int>(string setName, string itemKey) {
		if (settings.count(setName) == 0) return 0;
		else return GetSetting(setName).GetItem(itemKey).GetValInt();
	}

	/**
	* Gets value of a setting with given name and given key as a float.
	* If there is no such Setting or key-value pair, it returns 0
	*/
	template<>
	float Settings::GetSettingVal<float>(string setName, string itemKey) {
		if (settings.count(setName) == 0) return 0;
		else return GetSetting(setName).GetItem(itemKey).GetValFloat();
	}

	/**
	* Gets value of a setting with given name and given key as a boolean.
	* If there is no such Setting or key-value pair, it returns false
	*/
	template<>
	bool Settings::GetSettingVal<bool>(string setName, string itemKey) {
		if (settings.count(setName) == 0) return 0;
		else return GetSetting(setName).GetItem(itemKey).GetValBool();
	}

} // namespace