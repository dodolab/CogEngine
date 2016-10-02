#pragma once

#include "Component.h"
#include "Settings.h"
#include "TransformEnt.h"
#include "GeneralAnim.h"

namespace Cog {

	class SpriteSheet;

	/**
	* Provides access to custom asset files and XML manifests
	*/
	class AssetsManager {
	private:
		map<string, string> strings;
		map<string, string> dimensions;
		map<string, int> integers;

		// cached sprite sheets
		map<string, spt<SpriteSheet>> loadedSpriteSheets;

		Settings loadedDefaultSettings;
		Settings loadedGlobalSettings;
		Settings loadedProjectSettings;

		// cached fonts (one for each dpi)
		map<string, map<int, spt<ofTrueTypeFont>>> loadedFonts;

		// cached animation
		map<string, spt<GeneralAnim>> loadedAnimations;
		// cached description entities
		map<string, spt<DEntity>> loadedEntities;

	public:

		void OnInit();

		/**
		* Gets string by its key
		*/
		string GetString(string key);

		/**
		* Gets dimension by its key
		*/
		string GetDimension(string key);

		/**
		* Gets integer by its key
		*/
		int GetInteger(string key);

		/**
		* Loads font from file
		* @param path path to file
		* @size font size in dpi
		*/
		spt<ofTrueTypeFont> GetFont(string path, int size);

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
		* Gets animation by name
		* @param name animation name
		*/
		spt<GeneralAnim> AssetsManager::GetAnimation(string name);

		/**
		* Stores animation
		* @param anim animation to store
		*/
		void StoreAnimation(spt<GeneralAnim> anim);

		/**
		* Gets stored entity by name
		*/
		spt<DEntity> GetEntity(string name);

		/**
		* Stores entity under its name
		* @param entity entity to store
		*/
		void StoreEntity(spt<DEntity> entity);

		/**
		* Stores entity
		* @param name entity name, has to be unique
		* @entity entity to store
		*/
		void StoreEntity(string name, spt<DEntity> entity);

		/**
		* Gets default settings that contains default values
		* for any component
		*/
		Settings& GetDefaultSettings() {
			return loadedDefaultSettings;
		}

		/**
		* Gets global settings that contains initialization
		* configuration such as logger settings, aspect ratio etc.
		*/
		Settings& GetGlobalSettings() {
			return loadedGlobalSettings;
		}

		/**
		* Gets custom project settings that may contain anything
		*/
		Settings& GetProjectSettings() {
			return loadedProjectSettings;
		}

		/**
		* Gets default setting by name of the parent group,
		* for example "transform"
		*/
		Setting GetDefaultSettings(string name);

		/**
		* Gets global setting by name of the parent group,
		* for example "logger"
		*/
		Setting GetGlobalSettings(string name);

		/**
		* Gets project setting by name of the parent group
		*/
		Setting GetProjectSettings(string name);

	private:
		spt<ofxXml> LoadResourcesXml(string path);
	};

} // namespace