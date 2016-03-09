#pragma once

#include "ofxCogCommon.h"
#include "Component.h"
#include "Settings.h"
#include "DEntity.h"
#include "TransformEnt.h"
namespace Cog {

	class SoundFile;
	class SheetAnim;
	class SpriteSheet;

	/**
	* Resource controller that holds images, 3D objects and sounds
	*/
	class ResourceCache : public Component {

		OBJECT(ResourceCache)

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
		map<string, spt<CommonAnim>> loadedAnimations;
		// cached entities
		map<string, spt<DEntity>> loadedEntities;
		// cached spritesheets
		map<string, spt<SpriteSheet>> loadedSpriteSheets;
		// loaded fonts (each DPI must have one font loaded)
		map<string, map<int,spt<ofTrueTypeFont>>> loadedFonts;

		Settings loadedDefaultSettings;
		Settings loadedGlobalSettings;
		Settings loadedProjectSettings;

	public:

		ResourceCache() {
			// resource cache should be initialized with higher priority because it
			// loads XML settings
			this->initPriority = InitPriority::HIGH;
		}

		~ResourceCache() {

		}

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
		spt<CommonAnim> GetAnimation(string name);

		/**
		* Stores animation
		* @param anim animation to store
		*/
		void StoreAnimation(spt<CommonAnim> anim);

		/**
		* Gets stored entity by name
		*/
		spt<DEntity> GetEntity(string name);

		template<class T> spt<T> GetEntityC(string name) {
			spt<DEntity> entity = GetEntity(name);
			if (!entity) return spt<T>();

			DEntity* entityPtr = entity.get();
			COGASSERT(typeid(*entityPtr) == typeid(T), "RESOURCE", "Entity %s is of the wrong type!", name.c_str());

			// never create another pointer, because it will be destroyed within the original!
			// always use static_cast of the actual pointer
			return static_cast<spt<T>>(entity);
		}

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
		* Gets spritesheet by name
		* @param name name of spritesheet
		*/
		spt<SpriteSheet> GetSpriteSheet(string name);

		/**
		* Stores spritesheet
		* @param spriteSheet spritesheet to store
		*/
		void StoreSpriteSheet(spt<SpriteSheet> spriteSheet);

		Settings& GetDefaultSettings() {
			return loadedDefaultSettings;
		}

		Settings& GetGlobalSettings() {
			return loadedGlobalSettings;
		}

		Settings& GetProjectSettings() {
			return loadedProjectSettings;
		}

		Setting GetDefaultSettings(string name);

		Setting GetGlobalSettings(string name);

		Setting GetProjectSettings(string name);

		virtual void Update(const uint64 delta, const uint64 absolute) {
		}
	};

}// namespace