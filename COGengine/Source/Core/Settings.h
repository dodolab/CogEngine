#pragma once

#include <map>
#include <string>

using std::map;
using std::string;

#include "Definitions.h"
#include "ofxXmlSettings.h"
#include "Utils.h"

namespace Cog {

	/**
	* Key-value(s) record, usually loaded from XML
	*/
	class SettingItem {
	private:
		// list of values 
		vector<string> values;

	public:
		string key = "";

// delimiter for multiple values in one string, spares space in XML because
// multiple values can be defined like this:  value="val1|val2|val3"
// another possibility is this:  <value>val1</value><value>val2</value>
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

		/**
		* Adds value(s), separated by | vertical line
		*/
		void AddValues(string vals);

		bool HasMoreValues() {
			return values.size() > 1;
		}

		vector<string>& GetValues() {
			return values;
		}

		/**
		* Gets the first value as a string (or returns empty, if there is no value)
		*/
		string GetValStr(string defaultVal = "");

		/**
		* Gets the first value as a double (or returns 0, if there is no value)
		*/
		double GetValDouble(double defaultVal = 0);

		/**
		* Gets the first value as an integer (or returns 0, if there is no value)
		*/
		int GetValInt(int defaultVal = 0);

		/**
		* Gets the first value as a float (or returns 0, if there is no value)
		*/
		float GetValFloat(float defaultVal = 0);

		/**
		* Gets the first value as a bool (or returns false, if there is no value)
		*/
		bool GetValBool(bool defaultVal = false);
	};

	/**
	* Collection of key-pair values
	*/
	class Setting {
	public:
		// values, mapped by their keys
		map<string, SettingItem> items;
		string name = "";

		Setting() {
		}

		Setting(string name) : name(name) {
		}

		Setting(const Setting& copy);

		/**
		* Gets item by key or an empty item if not found
		*/
		SettingItem GetItem(string key);

		/**
		* Returns name of this collection
		*/
		string GetName() const {
			return name;
		}

		/**
		* Gets the first value of an item with given key 
		* or returns empty string if there is no such item
		*/
		string GetItemVal(string key, string defaultVal = "") {
			return GetItem(key).GetValStr(defaultVal);
		}

		/**
		* Gets the first value of an item with given key
		* or returns 0 if there is no such item
		*/
		double GetItemValDouble(string key, double defaultVal = 0) {
			return GetItem(key).GetValDouble(defaultVal);
		}

		/**
		* Gets the first value of an item with given key
		* or returns 0 if there is no such item
		*/
		int GetItemValInt(string key, int defaultVal = 0) {
			return GetItem(key).GetValInt(defaultVal);
		}

		/**
		* Gets the first value of an item with given key
		* or returns 0 if there is no such item
		*/
		float GetItemValFloat(string key, float defaultVal = 0) {
			return GetItem(key).GetValFloat(defaultVal);
		}

		/**
		* Gets the first value of an item with given key
		* or returns false if there is no such item
		*/
		bool GetItemValBool(string key, bool defaultVal = false) {
			return GetItem(key).GetValBool(defaultVal);
		}

		/**
		* Gets the collection of values with given key
		*/
		vector<string> GetItemVals(string key) {
			return GetItem(key).GetValues();
		}

		/**
		* Adds a new key-value item
		*/
		void AddItem(string key, string value);
		
		/**
		* Merges collection of items (inserts those that hasn't been inserted yet)
		*/
		void MergeItems(map<string, SettingItem>& newSet);

		/**
		* Returns true, if there are no items
		*/
		bool Empty() {
			return items.empty();
		}

		/**
		* Loads collection key-value pairs from xml
		*/
		void LoadFromXml(spt<ofxXml> xml);
	};

	/**
	* Collection of settings for various components, loaded usually from XML
	* Each Settings entity contains collection of Setting with given name
	* Moreover, each Setting entity contains collection of key-value pairs
	*/
	class Settings {
	private:
		map<string, Setting> settings;

	public:
		Settings() {

		}

		Settings(map<string, Setting> settings) : settings(settings) {

		}

		Settings(const Settings& copy);

		/**
		* Gets value of a setting with given name and given key as a string.
		* If there is no such Setting or key-value pair, it returns empty string
		*/
		string GetSettingVal(string setName, string itemKey) {
			if (settings.count(setName) == 0) return "";
			else return GetSetting(setName).GetItemVal(itemKey);
		}

		/**
		* Gets value of a setting with given name and given key as an integer.
		* If there is no such Setting or key-value pair, it returns 0
		*/
		int GetSettingValInt(string setName, string itemKey) {
			if (settings.count(setName) == 0) return 0;
			else return GetSetting(setName).GetItem(itemKey).GetValInt();
		}

		/**
		* Gets value of a setting with given name and given key as a float.
		* If there is no such Setting or key-value pair, it returns 0
		*/
		float GetSettingValFloat(string setName, string itemKey) {
			if (settings.count(setName) == 0) return 0;
			else return GetSetting(setName).GetItem(itemKey).GetValFloat();
		}

		Setting& GetSetting(string name) {
			return settings[name];
		}

		/**
		* Sets setting by its name
		*/
		void SetSetting(string name, Setting& val) {
			settings[name] = val;
		}

		/**
		* Merges collection of settings (inserts those that hasn't been inserted yet)
		*/
		void MergeSettings(map<string, Setting>& newSet);
		
		/**
		* Merges collection of settings (inserts those that hasn't been inserted yet)
		*/
		void MergeSettings(Settings& newSet);

		/**
		* Loads settings from XML
		*/
		void LoadFromXml(spt<ofxXml> xml);
	};



} // namespace