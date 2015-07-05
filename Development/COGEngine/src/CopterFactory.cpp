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
#include "CopterCanon.h"
#include "CopterBullet.h"
#include "CopterHelicopter.h"
#include "CopterCollisionMgr.h"
#include "CopterHeliMgr.h"
#include "CopterPara.h"
#include "CopterLives.h"
#include "CopterScore.h"

void CopterFactory::OnBackgroundhit(const uint64 delta, const uint64 absolute, const ofMatrix4x4& absMatrix, GNode* owner){
MEngine.factory->SwitchToScene(0);
}


GNode* CopterFactory::CreateRoot(){
	//return MGameFactory::CreateRoot();
	 root = new GNode(ObjType::ROOT, 12, "fofík");

	 splash = new GNode(ObjType::SCENE, 222, "splash");

    this->SetRenderImage(splash, "images/intro.png", 1, ofVec2f(0.5f, 0.5f), ofVec2f(0.5f, 0.5f));
	splash->SetState(States::HITTABLE);
	splash->AddBehavior(new BeHitEvent());
	
	CopterSceneManager* sceneMgr = new CopterSceneManager(this);
	MEngine.storage->RegisterListener(Actions::OBJECT_HIT, sceneMgr);
	
	splash->AddBehavior(sceneMgr);

	root->AddChild(splash);
	return root;
}

void CopterFactory::SwitchToScene(int sc){
	if (sc == 1){
		// go back to background
		scene->AddBehavior(new BeSlideTween(TweenDirection::RIGHT, splash, scene, 1));
		return;
	}

	if (scene != nullptr){
		root->RemoveChild(scene,true);
		// todo: remove child immediately
		delete scene;
	}

	scene = new GNode(ObjType::SCENE, 14, "scene");
	scene->AddBehavior(new BeRender(RenderType::IMAGE));
	scene->AddBehavior(new BeCollider(States::COLLID_SOURCE, States::COLLID_TARGET));
	scene->AddBehavior(new CopterCollisionMgr(this));
	scene->AddBehavior(new CopterHeliManager(this));

	spt<ofImage> img = MEngine.resourceCtrl->Get2DImage("images/background.png");
	scene->AddAttr(Attrs::IMGSOURCE, img);
	float sceneScale = MEngine.environmentCtrl->GetWidth() / img->getWidth();

	int width = MEngine.environmentCtrl->GetWidth();
	int height = MEngine.environmentCtrl->GetHeight();

	scene->GetTransform().LocalPos = ofVec3f(width/2+width, height/2);
	scene->GetTransform().Scale = ofVec3f(sceneScale, sceneScale, sceneScale);
	root->AddChild(scene);
	// immediately update absolute positions
	root->UpdateTransforms();

	scene->AddBehavior(new BeSlideTween(TweenDirection::LEFT, scene, splash,1));

	GNode* sun = new GNode(ObjType::OBJECT, 10, "sun");
	sun->AddBehavior(new BeRender(RenderType::IMAGE));
	img = MEngine.resourceCtrl->Get2DImage("images/sun.png");
	sun->AddAttr(Attrs::IMGSOURCE, img);
	sun->GetTransform().LocalPos = ofVec3f(RelPosX(70, scene), RelPosY(10, scene), 2);
	sun->GetTransform().Scale = CalcScale(img, 10, scene);
	sun->AddBehavior(new BeRotateAnim(0, 0, 10, false));
	scene->AddChild(sun);

	GNode* leftBut = new GNode(ObjType::OBJECT, 50, "but_left");
	leftBut->AddBehavior(new BeRender(RenderType::IMAGE));
	img = MEngine.resourceCtrl->Get2DImage("images/butLeft.png");
	leftBut->AddAttr(Attrs::IMGSOURCE, img);
	leftBut->SetState(States::HITTABLE);
	leftBut->AddBehavior(new BeHitEvent());
	leftBut->GetTransform().LocalPos = ofVec3f(RelPosX(10,scene), RelPosY(50,scene),2);
	leftBut->GetTransform().Scale = CalcScale(img, 10,scene);
	scene->AddChild(leftBut);


	GNode* rightBut = new GNode(ObjType::OBJECT, 50, "but_right");
	rightBut->AddBehavior(new BeRender(RenderType::IMAGE));
	img = MEngine.resourceCtrl->Get2DImage("images/butRight.png");
	rightBut->AddAttr(Attrs::IMGSOURCE, img);
	rightBut->SetState(States::HITTABLE);
	rightBut->AddBehavior(new BeHitEvent());
	rightBut->GetTransform().LocalPos = ofVec3f(RelPosX(21,scene), RelPosY(50,scene),2);
	rightBut->GetTransform().Scale = CalcScale(img, 10,scene);
	scene->AddChild(rightBut);

	GNode* fireBut = new GNode(ObjType::OBJECT, 50, "but_fire");
	fireBut->AddBehavior(new BeRender(RenderType::IMAGE));
	img = MEngine.resourceCtrl->Get2DImage("images/butFire.png");
	fireBut->AddAttr(Attrs::IMGSOURCE, img);
	fireBut->SetState(States::HITTABLE);
	fireBut->AddBehavior(new BeHitEvent());
	fireBut->GetTransform().LocalPos = ofVec3f(RelPosX(80,scene), RelPosY(50,scene),2);
	fireBut->GetTransform().Scale = CalcScale(img, 10,scene);
	scene->AddChild(fireBut);

	GNode* canon = new GNode(ObjType::OBJECT, 23, "canon");
	canon->AddBehavior(new BeRender(RenderType::IMAGE));
	img = MEngine.resourceCtrl->Get2DImage("images/canon.png");
	canon->AddAttr(Attrs::IMGSOURCE, img);
	canon->GetTransform().RotationOrigin = ofVec3f(0, -img->height / 2, 0);
	canon->GetTransform().LocalPos = ofVec3f(RelPosX(50, scene), RelPosY(44, scene) + (img->height / 2), 2);
	canon->GetTransform().Scale = CalcScale(img, 0.75f, scene);
	canon->AddBehavior(new CopterCanon(this));
	scene->AddChild(canon);

	// register onbutton hit
	MEngine.storage->RegisterCallback(Actions::OBJECT_HIT, &CopterFactory::OnButtonHit);
	MEngine.storage->RegisterCallback(Actions::OBJECT_RELEASED, &CopterFactory::OnButtonReleased);

	// player lives
	GNode* lives = new GNode(ObjType::OBJECT, 24, "lives");
	lives->AddAttr(Attrs::FONT, MEngine.resourceCtrl->GetFont("fonts/verdana.ttf",14));
	lives->AddAttr(Attrs::TEXT, string("LIVES: 10"));
	lives->AddBehavior(new BeRender(RenderType::TEXT));
	lives->AddBehavior(new CopterLives(10));
	lives->GetTransform().LocalPos = ofVec3f(RelPosX(80, scene), RelPosY(7, scene), 2);
	scene->AddChild(lives);

	// player score
	GNode* score = new GNode(ObjType::OBJECT, 24, "score");
	score->AddAttr(Attrs::FONT, MEngine.resourceCtrl->GetFont("fonts/verdana.ttf", 14));
	score->AddAttr(Attrs::TEXT, string("SCORE: 0"));
	score->AddBehavior(new BeRender(RenderType::TEXT));
	score->AddBehavior(new CopterScore());
	score->GetTransform().LocalPos = ofVec3f(RelPosX(5, scene), RelPosY(7, scene), 2);
	scene->AddChild(score);
	

   // int id = MEngine.storage->RegisterCallback(12, &CopterFactory::Test);
//	MEngine.storage->UnregisterCallback(12, id);
}

GNode* CopterFactory::CreateBullet(GNode* canon){
	GNode* scene = canon->GetSceneRoot();
	GNode* bullet = new GNode(ObjType::OBJECT, 23, "bullet");
	bullet->AddBehavior(new BeRender(RenderType::IMAGE));
	spt<ofImage> img = MEngine.resourceCtrl->Get2DImage("images/bullet.png");
	bullet->AddAttr(Attrs::IMGSOURCE, img);
	bullet->SetGroup(States::COLLID_SOURCE);
	bullet->AddAttr(Attrs::BOUNDS, new EnBounds());

	spt<ofImage> canonImg = canon->GetAttr<spt<ofImage>>(Attrs::IMGSOURCE);
	float canonRot = canon->GetTransform().Rotation*PI/180.0f;
	bullet->GetTransform().LocalPos = ofVec3f(RelPosX(50, scene) + canonImg->height*sin(canonRot), RelPosY(44, scene) + canonImg->height / 4 - canonImg->height / 2 * cos(canonRot), 2);
	bullet->GetTransform().Scale = CalcScale(img, 0.75f, scene);
	bullet->GetTransform().Rotation = canon->GetTransform().Rotation;
	bullet->AddBehavior(new CopterBullet(this));

	return bullet;
}

GNode* CopterFactory::CreateHelicopter(GNode* scene){

	GNode* copter = new GNode(ObjType::OBJECT, 23, "copter");
	copter->AddBehavior(new BeRender(RenderType::IMAGE));
	spt<ofImage> img = MEngine.resourceCtrl->Get2DImage("images/copter1.png");
	copter->AddAttr(Attrs::IMGSOURCE, img);
	copter->AddBehavior(new CopterHelicopter(this));
	copter->AddAttr(Attrs::FRAME, 1);
	copter->SetGroup(States::COLLID_TARGET);
	copter->AddAttr(Attrs::HEALTH, 40);
	copter->AddAttr(Attrs::BOUNDS, new EnBounds());

	if (ofRandom(0, 2) == 0){
		// from left to right
		copter->SetState(States::TO_RIGHT);
		copter->GetTransform().Scale = CalcScale(img, 8, scene);
		copter->GetTransform().LocalPos = ofVec3f(RelPosX(-50, scene), RelPosY(ofRandom(10, 20), scene), 2);
		copter->GetTransform().Scale.x = -copter->GetTransform().Scale.x;
		copter->AddAttr(Attrs::VELOCITY, ofVec2f(2,0));
	}
	else{
		copter->SetState(States::TO_LEFT);
		copter->GetTransform().Scale = CalcScale(img, 8, scene);
		copter->GetTransform().LocalPos = ofVec3f(RelPosX(150,scene), RelPosY(ofRandom(10, 20), scene), 2);
		copter->AddAttr(Attrs::VELOCITY, ofVec2f(-2, 0));
	}

	return copter;
}

GNode* CopterFactory::CreatePara(GNode* copter){
	GNode* scene = copter->GetSceneRoot();
	GNode* para = new GNode(ObjType::OBJECT, 23, "para");
	para->AddBehavior(new BeRender(RenderType::IMAGE));
	spt<ofImage> img = MEngine.resourceCtrl->Get2DImage("images/para.png");
	para->AddAttr(Attrs::IMGSOURCE, img);
	para->SetGroup(States::COLLID_TARGET);
	para->AddAttr(Attrs::BOUNDS, new EnBounds());
	para->AddAttr(Attrs::HEALTH, 5);

	para->GetTransform().LocalPos = copter->GetTransform().LocalPos;
	para->GetTransform().Scale = CalcScale(img, 2.5f, scene);
	para->AddBehavior(new CopterPara(this));

	return para;
}