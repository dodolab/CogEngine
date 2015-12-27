#include "ResourceCache.h"
#include <string>
#include "ofImage.h"
#include "Facade.h"
#include "SoundFile.h"

namespace Cog {

	void ResourceCache::Init() {
		// set global dpi so font will have the same pixel size on each display
		ofTrueTypeFont::setGlobalDpi(CogGetScreenWidth() * 72 * 0.001f);
	}

	void ResourceCache::Init(spt<ofxXml> xml) {
		xml->popAll();

		if (xml->tagExists("app_config")) {
			xml->pushTag("app_config");

			if (xml->tagExists("settings")) {
				xml->pushTag("settings");

				// parse default settings
				if (xml->tagExists("default_settings")) {
					xml->pushTag("default_settings");
					loadedDefaultSettings = LoadSettingsFromXml(xml);
					xml->popTag();
				}

				// parse global settings
				if (xml->tagExists("global_settings")) {
					xml->pushTag("global_settings");
					loadedGlobalSettings = LoadSettingsFromXml(xml);
					xml->popTag();
				}

				// parse project settings
				if (xml->tagExists("project_settings")) {
					xml->pushTag("project_settings");
					loadedProjectSettings = LoadSettingsFromXml(xml);
					xml->popTag();
				}
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
		bool loaded = img->loadImage(path);
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
			font->loadFont(path, size, true, true);
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
		MASSERT(anim->GetName().compare("") != 0, "Cache", "Attempt to store animation without a name!");

		auto found = loadedAnimations.find(anim->GetName());
		if (found == loadedAnimations.end()) {
			loadedAnimations[anim->GetName()] = anim;
		}
	}

	spt<SpriteSheet> ResourceCache::GetSpriteSheet(string name) {
		auto found = loadedSpriteSheets.find(name);
		if (found != loadedSpriteSheets.end()) return found->second;
		else return spt<SpriteSheet>();
	}

	void ResourceCache::StoreSpriteSheet(spt<SpriteSheet> spriteSheet) {
		MASSERT(spriteSheet->GetName().compare("") != 0, "Cache", "Attempt to store spritesheet without a name!");

		auto found = loadedSpriteSheets.find(spriteSheet->GetName());
		if (found == loadedSpriteSheets.end()) {
			loadedSpriteSheets[spriteSheet->GetName()] = spriteSheet;
		}
	}

	map<string, Setting> ResourceCache::LoadSettingsFromXml(spt<ofxXml> xml) {
		map<string, Setting> setMap = map<string, Setting>();

		int numOfSettings = xml->getNumTags("setting");

		for (int i = 0; i < numOfSettings; i++) {
			xml->pushTag("setting", i);

			Setting set = Setting();
			set.name = xml->getAttribute(":", "name", "");;

			int items = xml->getNumTags("item");

			for (int j = 0; j < items; j++) {
				xml->pushTag("item", j);

				SettingItem item = SettingItem();
				item.key = xml->getAttribute(":", "key", "");
				item.value = xml->getAttribute(":", "value", "");
				set.items[item.key] = item;
				xml->popTag();
			}

			setMap[set.name] = set;
			xml->popTag();
		}

		return setMap;
	}

	Setting ResourceCache::GetDefaultSettings(string name) {
		auto fnd = loadedDefaultSettings.find(name);

		if (fnd != loadedDefaultSettings.end()) return fnd->second;
		else return Setting();
	}

	Setting ResourceCache::GetGlobalSettings(string name) {
		auto fnd = loadedGlobalSettings.find(name);

		if (fnd != loadedGlobalSettings.end()) return fnd->second;
		else return Setting();
	}

	Setting ResourceCache::GetProjectSettings(string name) {
		auto fnd = loadedProjectSettings.find(name);

		if (fnd != loadedProjectSettings.end()) return fnd->second;
		else return Setting();
	}

}// namespace