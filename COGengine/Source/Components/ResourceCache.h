#pragma once

#include "ofxCogMain.h"
#include "SoundFile.h"
#include "Anim.h"
#include "SpriteSheet.h"

namespace Cog {

	class SettingItem {
	public:
		string key = "";
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

	class Setting {
	public:
		map<string, SettingItem> items;
		string name;

		Setting() {
			items = map<string, SettingItem>();
		}

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
	};


	/**
	* Resource controller that holds images, 3D objects and sounds
	*/
	class ResourceCache : public Component {

		COMPONENT(ResourceCache)

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

		map<string, Setting> LoadSettingsFromXml(spt<ofxXml> xml);
	};

}// namespace