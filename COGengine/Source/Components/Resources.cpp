#include "Resources.h"
#include "Soundfx.h"
#include "SheetAnim.h"
#include "SpriteSheet.h"
#include "Facade.h"
#include "AnimLoader.h"
#include "TransformEnt.h"
#include "NodeBuilder.h"
#include "BehaviorEnt.h"
#include "AttrAnimEnt.h"
#include "AttribAnim.h"

namespace Cog {

	void Resources::OnInit() {

		// set global dpi and therefore fonts will have the same pixel size on each display
		ofTrueTypeFont::setGlobalDpi((int)(CogGetScreenWidth() * 72 * 0.001f));

		// initialize assets manager
		assetsMgr.OnInit();
	}

	spt<ofImage> Resources::Get2DImage(string path) {

		auto found = loadedImages.find(path);
		if (found != loadedImages.end()) {
			spt<ofImage> existing = (found->second);
			if (!existing->isUsingTexture()) {
				// for preloaded images, they need to be copied to the texture
				existing->setUseTexture(true);
			}

			return existing;
		}

		COGLOGDEBUG("Resources", "Loading image %s", path.c_str());

		ofImage* img = new ofImage(path);
		ofVboMesh* mesh = new ofVboMesh();
		mesh->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
		mesh->addVertex(ofVec3f(-img->getWidth() / 2, -img->getHeight() / 2, 1));
		mesh->addVertex(ofVec3f(img->getWidth() / 2, -img->getHeight() / 2, 1));
		mesh->addVertex(ofVec3f(-img->getWidth() / 2, img->getHeight() / 2, 1));
		mesh->addVertex(ofVec3f(img->getWidth() / 2, img->getHeight() / 2, 1));

#ifdef WIN32
		mesh->addTexCoord(ofVec2f(0, 0));
		mesh->addTexCoord(ofVec2f(img->getWidth(), 0));
		mesh->addTexCoord(ofVec2f(0, img->getHeight()));
		mesh->addTexCoord(ofVec2f(img->getWidth(), img->getHeight()));

#else
		// for android, texture coordinates are in range 0-1
		mesh->addTexCoord(ofVec2f(0, 0));
		mesh->addTexCoord(ofVec2f(1, 0));
		mesh->addTexCoord(ofVec2f(0, 1));
		mesh->addTexCoord(ofVec2f(1, 1));

#endif

		auto image = spt<ofImage>(img);
		auto meshPtr = spt<ofVboMesh>(mesh);

		loadedImages[path] = image;
		loadedVboMeshes[path] = meshPtr;

		return image;
	}

	spt<ofImage> Resources::Preload2DImage(string path) {
		auto found = loadedImages.find(path);
		if (found != loadedImages.end()) {
			return (found->second);
		}

		COGLOGDEBUG("Resources", "Preloading image %s", path.c_str());
		ofImage* img = new ofImage();
		// don't use texture because images are loaded in separate thread
		img->setUseTexture(false);
		bool loaded = img->load(path);
		if (!loaded) CogLogError("Resources", "Image couldn't be loaded: %s", path.c_str());

		auto image = spt<ofImage>(img);
		loadedImages[path] = image;

		return image;
	}

	spt<ofVboMesh> Resources::GetVboMesh(string path) {
		auto found = loadedVboMeshes.find(path);
		if (found != loadedVboMeshes.end()) {
			return (found->second);
		}
		return spt<ofVboMesh>();
	}

	Soundfx* Resources::GetSound(string path) {
		auto found = loadedSounds.find(path);
		if (found != loadedSounds.end()) {
			return (found->second);
		}

		COGLOGDEBUG("Resources", "Loading sound %s", path.c_str());
		return new Soundfx(path);
	}

	spt<ofxXmlSettings> Resources::PreloadXMLFile(string path) {
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

	spt<ofxXmlSettings> Resources::LoadXMLFile(string path) {
		ofxXmlSettings* xml = new ofxXmlSettings();
		xml->loadFile(path);
		auto xmlPtr = spt<ofxXmlSettings>(xml);
		return xmlPtr;
	}


	string Resources::GetResourceStr(string resourceType, string resourceKey) {
		if (resourceType.compare("string") == 0) {
			return assetsMgr.GetString(resourceKey);
		}
		else if (resourceType.compare("dimen") == 0) {
			return assetsMgr.GetDimension(resourceKey);
		}

		CogLogError("Resources", "Resource of type %s with key %s not found", resourceType.c_str(), resourceKey.c_str());

		return string();
	}

}// namespace