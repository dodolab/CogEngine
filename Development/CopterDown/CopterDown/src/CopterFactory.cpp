#include "CopterFactory.h"

GNode* CopterFactory::CreateRoot(){
	GNode* root = new GNode(ObjType::ROOT, 12, "fofík");
	GNode* splash = new GNode(ObjType::SCENE, 13, "scéna");

	this->SetSingleBackground(splash, "images/intro.png");
	root->AddChild(splash);
	return root;
}