#include "MEngine.h"
#include "BeTranslateAnim.h"
#include "GNode.h"
#include "BeRotateAnim.h"
#include "MFactory.h"
#include "MRepository.h"
#include "ofSoundPlayer.h"

MEngine COGEngine;


void MEngine::Init(MFactory* factory, spt<ofxXmlSettings> config){

	// create components
	environmentCtrl = new MEnvironmentCtrl();
	resourceCtrl = new MStorage();
	logger = new MLogger(config);
	storage = new MRepository();
	renderer = new MRenderer();
	this->factory = factory;
	
	// initialize components 
	environmentCtrl->Init();
	factory->Init();
	logger->Init();
	renderer->Init();
	resourceCtrl->Init();
	// create root node
	_root = factory->CreateRoot();
}


void MEngine::Update(uint64 delta, uint64 absolute){
	frameCounter++;

	// update transforms
	this->_root->GetRoot()->UpdateTransform(true);
	// update scene
	this->_root->GetRoot()->Update(delta, absolute);
	// remove ended inputs
	environmentCtrl->RemoveEndedProcesses();
 	
	// flush each 100th frame
	if (frameCounter % 100 == 0){
		logger->Flush();
	}

	ofSoundUpdate();
}

void MEngine::Draw(uint64 delta, uint64 absolute){
	
	renderer->Init();
	// clear the drawing surface
	ofBackground(50,50,50);
	// setup ortographic camera
	ofSetupScreenOrtho(environmentCtrl->GetWidth(),environmentCtrl->GetHeight(),-1000,1000);
	// draw the root node
	this->_root->GetRoot()->Draw(delta, absolute);

	renderer->Render();
}
