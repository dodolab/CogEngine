#include "MResourceCtrl.h"
#include <string>
#include "ofImage.h"
#include "Enums.h"

void MResourceCtrl::Init(){
	// initialize resources manager
	// load application data
	//resourceGroup = IwGetResManager()->LoadGroup("resources.group");
}

spt<ofImage> MResourceCtrl::Get2DImage(string name){

	auto found = loadedImages.find(name);
	if (found != loadedImages.end()){
		return (found->second);
	}

	ofImage* img = new ofImage(name);

	ofVboMesh* mesh = new ofVboMesh();
	mesh->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mesh->addVertex(Vectorf3(-img->width/2,-img->height/2,1));
	mesh->addVertex(Vectorf3(img->width/2,-img->height/2,1));
	mesh->addVertex(Vectorf3(-img->width/2,img->height/2,1));
	mesh->addVertex(Vectorf3(img->width/2,img->height/2,1));

	// todo: for some devices (android) it has to be normalized
	mesh->addTexCoord(ofVec2f(0,0));
	mesh->addTexCoord(ofVec2f(img->width,0));
	mesh->addTexCoord(ofVec2f(0,img->height));
	mesh->addTexCoord(ofVec2f(img->width,img->height));

	auto image = spt<ofImage>(img);
	auto meshPtr = spt<ofVboMesh>(mesh);

	loadedImages[name] = image;
	loadedMeshes[name] = meshPtr;
	

	return image;
}

spt<ofVboMesh> MResourceCtrl::GetMesh(string name){
	auto found = loadedMeshes.find(name);
	if (found != loadedMeshes.end()){
		return (found->second);
	}
	return spt<ofVboMesh>();
}

void MResourceCtrl::Terminate(){
	//IwResManagerTerminate();
}