#include "ResourceCache.h"
#include "SoundFile.h"
#include "Anim.h"
#include "SpriteSheet.h"
#include "Facade.h"
#include "AnimationLoader.h"
#include "TransformEnt.h"
#include "NodeBuilder.h"
#include "BehaviorEnt.h"

namespace Cog {

	void ResourceCache::Init() {
		// set global dpi so font will have the same pixel size on each display
		ofTrueTypeFont::setGlobalDpi((int)(CogGetScreenWidth() * 72 * 0.001f));
	}

	void ResourceCache::Init(spt<ofxXml> xml) {
		xml->popAll();

		if (xml->tagExists("app_config")) {
			xml->pushTag("app_config");

			if (xml->pushTagIfExists("settings")) {

				// load default settings
				if (xml->pushTagIfExists("default_settings")) {
					loadedDefaultSettings.LoadFromXml(xml);
					xml->popTag();
				}

				// load global settings
				if (xml->pushTagIfExists("global_settings")) {
					loadedGlobalSettings.LoadFromXml(xml);
					xml->popTag();
				}

				// load project settings
				if (xml->pushTagIfExists("project_settings")) {
					loadedProjectSettings.LoadFromXml(xml);
					xml->popTag();
				}
			
				xml->popTag();
			}

			// preload fonts
			if (xml->pushTagIfExists("fonts")) {
				int fontNums = xml->getNumTags("font");

				for (int i = 0; i < fontNums; i++) {
					xml->pushTag("font", i);
					string name = xml->getAttributex("name", "");
					int size = xml->getAttributex("size", 0);
					CogGetFont(name, size);
					xml->popTag();
				}

				xml->popTag();
			}

			// load animations
			if (xml->pushTagIfExists("animations")) {

				auto animLoader = AnimationLoader();
				auto rootAnims = vector<spt<Anim>>();
				animLoader.LoadAnimationsFromXml(xml, rootAnims);

				// store animation
				for (spt<Anim> anim : rootAnims) {
					StoreAnimation(anim);
				}

				xml->popTag();
			}

			// load transformation entities
			if (xml->pushTagIfExists("transforms")) {
				int transNum = xml->getNumTags("transform");

				for (int i = 0; i < transNum; i++) {
					xml->pushTag("transform", i);
					spt<TransformEnt> trans = spt<TransformEnt>(new TransformEnt());
					trans->LoadFromXml(xml, loadedDefaultSettings.GetSetting("transform"));

					COGASSERT(!trans->name.empty(),"RESOURCE","Transform entity on index %d in configuration file must have a name!", i);

					StoreEntity(trans->name, trans);
					xml->popTag();
				}
				xml->popTag();
			}

			// load behaviors
			if (xml->pushTagIfExists("behaviors")) {
				int behNum = xml->getNumTags("behavior");

				for (int i = 0; i < behNum; i++) {
					xml->pushTag("behavior", i);
					spt<BehaviorEnt> ent = spt<BehaviorEnt>(new BehaviorEnt());

					ent->LoadFromXml(xml, Setting());

					COGASSERT(!ent->name.empty(), "RESOURCE", "Behavior entity on index %d in configuration file must have a name!", i);

					StoreEntity(ent->name, ent);
					xml->popTag();
				}

				xml->popTag();
			}

			// load spritesheets
			if (xml->pushTagIfExists("spritesheets")) {
				int sheetsNum = xml->getNumTags("spritesheet");

				for (int i = 0; i < sheetsNum; i++) {
					xml->pushTag("spritesheet", i);
					spt<SpriteSheet> spriteSheet = spt<SpriteSheet>(new SpriteSheet());
					spriteSheet->LoadFromXml(xml);
					this->StoreSpriteSheet(spriteSheet);
					xml->popTag();
				}

				xml->popTag();
			}
		}
	}

	spt<ofImage> ResourceCache::Get2DImage(string path) {

		auto found = loadedImages.find(path);
		if (found != loadedImages.end()) {
			spt<ofImage> existing = (found->second);
			if (!existing->isUsingTexture()) {
				// for preloaded images, they need to be copied to the texture
				existing->setUseTexture(true);
				//existing->reloadTexture();
			}

			return existing;
		}



		ofImage* img = new ofImage(path);
		ofVboMesh* mesh = new ofVboMesh();
		mesh->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
		mesh->addVertex(ofVec3f(-img->getWidth() / 2, -img->getHeight() / 2, 1));
		mesh->addVertex(ofVec3f(img->getWidth() / 2, -img->getHeight() / 2, 1));
		mesh->addVertex(ofVec3f(-img->getWidth() / 2, img->getHeight() / 2, 1));
		mesh->addVertex(ofVec3f(img->getWidth() / 2, img->getHeight() / 2, 1));

		// todo: for some devices (android) it has to be normalized
		mesh->addTexCoord(ofVec2f(0, 0));
		mesh->addTexCoord(ofVec2f(img->getWidth(), 0));
		mesh->addTexCoord(ofVec2f(0, img->getHeight()));
		mesh->addTexCoord(ofVec2f(img->getWidth(), img->getHeight()));

		auto image = spt<ofImage>(img);
		auto meshPtr = spt<ofVboMesh>(mesh);

		loadedImages[path] = image;
		loadedMeshes[path] = meshPtr;


		return image;
	}

	spt<ofImage> ResourceCache::Preload2DImage(string path) {
		auto found = loadedImages.find(path);
		if (found != loadedImages.end()) {
			return (found->second);
		}

		ofImage* img = new ofImage();
		// don't use texture because images are loaded in separate thread
		img->setUseTexture(false);
		bool loaded = img->load(path);
		if (!loaded) CogLogError("Image couldn't be loaded: %s", path.c_str());


		auto image = spt<ofImage>(img);
		loadedImages[path] = image;

		return image;
	}

	spt<ofVboMesh> ResourceCache::GetMesh(string path) {
		auto found = loadedMeshes.find(path);
		if (found != loadedMeshes.end()) {
			return (found->second);
		}
		return spt<ofVboMesh>();
	}

	spt<ofTrueTypeFont> ResourceCache::GetFont(string path, int size) {

		auto fontSetIt = loadedFonts.find(path);
		map<int, spt<ofTrueTypeFont>> fontSet;

		if (fontSetIt == loadedFonts.end()) {
			fontSet = map<int, spt<ofTrueTypeFont>>();
			loadedFonts[path] = fontSet;
		}
		else {
			fontSet = fontSetIt->second;
		}

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

	spt<Sound> ResourceCache::GetSound(string path) {
		auto found = loadedSounds.find(path);
		if (found != loadedSounds.end()) {
			return (found->second);
		}

		Sound* snd = new Sound(path);
		return spt<Sound>(snd);
	}

	spt<ofxXmlSettings> ResourceCache::PreloadXMLFile(string path) {
		auto found = loadedXMLs.find(path);
		if (found != loadedXMLs.end()) {
			return (found->second);
		}

		ofxXmlSettings* xml = new ofxXmlSettings();
		xml->loadFile(path);
		auto xmlPtr = spt<ofxXmlSettings>(xml);
		loadedXMLs[path] = xmlPtr;
		return xmlPtr;
	}

	spt<ofxXmlSettings> ResourceCache::LoadXMLFile(string path) {
		ofxXmlSettings* xml = new ofxXmlSettings();
		xml->loadFile(path);
		auto xmlPtr = spt<ofxXmlSettings>(xml);
		return xmlPtr;
	}

	spt<Anim> ResourceCache::GetAnimation(string name) {
		auto found = loadedAnimations.find(name);
		if (found != loadedAnimations.end()) return found->second;
		else return spt<Anim>();
	}

	void ResourceCache::StoreAnimation(spt<Anim> anim) {
		COGASSERT(anim->GetName().compare("") != 0, "Cache", "Attempt to store animation without a name!");

		auto found = loadedAnimations.find(anim->GetName());
		if (found == loadedAnimations.end()) {
			loadedAnimations[anim->GetName()] = anim;
		}
	}

	spt<DEntity> ResourceCache::GetEntity(string name) {
		auto found = loadedEntities.find(name);
		if (found != loadedEntities.end()) {
			return found->second;
		}
		else {
			return spt<DEntity>();
		}
	}

	void ResourceCache::StoreEntity(spt<DEntity> entity) {
		loadedEntities[entity->name] = entity;
	}

	void ResourceCache::StoreEntity(string name, spt<DEntity> entity) {
		loadedEntities[name] = entity;
	}

	spt<SpriteSheet> ResourceCache::GetSpriteSheet(string name) {
		auto found = loadedSpriteSheets.find(name);
		if (found != loadedSpriteSheets.end()) return found->second;
		else return spt<SpriteSheet>();
	}

	void ResourceCache::StoreSpriteSheet(spt<SpriteSheet> spriteSheet) {
		COGASSERT(spriteSheet->GetName().compare("") != 0, "Cache", "Attempt to store spritesheet without a name!");

		auto found = loadedSpriteSheets.find(spriteSheet->GetName());
		if (found == loadedSpriteSheets.end()) {
			loadedSpriteSheets[spriteSheet->GetName()] = spriteSheet;
		}
	}

	Setting ResourceCache::GetDefaultSettings(string name) {
		return loadedDefaultSettings.GetSetting(name);
	}

	Setting ResourceCache::GetGlobalSettings(string name) {
		return loadedGlobalSettings.GetSetting(name);
	}

	Setting ResourceCache::GetProjectSettings(string name) {
		return loadedProjectSettings.GetSetting(name);
	}

}// namespace