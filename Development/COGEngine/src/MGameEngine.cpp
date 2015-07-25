#include "MGameEngine.h"
#include "BeTranslateAnim.h"
#include "GNode.h"
#include "BeRender.h"
#include "BeRotateAnim.h"
#include "MGameFactory.h"
#include "MGameStorage.h"

MGameEngine MEngine;


void MGameEngine::Init(MGameFactory* factory){

	// create components
	environmentCtrl = new MEnvironmentCtrl();
	resourceCtrl = new MResourceCtrl();
	logger = new MLogger(new FileLoggerChannel("logfile.log", true));
	storage = new MGameStorage();

	this->factory = factory;
	
	// initialize components 
	environmentCtrl->Init();
	factory->Init();

	// create game root
	_root = factory->CreateRoot();
}


void MGameEngine::Update(uint64 delta, uint64 absolute){
	frameCounter++;

	// update transforms
	this->_root->GetRoot()->UpdateTransforms();
	// update scene
	this->_root->GetRoot()->Update(delta, absolute);
	// remove ended inputs
	environmentCtrl->RemoveEndedInputs();
 	
	// flush each 100th frame
	if (frameCounter % 100 == 0){
		logger->Flush();
	}
}

void MGameEngine::Draw(uint64 delta, uint64 absolute){
	// clear the drawing surface
	ofBackground(50,50,50);
	// setup ortographic camera
	ofSetupScreenOrtho(environmentCtrl->GetWidth(),environmentCtrl->GetHeight(),-1000,1000);
	// draw the root node
	this->_root->GetRoot()->Draw(delta, absolute);
}
