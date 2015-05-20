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
	this->gameCtrl->GetRoot()->Update(delta, absolute, CIwFMat2D::g_Identity);
}

void MGameEngine::Draw(uint64 delta, uint64 absolute){
	// Clear the drawing surface
	Iw2DSurfaceClear(0xff000000);

	// draw the root node
	this->gameCtrl->GetRoot()->Draw(delta, absolute, CIwFMat2D::g_Identity);

	// Show the drawing surface
	Iw2DSurfaceShow();
}


void MGameEngine::Init(){

	environmentCtrl = new MEnvironmentCtrl();
	gameCtrl = new MGameCtrl();
	resourceCtrl = new MResourceCtrl();

	absolute = s3eTimerGetMs();

	// initialize 2D graphic system
	Iw2DInit();
	// initialize common graphic system
	IwGxInit();
	
	// dont use mipmaps -> better memory usage
	Iw2DSetUseMipMapping(false);

	resourceCtrl->Init();
	environmentCtrl->Init();

	gameCtrl->Init();
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

		environmentCtrl->CheckInputs();

		// sleep 0ms (for input events update)
		s3eDeviceYield(0);
	}
}

void MGameEngine::Terminate(){
	resourceCtrl->Terminate();
	Iw2DTerminate();
	IwGxTerminate();
}