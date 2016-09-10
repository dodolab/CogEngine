#pragma once

#include <map>
#include <string>

using std::map;
using std::string;

namespace Cog {

	/**
	* Entity that holds key-value record, loaded usually from XML
	*/
	class SettingItem {
	private:
		/** list of values */
		vector<string> values;

	public:
		/** key */
		string key = "";


#define MULTIVAL_DELIMITER "|"

		SettingItem() {

		}

		SettingItem(string key) :key(key) {

		}

		SettingItem(string key, string vals) :key(key) {
			AddValues(vals);
		}

		SettingItem(const SettingItem& copy) {
			key = copy.key;

			for (string val : copy.values) {
				values.push_back(val);
			}
		}

		void AddValues(string vals) {
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

		bool HasMoreValues() {
			return values.size() > 1;
		}

		vector<string>& GetValues() {
			return values;
		}

		string GetValStr(string defaultVal = "") {
			if (values.size() == 0) return defaultVal;
			return values[0];
		}

		double GetValDouble(double defaultVal = 0) {
			if (values.size() == 0) return defaultVal;
			return ofToDouble(values[0]);
		}

		int GetValInt(int defaultVal = 0) {
			if (values.size() == 0) return defaultVal;
			return ofToInt(values[0]);
		}

		float GetValFloat(float defaultVal = 0) {
			if (values.size() == 0) return defaultVal;
			return ofToFloat(values[0]);
		}

		bool GetValBool(bool defaultVal = false) {
			if (values.size() == 0) return defaultVal;
			return ofToBool(values[0]);
		}
	};

	/**
	* Collection of key-pair values
	*/
	class Setting {
	public:
		map<string, SettingItem> items;
		string name = "";

		Setting() {
			items = map<string, SettingItem>();
		}

		Setting(const Setting& copy) {
			items = map<string, SettingItem>();
			name = copy.name;

			for (auto i = copy.items.begin(); i != copy.items.end(); ++i) {
				items[i->first] = i->second;
			}
		}

		/**
		* Gets item by key or an item with empty values, if there is no such item
		*/
		SettingItem GetItem(string key) {
			auto itm = items.find(key);
			if (itm != items.end()) {
				return itm->second;
			}
			return SettingItem();
		}

		string GetItemVal(string key, string defaultVal = "") {
			return GetItem(key).GetValStr(defaultVal);
		}

		double GetItemValDouble(string key, double defaultVal = 0) {
			return GetItem(key).GetValDouble(defaultVal);
		}

		int GetItemValInt(string key, int defaultVal = 0) {
			return GetItem(key).GetValInt(defaultVal);
		}

		float GetItemValFloat(string key, float defaultVal = 0) {
			return GetItem(key).GetValFloat(defaultVal);
		}

		bool GetItemValBool(string key, bool defaultVal = false) {
			return GetItem(key).GetValBool(defaultVal);
		}

		vector<string> GetItemVals(string key) {
			return GetItem(key).GetValues();
		}

		void AddItem(string key, string value) {
			SettingItem newItem = SettingItem(key, value);
			items[key] = newItem;
		}

		/**
		* Merges collection of items, adds not presented and replaces already stored
		*/
		void MergeItems(map<string, SettingItem>& newSet) {
			for (auto i = newSet.begin(); i != newSet.end(); ++i) {

				SettingItem copy = i->second;
				items[i->first] = copy;
			}
		}

		bool Empty() {
			return items.empty();
		}

		void LoadFromXml(spt<ofxXml> xml) {
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
		}
	};

	/**
	* Collection of settings for various components, loaded usually from XML
	*/
	class Settings {
	private:
		map<string, Setting> settings;

	public:
		Settings() {

		}

		Settings(map<string, Setting> settings) : settings(settings) {

		}

		Settings(const Settings& copy) {
			settings = map<string, Setting>();

			for (auto i = copy.settings.begin(); i != copy.settings.end(); ++i) {
				settings[i->first] = i->second;
			}
		}

		string GetSettingVal(string setKey, string itemKey) {
			if (settings.find(setKey) == settings.end()) return "";
			else return GetSetting(setKey).GetItemVal(itemKey);
		}

		int GetSettingValInt(string setKey, string itemKey) {
			if (settings.find(setKey) == settings.end()) return 0;
			else return GetSetting(setKey).GetItem(itemKey).GetValInt();
		}

		float GetSettingValFloat(string setKey, string itemKey) {
			if (settings.find(setKey) == settings.end()) return 0;
			else return GetSetting(setKey).GetItem(itemKey).GetValFloat();
		}

		Setting& GetSetting(string key) {
			return settings[key];
		}

		void SetSetting(string key, Setting val) {
			settings[key] = val;
		}

		/**
		* Merges collection of settings, adds not presented and replaces already stored
		*/
		void MergeSettings(map<string, Setting>& newSet) {
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

		void MergeSettings(Settings& newSet) {
			MergeSettings(newSet.settings);
		}

		void LoadFromXml(spt<ofxXml> xml) {
			
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
	};



} // namespace