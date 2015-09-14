#include "MStorage.h"
#include <string>
#include "ofImage.h"
#include "MFacade.h"
#include "MUtils.h"

void MStorage::Init(){
	// set global dpi so font will have the same pixel size on each display
	ofTrueTypeFont::setGlobalDpi(COGGetScreenWidth()*72*0.001f);
}

spt<ofImage> MStorage::Get2DImage(string path){

	auto found = loadedImages.find(path);
	if (found != loadedImages.end()){
		spt<ofImage> existing =  (found->second);
		if (!existing->isUsingTexture()){
			// for preloaded images, they need to be copied to the texture
			existing->setUseTexture(true);
			existing->reloadTexture();
		}

		return existing;
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

spt<ofImage> MStorage::Preload2DImage(string path){
	auto found = loadedImages.find(path);
	if (found != loadedImages.end()){
		return (found->second);
	}

	ofImage* img = new ofImage();
	// don't use texture because images are loaded in separate thread
	img->setUseTexture(false);
	img->loadImage(path);
	auto image = spt<ofImage>(img);
	loadedImages[path] = image;

	return image;
}

spt<ofVboMesh> MStorage::GetMesh(string path){
	auto found = loadedMeshes.find(path);
	if (found != loadedMeshes.end()){
		return (found->second);
	}
	return spt<ofVboMesh>();
}

spt<ofTrueTypeFont> MStorage::GetFont(string path, int size){
	ofTrueTypeFont* font = new ofTrueTypeFont();
	font->loadFont(path, size, true, true);
	return spt<ofTrueTypeFont>(font);
}

spt<EnSound> MStorage::GetSound(string path){
	auto found = loadedSounds.find(path);
	if (found != loadedSounds.end()){
		return (found->second);
	}

	EnSound* snd = new EnSound(path);
	return spt<EnSound>(snd);
}

spt<ofxXmlSettings> MStorage::PreloadXMLFile(string path){
	auto found = loadedXMLs.find(path);
	if (found != loadedXMLs.end()){
		return (found->second);
	}

	ofxXmlSettings* xml = new ofxXmlSettings();
	xml->loadFile(path);
	auto xmlPtr = spt<ofxXmlSettings>(xml);
	loadedXMLs[path] = xmlPtr;
	return xmlPtr;
}

spt<ofxXmlSettings> MStorage::LoadXMLFile(string path){
	ofxXmlSettings* xml = new ofxXmlSettings();
	xml->loadFile(path);
	auto xmlPtr = spt<ofxXmlSettings>(xml);
	return xmlPtr;
}

spt<EnAnim> MStorage::GetAnimation(string name){
	auto found = loadedAnimations.find(name);
	if (found != loadedAnimations.end()) return found->second;
	else return spt<EnAnim>();
}

void MStorage::StoreAnimation(spt<EnAnim> anim){
	MASSERT(anim->GetName().compare("") == 0, "Attempt to store animation without a name!");
	
	auto found = loadedAnimations.find(anim->GetName());
	if (found == loadedAnimations.end()){
		loadedAnimations[anim->GetName()] = anim;
	}
}