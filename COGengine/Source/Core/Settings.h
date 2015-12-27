#pragma once

#include "ofxCogMain.h"

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
			value = copy.value;
		}

		/** key */
		string key = "";

		/** string value */
		string value = "";


		string GetValStr() {
			return value;
		}

		double GetValDouble() {
			return ofToDouble(value);
		}

		int GetValInt() {
			return ofToInt(value);
		}

		float GetValFloat() {
			return ofToFloat(value);
		}

		bool GetValBool() {
			return ofToBool(value);
		}
	};

	/**
	* Collection of key-pair values
	*/
	class Setting {
	public:
		map<string, SettingItem> items;
		string name;

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
			return GetItem(key).value;
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
	};



} // namespace