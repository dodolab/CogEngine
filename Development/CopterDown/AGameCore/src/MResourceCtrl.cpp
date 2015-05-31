#include "MResourceCtrl.h"
#include "IwResManagerClass.h"
#include <string>
#include "Iw2D.h"
#include "IwTexture.h"

void MResourceCtrl::Init(){
	// initialize resources manager
	IwResManagerInit();
	// load application data
	//resourceGroup = IwGetResManager()->LoadGroup("resources.group");
}

spt<CIw2DImage> MResourceCtrl::Get2DImage(string name){

	auto found = loadedImages.find(name);
	if (found != loadedImages.end()){
		return (found->second);
	}

	auto image = spt<CIw2DImage>(Iw2DCreateImage(name.c_str()));
	loadedImages[name] = image;
	return image;
}

spt<CIwImage> MResourceCtrl::GetImage(string name){
	// todo: maybe memory leak here
	CIwImage* img = new CIwImage();
	img->LoadFromFile(name.c_str());
	
	return spt<CIwImage>(img);
}


void MResourceCtrl::Terminate(){
	IwResManagerTerminate();
}