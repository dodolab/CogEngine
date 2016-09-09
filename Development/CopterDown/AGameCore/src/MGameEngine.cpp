#include "MGameEngine.h"
#include "BeTranslateAnim.h"
#include "GNode.h"
#include "IwRandom.h"
#include "Iw2D.h"
#include "IwResManagerClass.h"
#include "BeRender.h"
#include "IwGx.h"
#include "Iw2DSceneGraphCore.h"

using namespace Iw2DSceneGraphCore;

void MGameEngine::Update(uint64 delta, uint64 absolute){
	root->Update(delta, absolute, CIwFMat2D::g_Identity.SetTrans(CIwFVec2(environmentCtrl.width / 2, environmentCtrl.height)));
}

void MGameEngine::Draw(uint64 delta, uint64 absolute){
	// Clear the drawing surface
	Iw2DSurfaceClear(0xff000000);

	// draw the root node
	root->Draw(delta, absolute, CIwFMat2D::g_Identity);

	// Show the drawing surface
	Iw2DSurfaceShow();
}


void MGameEngine::Init(){

	absolute = s3eTimerGetMs();

	// initialize 2D graphic system
	Iw2DInit();
	// initialize common graphic system
	IwGxInit();
	// initialize resources manager
	IwResManagerInit();
	// dont use mipmaps -> better memory usage
	Iw2DSetUseMipMapping(false);


	// initialize root node
	root = new GNode(ObjType::ROOT, 0, "root");
	
	root->AddAttr(Attrs::USERACTION, environmentCtrl.userActions);

	// load application data
	resourceGroup = IwGetResManager()->LoadGroup("resources.group");

	// Load images
	spt<CIw2DImage> image = spt<CIw2DImage>(Iw2DCreateImageResource("blue"));

	for (int i = 0; i < 2000; i++){
		GNode* child = new GNode(ObjType::OBJECT, 0, "other");

		CIwFVec2 randomTransform(IwRandMinMax(1, environmentCtrl.width), IwRandMinMax(1, environmentCtrl.height));
		CIwFVec2 randomTransform2(IwRandMinMax(1, environmentCtrl.width), IwRandMinMax(1, environmentCtrl.height));

		child->GetTransform().SetTrans(randomTransform);
		child->GetTransform().Scale(0.063f);

		child->AddAttr<spt<CIw2DImage>>(Attrs::IMGSOURCE, image);
		child->AddBehavior(new BeRender());
		float speed = 0.1f*((float)IwRandMinMax(1, 15));

		child->AddBehavior(new BeTranslateAnim(randomTransform, randomTransform2, speed, true, true));

		root->AddChild(child);
	}


	environmentCtrl.Initialize();
}
void MGameEngine::StartLoop(){
	uint64 deltaTime = s3eTimerGetMs();
	double angle = 0;

	while (!s3eDeviceCheckQuitRequest())
	{
		uint64 delta = s3eTimerGetMs() - deltaTime;
		deltaTime = s3eTimerGetMs();

		//Update the input systems
		s3eKeyboardUpdate();
		s3ePointerUpdate();
		environmentCtrl.UpdateInputs();

		Update(delta, absolute);
		Draw(delta, absolute);

		environmentCtrl.CheckInputs();

		// sleep 0ms (for input events update)
		s3eDeviceYield(0);
	}
}
void MGameEngine::Terminate(){
	IwResManagerTerminate();
	Iw2DTerminate();
	IwGxTerminate();
}