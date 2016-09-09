#include "MGameEngine.h"
#include "BeTranslateAnim.h"
#include "GNode.h"
#include "BeRender.h"
#include "BeRotateAnim.h"
#include "MGameFactory.h"
#include "MGameStorage.h"

MGameEngine MEngine;

void MGameEngine::Update(uint64 delta, uint64 absolute){
	ofMatrix4x4 identity = ofMatrix4x4::newIdentityMatrix();
	identity.translate(0,0,-690);

	this->_root->GetRoot()->UpdateTransforms();

	//this->_root->GetRoot()->Update(delta, absolute);

	// fixed delta -> no glitches
	this->_root->GetRoot()->Update(delta, absolute);
	environmentCtrl->RemoveEndedInputs();
 	logger->Flush();
}

void MGameEngine::Draw(uint64 delta, uint64 absolute){
	// Clear the drawing surface
	ofBackground(50,50,50);
	ofSetupScreenOrtho(environmentCtrl->GetWidth(),environmentCtrl->GetHeight(),-1000,1000);
	// draw the root node
	ofMatrix4x4 identity = ofMatrix4x4::newIdentityMatrix();
	identity.translate(0,0,-690);
	
	this->_root->GetRoot()->Draw(delta, absolute);
}


void MGameEngine::Init(MGameFactory* factory){

	
	environmentCtrl = new MEnvironmentCtrl();
	resourceCtrl = new MResourceCtrl();
	logger = new MLogger(new FileLoggerChannel("logfile.log", true));

	this->factory = factory;
	this->storage = storage = new MGameStorage();

	environmentCtrl->Init();
	resourceCtrl->Init();
	storage->Init();
	factory->Init();
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