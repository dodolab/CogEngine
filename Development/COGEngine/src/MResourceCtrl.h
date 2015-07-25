#pragma once

#include "ofxSmartPointer.h"
#include <string>
#include <map>
#include "ofImage.h"
#include "ofTrueTypeFont.h"


using namespace std;

/**
* Resource controller that holds images, 3D objects and sounds
*/
class MResourceCtrl{
private:
	// cached images
	map<string, spt<ofImage>> loadedImages;
	// cached meshes
	map<string, spt<ofVboMesh>> loadedMeshes;

public:

	/**
	* Loads 2D image from file
	* @param path path to file
	*/
	spt<ofImage> Get2DImage(string path);
	
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
};

