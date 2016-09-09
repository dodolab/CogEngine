#include "MGameFactory.h"
#include "SmartPointer.h"
#include "GNode.h"
#include "MGameEngine.h"
#include "BeRender.h"
#include "BeRotateAnim.h"
#include "BeHitEvent.h"

GNode* CreateFirst(){
	GNode* output = new GNode(ObjType::SCENE, 1, "first");

	// Load images
	spt<ofImage> image = MEngine.resourceCtrl->Get2DImage("images/blue.png");
	spt<ofImage> rytmus = MEngine.resourceCtrl->Get2DImage("images/rytmus.png");

	int pixels = COGGetHeight();
	int normPixels = 400;
	float scale = 0.0243*(((float)pixels) / normPixels);

	GNode* actual = output;
	ofColor color;
	ofVec3f actualSize = ofVec3f(300, 300);

	for (int i = 0; i < 1; i++){
		color.set(10 + i * 4, 100 + i * 20, 0);
		GNode* child = new GNode(ObjType::OBJECT, 0, "fofka");
		child->GetTransform().LocalPos = actualSize;
		if (i == 0){
			child->GetTransform().LocalPos = ofVec3f(COGGetWidth() / 2, COGGetHeight() / 2);
			child->GetTransform().Scale =  ofVec3f(scale * 30,scale * 30,scale * 30);
			child->AddAttr(Attrs::IMGSOURCE, rytmus);
			child->SetState(States::HITTABLE);
			child->AddBehavior(new BeHitEvent());
			child->AddBehavior(new BeRender(RenderType::IMAGE));
			child->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		}

		/*	child->AddAttr(Attrs::COLOR, color);
		child->AddAttr(Attrs::SIZE, actualSize);
		child->AddBehavior(new BeRender(RenderType::RECTANGLE));
		child->AddBehavior(new BeRotateAnim(0, 0, 2, false));*/
		actual->AddChild(child);

		color.set(100 + i * 20, 10 + i * 4, 0);
		GNode* sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = ofVec3f(actualSize.x / 2, actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE));
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->AddBehavior(new BeHitEvent());
		sChild->GetTransform().Scale = ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = ofVec3f(-actualSize.x / 2, actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE));
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->AddBehavior(new BeHitEvent());
		sChild->GetTransform().Scale = ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = ofVec3f(actualSize.x / 2, -actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE));
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->AddBehavior(new BeHitEvent());
		sChild->GetTransform().Scale = ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = ofVec3f(-actualSize.x / 2, -actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE));
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->AddBehavior(new BeHitEvent());
		sChild->GetTransform().Scale = ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		actual = child;
		actualSize = actualSize / 2;
	}

	return output;
}

GNode* CreateSecond(){
	GNode* output = new GNode(ObjType::SCENE, 2, "second");

	int pixels = COGGetHeight();
	int normPixels = 400;
	float scale = 0.0243*(((float)pixels) / normPixels);

	GNode* actual = output;
	ofColor color;
	ofVec3f actualSize = ofVec3f(300, 300);

	for (int i = 0; i < 5; i++){
		color.set(10 + i * 4, 100 + i * 20, 0);
		GNode* child = new GNode(ObjType::OBJECT, 0, "fofka");
		child->GetTransform().LocalPos = actualSize;
		if (i == 0){
			child->SetSubType(111);
			child->GetTransform().LocalPos = ofVec3f(COGGetWidth() / 2, COGGetHeight() / 2);
			child->GetTransform().Scale = ofVec3f(scale * 30, scale * 30, scale * 30);
			child->SetState(States::HITTABLE);
			child->AddBehavior(new BeHitEvent());
		}
		child->AddBehavior(new BeRender(RenderType::RECTANGLE));
		child->AddBehavior(new BeRotateAnim(0, 0, 12, false));

		child->AddAttr(Attrs::COLOR, color);
		child->AddAttr(Attrs::SIZE, actualSize);
		actual->AddChild(child);

		color.set(100 + i * 20, 10 + i * 4, 0);
		GNode* sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = ofVec3f(actualSize.x / 2, actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::RECTANGLE));
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->GetTransform().Scale = ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		if (i == 1){
			sChild = new GNode(ObjType::OBJECT, 123, "CHECK");
			color.set(0,0,255);
		}
		else {
			sChild = new GNode(ObjType::OBJECT, 0, "other");
		}
		sChild->GetTransform().LocalPos = ofVec3f(-actualSize.x / 2, actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::RECTANGLE));
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->GetTransform().Scale =ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		color.set(100 + i * 20, 10 + i * 4, 0);
		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = ofVec3f(actualSize.x / 2, -actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::RECTANGLE));
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->GetTransform().Scale = ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().LocalPos = ofVec3f(-actualSize.x / 2, -actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::RECTANGLE));
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false));
		sChild->GetTransform().Scale = ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild);

		actual = child;
		actualSize = actualSize / 2;
	}

	return output;
}

GNode* CreateThird(){

	GNode* output = new GNode(ObjType::SCENE, 3, "sthird");
	spt<ofImage> rytmus = MEngine.resourceCtrl->Get2DImage("images/rytmus.png");
	spt<ofImage> image = MEngine.resourceCtrl->Get2DImage("images/blue.png");

	int pixels = COGGetHeight();
	int normPixels = 400;
	float scale = 0.0243*(((float)pixels) / normPixels);

	GNode* child = new GNode(ObjType::OBJECT, 10, "other");
	child->GetTransform().LocalPos = ofVec3f(COGGetWidth() / 2, COGGetHeight() / 2);
	child->GetTransform().Scale = ofVec3f(scale * 10, scale * 10, scale * 10);
	child->AddAttr(Attrs::IMGSOURCE, rytmus);
	child->SetState(States::HITTABLE);
	child->AddBehavior(new BeHitEvent());
	child->AddBehavior(new BeRender(RenderType::IMAGE));
	child->AddBehavior(new BeRotateAnim(0, 0, 20, false));

	output->AddChild(child);

	for (int i = 0; i < 2000; i++){
		GNode* particle = new GNode(ObjType::OBJECT, 0, "other");

		ofVec3f randomTransform(ofRandom(1, COGGetWidth()), ofRandom(COGGetHeight() / 2 - COGGetWidth() / 2,
			COGGetHeight() / 2 + COGGetWidth() / 2));

		particle->GetTransform().LocalPos = randomTransform;
		particle->GetTransform().Scale = ofVec3f(scale * 2, scale * 2, scale * 2);
		particle->GetTransform().RotationOrigin = ofVec3f(COGGetWidth() / 2, COGGetHeight() / 2) - particle->GetTransform().LocalPos;
		particle->AddAttr<spt<ofImage>>(Attrs::IMGSOURCE, image);
	
		ofColor color;
		color.set(0,0,222);
		particle->AddAttr(Attrs::COLOR, color);
		particle->AddAttr(Attrs::SIZE, ofVec3f(250,250));
		particle->AddBehavior(new BeRender(RenderType::IMAGE));
		float speed = particle->GetTransform().RotationOrigin.length() / 6;
		BeRotateAnim* rot = new BeRotateAnim(0, 0, speed, false);
		particle->AddBehavior(rot);

		output->AddChild(particle);

	}

	return output;
}

GNode* CreateMask(int width, int height, ofVec3f position){
	GNode* mask = new GNode(ObjType::OBJECT, 0, "maska");
	mask->AddBehavior(new BeRender(RenderType::RECTANGLE));
	ofColor color;
	color.set(0, 0, 0);
	mask->AddAttr(Attrs::COLOR, color);
	mask->AddAttr(Attrs::SIZE, ofVec3f(width,height));
	mask->GetTransform().LocalPos = position;

	return mask;
}



ofVec2f MGameFactory::GetCenter(){
	return ofVec2f(COGGetWidth() / 2, COGGetHeight() / 2);
}


void MGameFactory::SetTransformRelPixel(GNode* node, float posX, float posY, ofVec2f scale){
	node->GetTransform().LocalPos = ofVec3f(posX, posY);
	node->GetTransform().Scale = ofVec3f(scale.x, scale.y, 1);
}

// TODO !!!!! create Renderable entity with ANCHORS

void MGameFactory::SetRenderImage(GNode* node, string imgPath, bool isHittable){
	node->AddBehavior(new BeRender(RenderType::IMAGE));
	spt<ofImage> img = MEngine.resourceCtrl->Get2DImage(imgPath);
	node->AddAttr(Attrs::IMGSOURCE, img);

	if(isHittable) node->SetState(States::HITTABLE);
}

void MGameFactory::SetRenderImage(GNode* node, string imgPath, float pScaleX, ofVec2f pPos, ofVec2f anchor){
	node->AddBehavior(new BeRender(RenderType::IMAGE));
	spt<ofImage> img = MEngine.resourceCtrl->Get2DImage(imgPath);
	node->AddAttr(Attrs::IMGSOURCE, img);
	
	ofVec2f scrSize = MEngine.GetSize();
	ofVec2f pos(pPos.x*scrSize.x, pPos.y*scrSize.y);

	float calcScale = pScaleX * MEngine.GetSize().x / img->getWidth();
	ofVec2f scale(calcScale);

	pos.x += (0.5f - anchor.x) * img->getWidth()*scale.x;
	pos.y += (0.5f - anchor.y) * img->getHeight()*scale.y;

	SetTransformRelPixel(node, pos.x, pos.y, scale);

	node->GetTransform().SetAbsAsLocal();

}

void MGameFactory::SetRenderImageAbsolute(GNode* node, string imgPath, ofVec2f pos, bool absolutePos, float scaleX, bool absoluteScale, ofVec2f anchor,GNode* parent){

	node->AddBehavior(new BeRender(RenderType::IMAGE));
	spt<ofImage> img = MEngine.resourceCtrl->Get2DImage(imgPath);
	node->AddAttr(Attrs::IMGSOURCE, img);

	SetTransformAbs(node, pos, absolutePos, scaleX, absoluteScale, anchor, img->getWidth(), img->getHeight(), parent);

}

ofVec2f MGameFactory::CalcPositionAbsolute(GNode* node, ofVec2f pos, bool absolutePos, ofVec2f anchor, GNode* parent){
	ofVec2f scrSize = MEngine.GetSize();
	EnTransform& parentTrans = parent->GetTransform();
	ofVec2f absPos;

	if (absolutePos){
		absPos = ofVec2f((pos.x - parentTrans.AbsPos.x) / parentTrans.AbsScale.x, (pos.y - parentTrans.AbsPos.y) / parentTrans.AbsScale.y);
	}
	else{
		absPos = ofVec2f((pos.x*scrSize.x - parentTrans.AbsPos.x) / parentTrans.AbsScale.x, (pos.y*scrSize.y - parentTrans.AbsPos.y) / parentTrans.AbsScale.y);
	}

	return absPos;
}

ofVec3f MGameFactory::CalsScaleAbsolute(GNode* node, float scaleX, float width, bool absoluteScale, GNode* parent){
	ofVec2f scrSize = MEngine.GetSize();
	EnTransform& parentTrans = parent->GetTransform();
	float calcScale = absoluteScale ? scaleX : (scaleX* scrSize.x / width) / parentTrans.AbsScale.x;

	return ofVec3f(calcScale);
}

void MGameFactory::SetTransformAbs(GNode* node, ofVec2f pos, bool absolutePos, float scaleX, bool absoluteScale, ofVec2f anchor,int width, int height, GNode* parent){
	ofVec2f scrSize = MEngine.GetSize();

	EnTransform& parentTrans = parent->GetTransform();

	float calcScale = absoluteScale ? scaleX : (scaleX* scrSize.x / width) / parentTrans.AbsScale.x;
	ofVec2f scale(calcScale);
	ofVec2f absPos;

	if (absolutePos){
		absPos = ofVec2f((pos.x - parentTrans.AbsPos.x) / parentTrans.AbsScale.x, (pos.y - parentTrans.AbsPos.y) / parentTrans.AbsScale.y);
	}
	else{
		absPos = ofVec2f((pos.x*scrSize.x - parentTrans.AbsPos.x) / parentTrans.AbsScale.x, (pos.y*scrSize.y - parentTrans.AbsPos.y) / parentTrans.AbsScale.y);
	}


	absPos.x += (0.5f - anchor.x) * width*scale.x;
	absPos.y += (0.5f - anchor.y) * height*scale.y;

	SetTransformRelPixel(node, absPos.x, absPos.y, scale);

	node->GetTransform().CalcAbsTransform(parent->GetTransform());
}


ofVec3f MGameFactory::GetPercentageVec(float xPerc, float yPerc){
	float width = COGGetWidth();
	float height = COGGetHeight();

	return ofVec3f(width*xPerc, height*yPerc);
}



GNode* MGameFactory::CreateRoot(){

	GNode* root = new GNode(ObjType::ROOT, 0, "root");
	sceneManager = new BeSceneManager();
	root->AddBehavior(sceneManager);

	return root;
}

void MGameFactory::SwitchToScene(int scene){
	// nothing to do here...
}