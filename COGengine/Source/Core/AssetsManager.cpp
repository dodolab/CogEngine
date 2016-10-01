#include "AssetsManager.h"
#include "Facade.h"
#include "ofTrueTypeFont.h"
#include "SpriteSheet.h"
#include "AnimLoader.h"
#include "TransformEnt.h"
#include "BehaviorEnt.h"

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

		string fonts = ofToDataPath(PATH_FONTS);

		if (ofFile(fonts.c_str()).exists()) {
			spt<ofxXml> xml = spt<ofxXml>(new ofxXml());
			xml->loadFile(fonts);

			if (xml->pushTag("resources")) {
				int fontNums = xml->getNumTags("font");

				for (int i = 0; i < fontNums; i++) {
					xml->pushTag("font", i);
					string name = xml->getAttributex("name", "");
					int size = xml->getAttributex("size", 1);
					GetFont(name, size);
					xml->popTag();
				}
			}
		}


		string animationsPath = ofToDataPath(PATH_ANIMATIONS);

		if (ofFile(animationsPath.c_str()).exists()) {
			spt<ofxXml> xml = spt<ofxXml>(new ofxXml());
			xml->loadFile(animationsPath);

			if (xml->pushTag("resources")) {
				auto animLoader = AnimLoader();
				auto rootAnims = vector<spt<GeneralAnim>>();

				auto builder = [](void) {
					return new SheetAnim();
				};

				animLoader.LoadAnimationsFromXml(xml, rootAnims, builder);

				// store animation
				for (spt<GeneralAnim> anim : rootAnims) {
					anim = static_pointer_cast<SheetAnim>(anim);
					StoreAnimation(anim);
				}
			}
		}

		string attrAnimationsPath = ofToDataPath(PATH_ATTRANIMATIONS);

		if (ofFile(attrAnimationsPath.c_str()).exists()) {
			spt<ofxXml> xml = spt<ofxXml>(new ofxXml());
			xml->loadFile(attrAnimationsPath);

			if (xml->pushTag("resources")) {
				int transNum = xml->getNumTags("transform");

				for (int i = 0; i < transNum; i++) {
					xml->pushTag("transform", i);
					spt<TransformEnt> trans = spt<TransformEnt>(new TransformEnt());
					trans->LoadFromXml(xml, GetDefaultSettings("transform"));

					COGASSERT(!trans->name.empty(), "Resources", "Transform entity on index %d in configuration file must have a name!", i);

					StoreEntity(trans->name, trans);
					xml->popTag();
				}
			}
		}

		string transformsPath = ofToDataPath(PATH_TRANSFORMS);

		if (ofFile(transformsPath.c_str()).exists()) {
			spt<ofxXml> xml = spt<ofxXml>(new ofxXml());
			xml->loadFile(transformsPath);

			if (xml->pushTag("resources")) {

			}
		}

		string behaviorsPath = ofToDataPath(PATH_BEHAVIORS);

		if (ofFile(behaviorsPath.c_str()).exists()) {
			spt<ofxXml> xml = spt<ofxXml>(new ofxXml());
			xml->loadFile(behaviorsPath);

			if (xml->pushTag("resources")) {
				int behNum = xml->getNumTags("behavior");

				for (int i = 0; i < behNum; i++) {
					xml->pushTag("behavior", i);
					spt<BehaviorEnt> ent = spt<BehaviorEnt>(new BehaviorEnt());

					auto dummySet = Setting();
					ent->LoadFromXml(xml, dummySet);

					COGASSERT(!ent->name.empty(), "Resources", "Behavior entity on index %d in configuration file must have a name!", i);

					StoreEntity(ent->name, ent);
					xml->popTag();
				}
			}
		}


		string spriteSheetsPath = ofToDataPath(PATH_SPRITESHEETS);

		if (ofFile(spriteSheetsPath.c_str()).exists()) {
			spt<ofxXml> xml = spt<ofxXml>(new ofxXml());
			xml->loadFile(spriteSheetsPath);

			if (xml->pushTag("resources")) {
				int sheetsNum = xml->getNumTags("spritesheet");

				for (int i = 0; i < sheetsNum; i++) {
					xml->pushTag("spritesheet", i);
					spt<SpriteSheet> spriteSheet = spt<SpriteSheet>(new SpriteSheet());
					spriteSheet->LoadFromXml(xml);
					this->StoreSpriteSheet(spriteSheet);
					xml->popTag();
				}
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

	spt<ofTrueTypeFont> AssetsManager::GetFont(string path, int size) {

		auto fontSetIt = loadedFonts.find(path);
		map<int, spt<ofTrueTypeFont>> fontSet;

		if (fontSetIt == loadedFonts.end()) {
			loadedFonts[path] = fontSet;
		}
		else {
			fontSet = fontSetIt->second;
		}

		COGLOGDEBUG("Resources", "Loading font %s", path.c_str());

		map<int, spt<ofTrueTypeFont>>& fs = loadedFonts.find(path)->second;

		auto font = (fs).find(size);
		if (font != fs.end()) {
			// return cached font
			return font->second;
		}
		else {
			ofTrueTypeFont* font = new ofTrueTypeFont();
			font->load(path, size, true, true);
			auto fontPtr = spt<ofTrueTypeFont>(font);
			fs[size] = fontPtr;
			return fontPtr;
		}
	}

	spt<SpriteSheet> AssetsManager::GetSpriteSheet(string name) {
		auto found = loadedSpriteSheets.find(name);
		if (found != loadedSpriteSheets.end()) return found->second;
		else return spt<SpriteSheet>();
	}

	void AssetsManager::StoreSpriteSheet(spt<SpriteSheet> spriteSheet) {
		COGASSERT(spriteSheet->GetName().compare("") != 0, "Resources", "Attempt to store spritesheet without a name!");

		auto found = loadedSpriteSheets.find(spriteSheet->GetName());
		if (found == loadedSpriteSheets.end()) {
			loadedSpriteSheets[spriteSheet->GetName()] = spriteSheet;
		}
	}

	spt<GeneralAnim> AssetsManager::GetAnimation(string name) {
		auto found = loadedAnimations.find(name);
		if (found != loadedAnimations.end()) return found->second;
		else return spt<GeneralAnim>();
	}

	void AssetsManager::StoreAnimation(spt<GeneralAnim> anim) {
		COGASSERT(anim->GetName().compare("") != 0, "Resources", "Attempt to store animation without a name!");

		auto found = loadedAnimations.find(anim->GetName());
		if (found == loadedAnimations.end()) {
			loadedAnimations[anim->GetName()] = anim;
		}
	}

	spt<DEntity> AssetsManager::GetEntity(string name) {
		auto found = loadedEntities.find(name);
		if (found != loadedEntities.end()) {
			return found->second;
		}
		else {
			return spt<DEntity>();
		}
	}

	void AssetsManager::StoreEntity(spt<DEntity> entity) {
		COGASSERT(loadedEntities.count(entity->name) == 0 || typeid(loadedEntities[entity->name]) == typeid(entity.get),
			"Resources", "Different type of entity %s is already in the cache!", entity->name.c_str());

		loadedEntities[entity->name] = entity;
	}

	void AssetsManager::StoreEntity(string name, spt<DEntity> entity) {
		COGASSERT(loadedEntities.count(entity->name) == 0 || typeid(loadedEntities[entity->name]) == typeid(entity.get),
			"Resources", "Different type of entity %s is already in the cache!", entity->name.c_str());

		loadedEntities[name] = entity;
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