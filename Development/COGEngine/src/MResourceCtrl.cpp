#include "MResourceCtrl.h"
#include <string>
#include "ofImage.h"
#include "MEnums.h"


spt<ofImage> MResourceCtrl::Get2DImage(string path){

	auto found = loadedImages.find(path);
	if (found != loadedImages.end()){
		return (found->second);
	}

	ofImage* img = new ofImage(path);
	ofVboMesh* mesh = new ofVboMesh();
	mesh->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mesh->addVertex(ofVec3f(-img->width/2,-img->height/2,1));
	mesh->addVertex(ofVec3f(img->width/2,-img->height/2,1));
	mesh->addVertex(ofVec3f(-img->width/2,img->height/2,1));
	mesh->addVertex(ofVec3f(img->width/2,img->height/2,1));

	// todo: for some devices (android) it has to be normalized
	mesh->addTexCoord(ofVec2f(0,0));
	mesh->addTexCoord(ofVec2f(img->width,0));
	mesh->addTexCoord(ofVec2f(0,img->height));
	mesh->addTexCoord(ofVec2f(img->width,img->height));

	auto image = spt<ofImage>(img);
	auto meshPtr = spt<ofVboMesh>(mesh);

	loadedImages[path] = image;
	loadedMeshes[path] = meshPtr;
	

	return image;
}

spt<ofVboMesh> MResourceCtrl::GetMesh(string path){
	auto found = loadedMeshes.find(path);
	if (found != loadedMeshes.end()){
		return (found->second);
	}
	return spt<ofVboMesh>();
}

spt<ofTrueTypeFont> MResourceCtrl::GetFont(string path, int size){
	ofTrueTypeFont::setGlobalDpi(72);

	ofTrueTypeFont* font = new ofTrueTypeFont();

	font->loadFont(path, size, true, true);
	font->setLineHeight(18.0f);
	font->setLetterSpacing(1.037);

	return spt<ofTrueTypeFont>(font);
}