#include "AssetsManager.h"
#include "Facade.h"
#include "ofTrueTypeFont.h"
#include "SpriteSheet.h"
#include "AnimLoader.h"
#include "TransformEnt.h"
#include "BehaviorEnt.h"
#include "AttribAnim.h"

namespace Cog {

	void AssetsManager::OnInit() {

		string stringsPath = ofToDataPath(PATH_STRINGS);
		auto xmlDoc = LoadResourcesXml(stringsPath);

		if (xmlDoc) {
			for (auto stringNode : xmlDoc->child("resources").children("string")) {
				string key = stringNode.attribute("name").as_string();
				string value = stringNode.value();

				if (!key.empty() && !value.empty()) {
					strings[key] = value;
				}
			}
		}

		string dimensionsPath = ofToDataPath(PATH_DIMENSIONS);
		xmlDoc = LoadResourcesXml(dimensionsPath);
		if (xmlDoc) {
			for (auto dimenNode : xmlDoc->child("resources").children("string")) {
				string key = dimenNode.attribute("name").as_string();
				string value = dimenNode.value();

				if (!key.empty() && !value.empty()) {
					dimensions[key] = value;
				}
			}
		}

		string globalSettings = ofToDataPath(PATH_GLOBAL_SETTINGS);
		xmlDoc = LoadResourcesXml(globalSettings);
		if (xmlDoc) {
			auto resources = xmlDoc->child("resources");
			loadedGlobalSettings.LoadFromXml(resources);
		}

		string defaultSettings = ofToDataPath(PATH_DEFAULT_SETTINGS);
		xmlDoc = LoadResourcesXml(defaultSettings);
		if (xmlDoc) {
			auto resources = xmlDoc->child("resources");
			loadedDefaultSettings.LoadFromXml(resources);
		}

		string projectSettings = ofToDataPath(PATH_PROJECT_SETTINGS);
		xmlDoc = LoadResourcesXml(projectSettings);
		if (xmlDoc) {
			auto resources = xmlDoc->child("resources");
			loadedProjectSettings.LoadFromXml(resources);
		}

		string animationsPath = ofToDataPath(PATH_ANIMATIONS);
		xmlDoc = LoadResourcesXml(animationsPath);
		if (xmlDoc) {
			auto animLoader = AnimLoader();
			auto rootAnims = vector<spt<GeneralAnim>>();

			auto builder = [](void) {
				return new SheetAnim();
			};

			auto resources = xmlDoc->child("resources");
			animLoader.LoadAnimationsFromXml(resources, rootAnims, builder);


			// store animation
			for (spt<GeneralAnim> anim : rootAnims) {
				anim = static_pointer_cast<SheetAnim>(anim);
				StoreAnimation(anim);
			}
		}


		string attrAnimationsPath = ofToDataPath(PATH_ATTRANIMATIONS);
		xmlDoc = LoadResourcesXml(attrAnimationsPath);
		if (xmlDoc) {
			auto animLoader = AnimLoader();
			auto rootAnims = vector<spt<GeneralAnim>>();

			auto builder = [](void) {
				return new AttribAnim();
			};

			auto resources = xmlDoc->child("resources");
			animLoader.LoadAnimationsFromXml(resources, rootAnims, builder);

			// store animation
			for (spt<GeneralAnim> anim : rootAnims) {
				anim = static_pointer_cast<AttribAnim>(anim);
				StoreAnimation(anim);
			}
		}

		string transformsPath = ofToDataPath(PATH_TRANSFORMS);
		xmlDoc = LoadResourcesXml(transformsPath);
		if (xmlDoc) {
			auto resources = xmlDoc->child("resources");
			int indexCtr = 0;
			for (auto transNode : resources.children("transform")) {
				spt<TransformEnt> trans = spt<TransformEnt>(new TransformEnt());
				Setting defaultSet = GetDefaultSettings("transform");
				trans->LoadFromXml(transNode, defaultSet);

				COGASSERT(!trans->name.empty(), "Resources", "Transform entity on index %d in configuration file must have a name!", indexCtr);

				StoreEntity(trans->name, trans);
				indexCtr++;
			}
		}


		string behaviorsPath = ofToDataPath(PATH_BEHAVIORS);
		xmlDoc = LoadResourcesXml(behaviorsPath);
		if (xmlDoc) {
			auto resources = xmlDoc->child("resources");
			int indexCtr = 0;
			for (auto behNode : resources.children("behavior")) {
				spt<BehaviorEnt> ent = spt<BehaviorEnt>(new BehaviorEnt());

				auto dummySet = Setting();
				ent->LoadFromXml(behNode, dummySet);

				COGASSERT(!ent->name.empty(), "Resources", "Behavior entity on index %d in configuration file must have a name!", indexCtr);

				StoreEntity(ent->name, ent);
				indexCtr++;
			}
		}

		string spriteSheetsPath = ofToDataPath(PATH_SPRITESHEETS);
		xmlDoc = LoadResourcesXml(spriteSheetsPath);
		if (xmlDoc) {
			auto resources = xmlDoc->child("resources");

			for (auto sheetNode : resources.children("spritesheet")) {
				spt<SpriteSheet> spriteSheet = spt<SpriteSheet>(new SpriteSheet());
				spriteSheet->LoadFromXml(sheetNode);
				this->StoreSpriteSheet(spriteSheet);
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


	spt<xml_document> AssetsManager::LoadResourcesXml(string path) {
		if (ofFile(path.c_str()).exists()) {
			spt<xml_document> xml = spt<xml_document>(new xml_document());
			xml->load_file(path.c_str());
			return xml;
		}

		return spt<xml_document>();
	}
}