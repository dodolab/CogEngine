#pragma once

#include "Component.h"
#include "Settings.h"
#include "DEntity.h"
#include "TransformEnt.h"
#include "GeneralAnim.h"
#include "AssetsManager.h"
#include "pugixml.h"

using namespace pugi;

namespace Cog {

	class Soundfx;
	class SheetAnim;
	class SpriteSheet;

	/**
	* Resource controller that contains media and configuration files
	*/
	class Resources : public Component {

	private:
		AssetsManager assetsMgr;

		// cached images
		map<string, spt<ofImage>> loadedImages;
		// cached meshes
		map<string, spt<ofVboMesh>> loadedVboMeshes;
		// cached sounds and music
		map<string, Soundfx*> loadedSounds;
		// cached xml files
		map<string, spt<xml_document>> loadedXMLs;

	public:

		Resources() {
			// resource cache should be initialized with higher priority
			this->initPriority = InitPriority::HIGH;
		}

		~Resources() {

		}

		void OnInit();

		/**
		* Loads 2D image from file
		* @param path path to file
		*/
		spt<ofImage> Get2DImage(string path);

		/**
		* Preloads 2D image from file; doesn't use texture so the image
		* must be copied before the first render, but this is done
		* automatically; this method is useful for loading from separate
		* thread since the access to textures is problematic
		* @param path path to file
		*/
		spt<ofImage> Preload2DImage(string path);

		/**
		* Loads mesh from file
		* @param path path to file
		*/
		spt<ofVboMesh> GetVboMesh(string path);

		/**
		* Loads font from file
		* @param path path to file
		* @size font size in dpi
		*/
		spt<ofTrueTypeFont> GetFont(string path, int size) {
			return assetsMgr.GetFont(path, size);
		}

		/**
		* Loads either sound or music from file
		* @param path path to file
		*/
		Soundfx* GetSound(string path);

		/**
		* Preloads xml file; if the file is in the cache, returns cached xml
		* @param path path to file
		*/
		spt<xml_document> PreloadXMLFile(string path);

		/**
		* Loads xml file without caching
		* @param path path to file
		*/
		spt<xml_document> LoadXMLFile(string path);

		/**
		* Gets animation by name
		* @param name animation name
		*/
		spt<GeneralAnim> GetAnimation(string name) {
			return assetsMgr.GetAnimation(name);
		}

		/**
		* Stores animation
		* @param anim animation to store
		*/
		void StoreAnimation(spt<GeneralAnim> anim) {
			assetsMgr.StoreAnimation(anim);
		}

		/**
		* Gets stored entity by name
		*/
		spt<DEntity> GetEntity(string name) {
			return assetsMgr.GetEntity(name);
		}

		/**
		* Gets stored entity by name
		* @param name name of the entity
		* @tparam entity type
		*/
		template<class T> spt<T> GetEntity(string name) {
			spt<DEntity> entity = GetEntity(name);
			if (!entity) return spt<T>();

			DEntity* entityPtr = entity.get();
			COGASSERT(typeid(*entityPtr) == typeid(T), "Resources", "Entity %s is of the wrong type!", name.c_str());

			// never create another shared pointer of the same object, because one of them may be destroyed when the
			// second one still works with the object
			// always use static_pointer_cast instead
			return static_pointer_cast<T>(entity);
		}

		/**
		* Stores entity under its name
		* @param entity entity to store
		*/
		void StoreEntity(spt<DEntity> entity) {
			assetsMgr.StoreEntity(entity);
		}

		/**
		* Stores entity
		* @param name entity name, has to be unique
		* @entity entity to store
		*/
		void StoreEntity(string name, spt<DEntity> entity) {
			assetsMgr.StoreEntity(name, entity);
		}

		/**
		* Gets spritesheet by name
		* @param name name of spritesheet
		*/
		spt<SpriteSheet> GetSpriteSheet(string name) {
			return assetsMgr.GetSpriteSheet(name);
		}

		/**
		* Stores spritesheet
		* @param spriteSheet spritesheet to store
		*/
		void StoreSpriteSheet(spt<SpriteSheet> spriteSheet) {
			assetsMgr.StoreSpriteSheet(spriteSheet);
		}

		/**
		* Gets default settings that contains default values
		* for any component
		*/
		Settings& GetDefaultSettings() {
			return assetsMgr.GetDefaultSettings();
		}

		/**
		* Gets global settings that contains initialization
		* configuration such as logger settings, aspect ratio etc.
		*/
		Settings& GetGlobalSettings() {
			return assetsMgr.GetGlobalSettings();
		}

		/**
		* Gets custom project settings that may contain anything
		*/
		Settings& GetProjectSettings() {
			return assetsMgr.GetProjectSettings();
		}

		/**
		* Gets default setting by name of the parent group,
		* for example "transform"
		*/
		Setting GetDefaultSettings(string name) {
			return assetsMgr.GetDefaultSettings(name);
		}

		/**
		* Gets global setting by name of the parent group,
		* for example "logger"
		*/
		Setting GetGlobalSettings(string name) {
			return assetsMgr.GetGlobalSettings(name);
		}

		/**
		* Gets project setting by name of the parent group
		*/
		Setting GetProjectSettings(string name) {
			return assetsMgr.GetProjectSettings(name);
		}

		/**
		* Gets resource value from assets
		* @param resourceType type of the resource (string, dimen etc.)
		* @param resourceKey key of the resource
		*/
		string GetResourceStr(string resourceType, string resourceKey);

		virtual void Update(const uint64 delta, const uint64 absolute) {
		}
	};

}// namespace