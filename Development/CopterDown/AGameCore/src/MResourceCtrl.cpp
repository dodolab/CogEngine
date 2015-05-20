#include "MResourceCtrl.h"
#include "IwResManagerClass.h"
#include <string>
#include "Iw2D.h"

void MResourceCtrl::Init(){
	// initialize resources manager
	IwResManagerInit();
	// load application data
	resourceGroup = IwGetResManager()->LoadGroup("resources.group");
}

spt<CIw2DImage> MResourceCtrl::GetImage(string name){
	return spt<CIw2DImage>(Iw2DCreateImageResource(name.c_str()));
}

void MResourceCtrl::Terminate(){
	IwResManagerTerminate();
}