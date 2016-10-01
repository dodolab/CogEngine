#include "AssetsManager.h"
#include "Facade.h"

namespace Cog {

	void AssetsManager::OnInit() {
		string stringsPath = ofToDataPath(PATH_STRINGS);

		if (ofFile(stringsPath.c_str()).exists()) {
			ofxXml* xml = new ofxXml();
			xml->loadFile(stringsPath);

			if (xml->pushTag("resources")) {
				int stringsNum = xml->getNumTags("string");

				for (int i = 0; i < stringsNum; i++) {
					xml->pushTag("string", i);
					string key = xml->getAttributex("name", "");
					string value = xml->getValuex("");

					if (!key.empty() && !value.empty()) {
						strings[key] = value;
					}

					xml->popTag();
				}
			}

			delete xml;
		}

		string dimensionsPath = ofToDataPath(PATH_DIMENSIONS);

		if (ofFile(dimensionsPath.c_str()).exists()) {
			ofxXml* xml = new ofxXml();
			xml->loadFile(dimensionsPath);

			if (xml->pushTag("resources")) {
				int stringsNum = xml->getNumTags("dimen");

				for (int i = 0; i < stringsNum; i++) {
					xml->pushTag("dimen", i);
					string key = xml->getAttributex("name", "");
					string value = xml->getValuex("");

					if (!key.empty() && !value.empty()) {
						dimensions[key] = value;
					}

					xml->popTag();
				}
			}

			delete xml;
		}

		string globalSettings = ofToDataPath(PATH_GLOBAL_SETTINGS);

		if (ofFile(globalSettings.c_str()).exists()) {
			spt<ofxXml> xml = spt<ofxXml>(new ofxXml());
			xml->loadFile(globalSettings);

			if (xml->pushTag("resources")) {
				loadedGlobalSettings.LoadFromXml(xml);
			}
		}

		string defaultSettings = ofToDataPath(PATH_DEFAULT_SETTINGS);

		if (ofFile(defaultSettings.c_str()).exists()) {
			spt<ofxXml> xml = spt<ofxXml>(new ofxXml());
			xml->loadFile(defaultSettings);

			if (xml->pushTag("resources")) {
				loadedDefaultSettings.LoadFromXml(xml);
			}
		}

		string projectSettings = ofToDataPath(PATH_PROJECT_SETTINGS);

		if (ofFile(projectSettings.c_str()).exists()) {
			spt<ofxXml> xml = spt<ofxXml>(new ofxXml());
			xml->loadFile(projectSettings);

			if (xml->pushTag("resources")) {
				loadedProjectSettings.LoadFromXml(xml);
			}
		}
	}

	string AssetsManager::GetString(string key) {
		auto found = strings.find(key);
		if (found != strings.end()) {
			return found->second;
		}

		CogLogError("Resources", "String with key %s not found", key.c_str());

		return string();
	}

	string AssetsManager::GetDimension(string key) {
		auto found = dimensions.find(key);
		if (found != dimensions.end()) {
			return found->second;
		}

		CogLogError("Resources", "Dimension with key %s not found", key.c_str());

		return string();
	}

	int AssetsManager::GetInteger(string key) {
		auto found = integers.find(key);
		if (found != integers.end()) {
			return found->second;
		}

		CogLogError("Resources", "Integer with key %s not found", key.c_str());

		return 0;
	}

	Setting AssetsManager::GetDefaultSettings(string name) {
		return loadedDefaultSettings.GetSetting(name);
	}

	Setting AssetsManager::GetGlobalSettings(string name) {
		return loadedGlobalSettings.GetSetting(name);
	}

	Setting AssetsManager::GetProjectSettings(string name) {
		return loadedProjectSettings.GetSetting(name);
	}

}