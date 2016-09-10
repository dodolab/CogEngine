#pragma once

#include "ofxCogMain.h"
#include "SoundFile.h"
#include "Anim.h"
#include "SpriteSheet.h"
#include "Component.h"

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

	/**
	* Resource controller that holds images, 3D objects and sounds
	*/
	class ResourceCache : public Component {

		OBJECT(ResourceCache)

	private:
		// cached images
		map<string, spt<ofImage>> loadedImages;
		// cached meshes
		map<string, spt<ofVboMesh>> loadedMeshes;
		// cached sounds and music
		map<string, spt<Sound>> loadedSounds;
		// cached xml files
		map<string, spt<ofxXmlSettings>> loadedXMLs;
		// cached animation
		map<string, spt<Anim>> loadedAnimations;
		// cached spritesheets
		map<string, spt<SpriteSheet>> loadedSpriteSheets;
		// loaded fonts (each DPI must have one font loaded)
		map<string, map<int,spt<ofTrueTypeFont>>> loadedFonts;

		map<string, Setting> loadedDefaultSettings;
		map<string, Setting> loadedGlobalSettings;
		map<string, Setting> loadedProjectSettings;

	public:

		/**
		* Initializes controller
		*/
		void Init();

		void Init(spt<ofxXml> xml);

		/**
		* Loads 2D image from file
		* @param path path to file
		*/
		spt<ofImage> Get2DImage(string path);

		/**
		* Preloads 2D image from file; doesn't use texture so before first render,
		* the image must be copied onto texture; very usefull for ofxThread
		* @param path path to file
		*/
		spt<ofImage> Preload2DImage(string path);

		/**
		* Loads mesh from file
		* @param path path to file
		*/
		spt<ofVboMesh> GetMesh(string path);

		/**
		* Loads font from file
		* @param path path to file
		* @size font size
		*/
		spt<ofTrueTypeFont> GetFont(string path, int size);

		/**
		* Loads sound from file
		* @param path path to file
		*/
		spt<Sound> GetSound(string path);

		/**
		* Preloads xml file; if the file is in the cache, returns cached xml
		* @param path path to file
		*/
		spt<ofxXmlSettings> PreloadXMLFile(string path);

		/**
		* Loads xml file
		* @param path path to file
		*/
		spt<ofxXmlSettings> LoadXMLFile(string path);

		/**
		* Gets animation
		* @param name animation name
		*/
		spt<Anim> GetAnimation(string name);

		/**
		* Stores animation
		* @param anim animation to store
		*/
		void StoreAnimation(spt<Anim> anim);

		/**
		* Gets spritesheet by name
		* @param name name of spritesheet
		*/
		spt<SpriteSheet> GetSpriteSheet(string name);

		/**
		* Stores spritesheet
		* @param spriteSheet spritesheet to store
		*/
		void StoreSpriteSheet(spt<SpriteSheet> spriteSheet);

		/**
		* Loads settings from XML
		*/
		map<string, Setting> LoadSettingsFromXml(spt<ofxXml> xml);
	};

}// namespace