#include "Cache.h"
#include <string>
#include "ofImage.h"
#include "CogFacade.h"
#include "Utils.h"
#include "SoundFile.h"

void Storage::Init(){
	// set global dpi so font will have the same pixel size on each display
	ofTrueTypeFont::setGlobalDpi(COGGetScreenWidth()*72*0.001f);
}

spt<ofImage> Storage::Get2DImage(string path){

	auto found = loadedImages.find(path);
	if (found != loadedImages.end()){
		spt<ofImage> existing =  (found->second);
		if (!existing->isUsingTexture()){
			// for preloaded images, they need to be copied to the texture
			existing->setUseTexture(true);
			//existing->reloadTexture();
		}

		return existing;
	}

	ofImage* img = new ofImage(path);
	ofVboMesh* mesh = new ofVboMesh();
	mesh->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mesh->addVertex(ofVec3f(-img->getWidth()/2,-img->getHeight()/2,1));
	mesh->addVertex(ofVec3f(img->getWidth()/2,-img->getHeight()/2,1));
	mesh->addVertex(ofVec3f(-img->getWidth()/2,img->getHeight()/2,1));
	mesh->addVertex(ofVec3f(img->getWidth()/2,img->getHeight()/2,1));

	// todo: for some devices (android) it has to be normalized
	mesh->addTexCoord(ofVec2f(0,0));
	mesh->addTexCoord(ofVec2f(img->getWidth(),0));
	mesh->addTexCoord(ofVec2f(0,img->getHeight()));
	mesh->addTexCoord(ofVec2f(img->getWidth(),img->getHeight()));

	auto image = spt<ofImage>(img);
	auto meshPtr = spt<ofVboMesh>(mesh);

	loadedImages[path] = image;
	loadedMeshes[path] = meshPtr;
	

	return image;
}

spt<ofImage> Storage::Preload2DImage(string path){
	auto found = loadedImages.find(path);
	if (found != loadedImages.end()){
		return (found->second);
	}

	ofImage* img = new ofImage();
	// don't use texture because images are loaded in separate thread
	img->setUseTexture(false);
	bool loaded = img->loadImage(path);
	if (!loaded) COGLogError("Image couldn't be loaded: %s",path.c_str());


	auto image = spt<ofImage>(img);
	loadedImages[path] = image;

	return image;
}

spt<ofVboMesh> Storage::GetMesh(string path){
	auto found = loadedMeshes.find(path);
	if (found != loadedMeshes.end()){
		return (found->second);
	}
	return spt<ofVboMesh>();
}

spt<ofTrueTypeFont> Storage::GetFont(string path, int size){
	ofTrueTypeFont* font = new ofTrueTypeFont();
	font->loadFont(path, size, true, true);
	return spt<ofTrueTypeFont>(font);
}

spt<SoundFile> Storage::GetSound(string path){
	auto found = loadedSounds.find(path);
	if (found != loadedSounds.end()){
		return (found->second);
	}

	SoundFile* snd = new SoundFile(path);
	return spt<SoundFile>(snd);
}

spt<ofxXmlSettings> Storage::PreloadXMLFile(string path){
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

spt<ofxXmlSettings> Storage::LoadXMLFile(string path){
	ofxXmlSettings* xml = new ofxXmlSettings();
	xml->loadFile(path);
	auto xmlPtr = spt<ofxXmlSettings>(xml);
	return xmlPtr;
}

spt<Anim> Storage::GetAnimation(string name){
	auto found = loadedAnimations.find(name);
	if (found != loadedAnimations.end()) return found->second;
	else return spt<Anim>();
}

void Storage::StoreAnimation(spt<Anim> anim){
	MASSERT(anim->GetName().compare("") != 0, "Storage", "Attempt to store animation without a name!");
	
	auto found = loadedAnimations.find(anim->GetName());
	if (found == loadedAnimations.end()){
		loadedAnimations[anim->GetName()] = anim;
	}
}

spt<SpriteSheet> Storage::GetSpriteSheet(string name){
	auto found = loadedSpriteSheets.find(name);
	if (found != loadedSpriteSheets.end()) return found->second;
	else return spt<SpriteSheet>();
}

void Storage::StoreSpriteSheet(spt<SpriteSheet> spriteSheet){
	MASSERT(spriteSheet->GetName().compare("") != 0, "Storage", "Attempt to store spritesheet without a name!");

	auto found = loadedSpriteSheets.find(spriteSheet->GetName());
	if (found == loadedSpriteSheets.end()){
		loadedSpriteSheets[spriteSheet->GetName()] = spriteSheet;
	}
}