#include "MResourceCtrl.h"
#include <string>
#include "ofImage.h"

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

	auto image = spt<ofImage>(img);
	loadedImages[name] = image;
	return image;
}



void MResourceCtrl::Terminate(){
	//IwResManagerTerminate();
}