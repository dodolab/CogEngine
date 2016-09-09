#include "CogEngine.h"
#include "CogTranslateAnim.h"
#include "CogNode.h"
#include "CogRotateAnim.h"
#include "CogFactory.h"
#include "CogCache.h"
#include "ofSoundPlayer.h"

CogEngine COGEngine;


void CogEngine::Init(CogFactory* factory, spt<ofxXmlSettings> config){

	// create components
	environmentCtrl = new CogEnvironment();
	resourceCtrl = new CogCache();
	logger = new CogLogger(config);
	storage = new CogRepository();
	renderer = new CogRenderer();
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


void CogEngine::Update(uint64 delta, uint64 absolute){
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

void CogEngine::Draw(uint64 delta, uint64 absolute){
	renderer->ClearCounters();
	// clear the drawing surface
	ofBackground(50,50,50);
	// setup ortographic camera
	ofSetupScreenOrtho(environmentCtrl->GetRealWidth(),environmentCtrl->GetRealHeight(),-1000,1000);

	// draw the root node
	this->_root->GetRoot()->Draw(delta, absolute);

	renderer->Render();
}
