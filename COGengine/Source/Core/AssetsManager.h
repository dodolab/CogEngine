#pragma once

#include "Component.h"
#include "Settings.h"

namespace Cog {

	/**
	* Provides access to custom asset files and XML manifests
	*/
	class AssetsManager {
	private:
		map<string, string> strings;
		map<string, string> dimensions;
		map<string, int> integers;

		Settings loadedDefaultSettings;
		Settings loadedGlobalSettings;
		Settings loadedProjectSettings;
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
	};

} // namespace