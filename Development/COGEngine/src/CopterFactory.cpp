#include "CopterFactory.h"
#include "BeTranslateAnim.h"
#include "BeRotateAnim.h"
#include "BeMove.h"
#include "BeHitEvent.h"
#include "BeCollider.h"
#include "BeRender.h"
#include "SmartPointer.h"
#include "BeTween.h"
#include "CopterSceneManager.h"

void CopterFactory::OnBackgroundhit(const uint64 delta, const uint64 absolute, const ofMatrix4x4& absMatrix, GNode* owner){
MEngine.factory->SwitchToScene(0);
}


GNode* CopterFactory::CreateRoot(){
	return MGameFactory::CreateRoot();
	/* root = new GNode(ObjType::ROOT, 12, "fofík");
	 splash = new GNode(ObjType::SCENE, 13, "scéna");

    this->SetRenderImage(splash, "images/intro.png", 1, ofVec2f(0.5f, 0.5f), ofVec2f(0.5f, 0.5f));
	splash->SetState(States::HITTABLE);
	splash->AddBehavior(new BeHitEvent(-1));
	splash->AddBehavior(new CopterSceneManager(this));

	root->AddChild(splash);
	return root;*/
}

void CopterFactory::SwitchToScene(int sc){
	GNode* scene = new GNode(ObjType::SCENE, 14, "scene");
	scene->AddBehavior(new BeRender(RenderType::IMAGE));
	spt<ofImage> img = MEngine.resourceCtrl->Get2DImage("images/background.png");
	scene->AddAttr(Attrs::IMGSOURCE, img);
	float sceneScale = MEngine.environmentCtrl->GetWidth() / img->getWidth();

	int width = MEngine.environmentCtrl->GetWidth();
	int height = MEngine.environmentCtrl->GetHeight();

	scene->GetTransform().LocalPos = ofVec3f(width/2+width, height/2);
	scene->GetTransform().Scale = ofVec3f(sceneScale, sceneScale, sceneScale);

	root->AddChild(scene);
	scene->AddBehavior(new BeSlideTween(TweenDirection::LEFT, scene, splash,1));


	GNode* leftBut = new GNode(ObjType::OBJECT, 50, "b");
	leftBut->AddBehavior(new BeRender(RenderType::IMAGE));
	img = MEngine.resourceCtrl->Get2DImage("images/butLeft.png");
	leftBut->AddAttr(Attrs::IMGSOURCE, img);
	leftBut->SetState(States::HITTABLE);
	leftBut->GetTransform().LocalPos = ofVec3f(RelPosX(10,scene), RelPosY(50,scene),2);
	leftBut->GetTransform().Scale = CalcScale(img, 10,scene);
	scene->AddChild(leftBut);


	GNode* rightBut = new GNode(ObjType::OBJECT, 50, "b");
	rightBut->AddBehavior(new BeRender(RenderType::IMAGE));
	img = MEngine.resourceCtrl->Get2DImage("images/butRight.png");
	rightBut->AddAttr(Attrs::IMGSOURCE, img);
	rightBut->SetState(States::HITTABLE);
	rightBut->GetTransform().LocalPos = ofVec3f(RelPosX(21,scene), RelPosY(50,scene),2);
	rightBut->GetTransform().Scale = CalcScale(img, 10,scene);
	scene->AddChild(rightBut);

	GNode* fireBut = new GNode(ObjType::OBJECT, 50, "b");
	fireBut->AddBehavior(new BeRender(RenderType::IMAGE));
	img = MEngine.resourceCtrl->Get2DImage("images/butFire.png");
	fireBut->AddAttr(Attrs::IMGSOURCE, img);
	fireBut->SetState(States::HITTABLE);
	fireBut->GetTransform().LocalPos = ofVec3f(RelPosX(80,scene), RelPosY(50,scene),2);
	fireBut->GetTransform().Scale = CalcScale(img, 10,scene);
	scene->AddChild(fireBut);

	GNode* copter = nullptr;

	for(int i=0; i<10; i++){
		copter = new GNode(ObjType::OBJECT, 50, "b");
		copter->AddBehavior(new BeRender(RenderType::IMAGE));
		img = MEngine.resourceCtrl->Get2DImage("images/copter1.png");
		copter->AddAttr(Attrs::IMGSOURCE, img);
		copter->SetState(States::HITTABLE);

		int randX = ofRandom(-50,50);
		int randY = ofRandom(10,30);
		int randM = ofRandom(0,2);
		if(randM == 1) randX = 150+randX;

		copter->GetTransform().LocalPos = ofVec3f(RelPosX(randX,scene), RelPosY(randY,scene),2);
		copter->GetTransform().Scale = CalcScale(img, 10,scene);
		copter->AddBehavior(new BeTranslateAnim(ofVec3f(RelPosX(randX,scene),RelPosY(randY,scene),2),ofVec3f(RelPosX(randM == 0 ? 130 : -130,scene),RelPosY(randY,scene),2),0.1f,false,true));
		copter->AddBehavior(new CopterBeh());
		scene->AddChild(copter);
	}

    int id = MEngine.storage->RegisterCallback(12, &CopterFactory::Test);
	MEngine.storage->UnregisterCallback(12, id);
}
