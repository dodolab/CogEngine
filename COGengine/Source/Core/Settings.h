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
	public:

		SettingItem() {

		}

		SettingItem(const SettingItem& copy) {
			key = copy.key;

			for (string val : copy.values) {
				values.push_back(val);
			}
		}

		/** key */
		string key = "";

		/** list of values */
		vector<string> values;

		bool HasMoreValues() {
			return values.size() > 1;
		}

		vector<string>& GetValues() {
			return values;
		}

		string GetValStr() {
			if (values.size() == 0) return "";
			return values[0];
		}

		double GetValDouble() {
			if (values.size() == 0) return 0;
			return ofToDouble(values[0]);
		}

		int GetValInt() {
			if (values.size() == 0) return 0;
			return ofToInt(values[0]);
		}

		float GetValFloat() {
			if (values.size() == 0) return 0;
			return ofToFloat(values[0]);
		}

		bool GetValBool() {
			if (values.size() == 0) return false;
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

		string GetItemVal(string key) {
			return GetItem(key).GetValStr();
		}

		double GetItemValDouble(string key) {
			return GetItem(key).GetValDouble();
		}

		int GetItemValInt(string key) {
			return GetItem(key).GetValInt();
		}

		float GetItemValFloat(string key) {
			return GetItem(key).GetValFloat();
		}

		bool GetItemValBool(string key) {
			return GetItem(key).GetValBool();
		}

		vector<string> GetItemVals(string key) {
			return GetItem(key).values;
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
			return GetSetting(setKey).GetItemVal(itemKey);
		}

		int GetSettingValInt(string setKey, string itemKey) {
			return GetSetting(setKey).GetItem(itemKey).GetValInt();
		}

		float GetSettingValFloat(string setKey, string itemKey) {
			return GetSetting(setKey).GetItem(itemKey).GetValFloat();
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
	};



} // namespace