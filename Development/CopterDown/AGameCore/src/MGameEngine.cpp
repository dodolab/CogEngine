#include "MGameEngine.h"
#include "BeTranslateAnim.h"
#include "GNode.h"
#include "IwRandom.h"
#include "Iw2D.h"
#include "IwResManagerClass.h"
#include "BeRender.h"
#include "IwGx.h"
#include "Iw2DSceneGraphCore.h"
#include "BeRotateAnim.h"

using namespace Iw2DSceneGraphCore;

MGameEngine MEngine;

void MGameEngine::Update(uint64 delta, uint64 absolute){
	this->_root->GetRoot()->Update(delta, absolute, CIwFMat2D::g_Identity);
}

void MGameEngine::Draw(uint64 delta, uint64 absolute){
	// Clear the drawing surface
	Iw2DSurfaceClear(0xff000000);

	// draw the root node
	this->_root->GetRoot()->Draw(delta, absolute, CIwFMat2D::g_Identity);

	// Show the drawing surface
	Iw2DSurfaceShow();
}


void MGameEngine::Init(MGameFactory* factory, MGameStorage* storage){

	environmentCtrl = new MEnvironmentCtrl();
	resourceCtrl = new MResourceCtrl();
	this->factory = factory;
	this->storage = storage;

	absolute = s3eTimerGetMs();

	environmentCtrl->Init();
	resourceCtrl->Init();
	factory->Init();
	storage->Init();

	_root = factory->CreateRoot();
}

void MGameEngine::StartLoop(){
	uint64 deltaTime = s3eTimerGetMs();
	double angle = 0;

	while (!s3eDeviceCheckQuitRequest())
	{
		uint64 delta = s3eTimerGetMs() - deltaTime;
		deltaTime = s3eTimerGetMs();

		environmentCtrl->UpdateInputs();

		Update(delta, absolute);
		Draw(delta, absolute);

		environmentCtrl->RemoveEndedInputs();

		// sleep 0ms (for input events update)
		s3eDeviceYield(0);
	}
}

void MGameEngine::Terminate(){
	storage->Terminate();
	factory->Terminate();
	resourceCtrl->Terminate();
	environmentCtrl->Terminate();

	Iw2DTerminate();
}