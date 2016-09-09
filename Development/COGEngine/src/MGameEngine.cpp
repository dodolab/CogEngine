#include "MGameEngine.h"
#include "BeTranslateAnim.h"
#include "GNode.h"
#include "BeRender.h"
#include "BeRotateAnim.h"


MGameEngine MEngine;

void MGameEngine::Update(uint64 delta, uint64 absolute){

	ofMatrix4x4 identity = ofMatrix4x4::newIdentityMatrix();
	identity.translate(0,0,-690);
	this->_root->GetRoot()->Update(delta, absolute, identity);

	environmentCtrl->RemoveEndedInputs();
}

void MGameEngine::Draw(uint64 delta, uint64 absolute){
	// Clear the drawing surface
	ofBackground(250,0,0);
	ofSetupScreenOrtho(environmentCtrl->GetWidth(),environmentCtrl->GetHeight(),-1000,1000);
	// draw the root node
	ofMatrix4x4 identity = ofMatrix4x4::newIdentityMatrix();
	identity.translate(0,0,-690);
	this->_root->GetRoot()->Draw(delta, absolute, identity);

}


void MGameEngine::Init(MGameFactory* factory, MGameStorage* storage){

	environmentCtrl = new MEnvironmentCtrl();
	resourceCtrl = new MResourceCtrl();
	this->factory = factory;
	this->storage = storage;

	environmentCtrl->Init();
	resourceCtrl->Init();
	factory->Init();
	storage->Init();

	_root = factory->CreateRoot();
}

void MGameEngine::StartLoop(){
	
}

void MGameEngine::Terminate(){
	storage->Terminate();
	factory->Terminate();
	resourceCtrl->Terminate();
	environmentCtrl->Terminate();
}