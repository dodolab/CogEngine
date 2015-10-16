#include "ofxAreEngine.h"
#include "ofxArbTranslateAnim.h"
#include "ofxAreNode.h"
#include "ofxArbRotateAnim.h"
#include "ofxArcFactory.h"
#include "ofxArcRepository.h"
#include "ofSoundPlayer.h"

ofxAreEngine COGEngine;


void ofxAreEngine::Init(ofxArcFactory* factory, spt<ofxXmlSettings> config){

	// create components
	environmentCtrl = new MofxArcEnvironmentCtrl();
	resourceCtrl = new ofxArcStorage();
	logger = new ofxAreLogger(config);
	storage = new ofxArcRepository();
	renderer = new ofxArcRenderer();
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


void ofxAreEngine::Update(uint64 delta, uint64 absolute){
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

void ofxAreEngine::Draw(uint64 delta, uint64 absolute){
	renderer->ClearCounters();
	// clear the drawing surface
	ofBackground(50,50,50);
	// setup ortographic camera
	ofSetupScreenOrtho(environmentCtrl->GetRealWidth(),environmentCtrl->GetRealHeight(),-1000,1000);

	// draw the root node
	this->_root->GetRoot()->Draw(delta, absolute);

	renderer->Render();
}
