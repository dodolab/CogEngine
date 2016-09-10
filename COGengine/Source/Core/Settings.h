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

		void AddValues(string vals);

		bool HasMoreValues() {
			return values.size() > 1;
		}

		vector<string>& GetValues() {
			return values;
		}

		string GetValStr(string defaultVal = "");

		double GetValDouble(double defaultVal = 0);

		int GetValInt(int defaultVal = 0);

		float GetValFloat(float defaultVal = 0);

		bool GetValBool(bool defaultVal = false);
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

		Setting(const Setting& copy);

		/**
		* Gets item by key or an item with empty values, if there is no such item
		*/
		SettingItem GetItem(string key);

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

		void AddItem(string key, string value);
		
		/**
		* Merges collection of items, adds not presented and replaces already stored
		*/
		void MergeItems(map<string, SettingItem>& newSet);

		bool Empty() {
			return items.empty();
		}

		void LoadFromXml(spt<ofxXml> xml);
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

		Settings(const Settings& copy);

		string GetSettingVal(string setKey, string itemKey) {
			if (settings.count(setKey) == 0) return "";
			else return GetSetting(setKey).GetItemVal(itemKey);
		}

		int GetSettingValInt(string setKey, string itemKey) {
			if (settings.count(setKey) == 0) return 0;
			else return GetSetting(setKey).GetItem(itemKey).GetValInt();
		}

		float GetSettingValFloat(string setKey, string itemKey) {
			if (settings.count(setKey) == 0) return 0;
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
		void MergeSettings(map<string, Setting>& newSet);
		
		void MergeSettings(Settings& newSet);

		void LoadFromXml(spt<ofxXml> xml);
	};



} // namespace