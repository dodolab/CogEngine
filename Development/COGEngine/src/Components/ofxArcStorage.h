#pragma once

#include "ofxSmartPointer.h"
#include <string>
#include <map>
#include "ofImage.h"
#include "ofTrueTypeFont.h"
#include "ofxAraSound.h"
#include "ofxXmlSettings.h"
#include "ofxAraAnim.h"
#include "ofxAraSpriteSheet.h"


using namespace std;

/**
* Resource controller that holds images, 3D objects and sounds
*/
class ofxArcStorage{
private:
	// cached images
	map<string, spt<ofImage>> loadedImages;
	// cached meshes
	map<string, spt<ofVboMesh>> loadedMeshes;
	// cached sounds and music
	map<string, spt<ofxAraSound>> loadedSounds;
	// cached xml files
	map<string, spt<ofxXmlSettings>> loadedXMLs;
	// cached animation
	map<string, spt<ofxAraAnim>> loadedAnimations;
	// cached spritesheets
	map<string, spt<ofxAraSpriteSheet>> loadedSpriteSheets;

public:

	/**
	* Initializes controller
	*/
	void Init();

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
	spt<ofxAraSound> GetSound(string path);

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
	spt<ofxAraAnim> GetAnimation(string name);

	/**
	* Stores animation
	* @param anim animation to store
	*/
	void StoreAnimation(spt<ofxAraAnim> anim);

	/**
	* Gets spritesheet by name
	* @param name name of spritesheet
	*/
	spt<ofxAraSpriteSheet> GetSpriteSheet(string name);

	/**
	* Stores spritesheet
	* @param spriteSheet spritesheet to store
	*/
	void StoreSpriteSheet(spt<ofxAraSpriteSheet> spriteSheet);
};

