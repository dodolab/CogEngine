#include "MGameFactory.h"
#include "ofxSmartPointer.h"
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
		child->GetTransform().localPos = actualSize;
		if (i == 0){
			child->GetTransform().localPos = ofVec3f(COGGetWidth() / 2, COGGetHeight() / 2);
			child->GetTransform().scale =  ofVec3f(scale * 30,scale * 30,scale * 30);
			child->AddAttr(Attrs::IMGSOURCE, rytmus);
			child->SetState(States::HITTABLE);
			child->AddBehavior(new BeHitEvent(false), true);
			child->AddBehavior(new BeRender(RenderType::IMAGE), true);
			child->AddBehavior(new BeRotateAnim(0, 0, 2, false), true);
		}

		/*	child->AddAttr(Attrs::COLOR, color);
		child->AddAttr(Attrs::SIZE, actualSize);
		child->AddBehavior(new BeRender(RenderType::RECTANGLE));
		child->AddBehavior(new BeRotateAnim(0, 0, 2, false));*/
		actual->AddChild(child, true);

		color.set(100 + i * 20, 10 + i * 4, 0);
		GNode* sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().localPos = ofVec3f(actualSize.x / 2, actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE), true);
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false), true);
		sChild->AddBehavior(new BeHitEvent(false), true);
		sChild->GetTransform().scale = ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild, true);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().localPos = ofVec3f(-actualSize.x / 2, actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE), true);
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false), true);
		sChild->AddBehavior(new BeHitEvent(false), true);
		sChild->GetTransform().scale = ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild, true);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().localPos = ofVec3f(actualSize.x / 2, -actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE), true);
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false), true);
		sChild->AddBehavior(new BeHitEvent(false), true);
		sChild->GetTransform().scale = ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild, true);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().localPos = ofVec3f(-actualSize.x / 2, -actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::IMAGE), true);
		sChild->AddAttr(Attrs::IMGSOURCE, image);
		sChild->SetState(States::HITTABLE);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false), true);
		sChild->AddBehavior(new BeHitEvent(false), true);
		sChild->GetTransform().scale = ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild, true);

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
		child->GetTransform().localPos = actualSize;
		if (i == 0){
			child->SetSubType(111);
			child->GetTransform().localPos = ofVec3f(COGGetWidth() / 2, COGGetHeight() / 2);
			child->GetTransform().scale = ofVec3f(scale * 30, scale * 30, scale * 30);
			child->SetState(States::HITTABLE);
			child->AddBehavior(new BeHitEvent(false), true);
		}
		child->AddBehavior(new BeRender(RenderType::RECTANGLE), true);
		child->AddBehavior(new BeRotateAnim(0, 0, 12, false), true);

		child->AddAttr(Attrs::COLOR, color);
		child->AddAttr(Attrs::SIZE, actualSize);
		actual->AddChild(child, true);

		color.set(100 + i * 20, 10 + i * 4, 0);
		GNode* sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().localPos = ofVec3f(actualSize.x / 2, actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::RECTANGLE), true);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false), true);
		sChild->GetTransform().scale = ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild, true);

		if (i == 1){
			sChild = new GNode(ObjType::OBJECT, 123, "CHECK");
			color.set(0,0,255);
		}
		else {
			sChild = new GNode(ObjType::OBJECT, 0, "other");
		}
		sChild->GetTransform().localPos = ofVec3f(-actualSize.x / 2, actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::RECTANGLE), true);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false), true);
		sChild->GetTransform().scale =ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild, true);

		color.set(100 + i * 20, 10 + i * 4, 0);
		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().localPos = ofVec3f(actualSize.x / 2, -actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::RECTANGLE), true);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false), true);
		sChild->GetTransform().scale = ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild, true);

		sChild = new GNode(ObjType::OBJECT, 0, "other");
		sChild->GetTransform().localPos = ofVec3f(-actualSize.x / 2, -actualSize.y / 2);
		sChild->AddAttr(Attrs::COLOR, color);
		sChild->AddAttr(Attrs::SIZE, actualSize / 4);
		sChild->AddBehavior(new BeRender(RenderType::RECTANGLE), true);
		sChild->AddBehavior(new BeRotateAnim(0, 0, 2, false), true);
		sChild->GetTransform().scale = ofVec3f(0.4f,0.4f,0.4f);
		child->AddChild(sChild, true);

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
	child->GetTransform().localPos = ofVec3f(COGGetWidth() / 2, COGGetHeight() / 2);
	child->GetTransform().scale = ofVec3f(scale * 10, scale * 10, scale * 10);
	child->AddAttr(Attrs::IMGSOURCE, rytmus);
	child->SetState(States::HITTABLE);
	child->AddBehavior(new BeHitEvent(false), true);
	child->AddBehavior(new BeRender(RenderType::IMAGE), true);
	child->AddBehavior(new BeRotateAnim(0, 0, 20, false), true);

	output->AddChild(child, true);

	for (int i = 0; i < 2000; i++){
		GNode* particle = new GNode(ObjType::OBJECT, 0, "other");

		ofVec3f randomTransform(ofRandom(1, COGGetWidth()), ofRandom(COGGetHeight() / 2 - COGGetWidth() / 2,
			COGGetHeight() / 2 + COGGetWidth() / 2));

		particle->GetTransform().localPos = randomTransform;
		particle->GetTransform().scale = ofVec3f(scale * 2, scale * 2, scale * 2);
		particle->GetTransform().rotationOrigin = ofVec3f(COGGetWidth() / 2, COGGetHeight() / 2) - particle->GetTransform().localPos;
		particle->AddAttr<spt<ofImage>>(Attrs::IMGSOURCE, image);
	
		ofColor color;
		color.set(0,0,222);
		particle->AddAttr(Attrs::COLOR, color);
		particle->AddAttr(Attrs::SIZE, ofVec3f(250,250));
		particle->AddBehavior(new BeRender(RenderType::IMAGE), true);
		float speed = particle->GetTransform().rotationOrigin.length() / 6;
		BeRotateAnim* rot = new BeRotateAnim(0, 0, speed, false);
		particle->AddBehavior(rot, true);

		output->AddChild(particle, true);

	}

	return output;
}

GNode* CreateMask(int width, int height, ofVec3f position){
	GNode* mask = new GNode(ObjType::OBJECT, 0, "maska");
	mask->AddBehavior(new BeRender(RenderType::RECTANGLE), true);
	ofColor color;
	color.set(0, 0, 0);
	mask->AddAttr(Attrs::COLOR, color);
	mask->AddAttr(Attrs::SIZE, ofVec3f(width,height));
	mask->GetTransform().localPos = position;

	return mask;
}



ofVec2f MGameFactory::GetCenter(){
	return ofVec2f(COGGetWidth() / 2, COGGetHeight() / 2);
}


void MGameFactory::SetTransformRelPixel(GNode* node, float posX, float posY, ofVec2f scale){
	node->GetTransform().localPos = ofVec3f(posX, posY);
	node->GetTransform().scale = ofVec3f(scale.x, scale.y, 1);
}

// TODO !!!!! create Renderable entity with ANCHORS

void MGameFactory::SetRenderImage(GNode* node, string imgPath, bool isHittable){
	node->AddBehavior(new BeRender(RenderType::IMAGE), true);
	spt<ofImage> img = MEngine.resourceCtrl->Get2DImage(imgPath);
	node->AddAttr(Attrs::IMGSOURCE, img);

	if(isHittable) node->SetState(States::HITTABLE);
}

void MGameFactory::SetRenderImage(GNode* node, string imgPath, float pScaleX, ofVec2f pPos, ofVec2f anchor){
	node->AddBehavior(new BeRender(RenderType::IMAGE), true);
	spt<ofImage> img = MEngine.resourceCtrl->Get2DImage(imgPath);
	node->AddAttr(Attrs::IMGSOURCE, img);
	
	ofVec2f scrSize = COGGetSize();
	ofVec2f pos(pPos.x*scrSize.x, pPos.y*scrSize.y);

	float calcScale = pScaleX * COGGetWidth() / img->getWidth();
	ofVec2f scale(calcScale);

	pos.x += (0.5f - anchor.x) * img->getWidth()*scale.x;
	pos.y += (0.5f - anchor.y) * img->getHeight()*scale.y;

	SetTransformRelPixel(node, pos.x, pos.y, scale);

	node->GetTransform().SetAbsAsLocal();

}

void MGameFactory::SetRenderImageAbsolute(GNode* node, string imgPath, ofVec2f pos, bool absolutePos, float scaleX, bool absoluteScale, ofVec2f anchor,GNode* parent){

	node->AddBehavior(new BeRender(RenderType::IMAGE), true);
	spt<ofImage> img = MEngine.resourceCtrl->Get2DImage(imgPath);
	node->AddAttr(Attrs::IMGSOURCE, img);

	SetTransformAbs(node, pos, absolutePos, scaleX, absoluteScale, anchor, img->getWidth(), img->getHeight(), parent);

}

ofVec2f MGameFactory::CalcPositionAbsolute(GNode* node, ofVec2f pos, bool absolutePos, ofVec2f anchor, GNode* parent){
	ofVec2f scrSize = COGGetSize();
	EnTransform& parentTrans = parent->GetTransform();
	ofVec2f absPos;

	if (absolutePos){
		absPos = ofVec2f((pos.x - parentTrans.absPos.x) / parentTrans.absScale.x, (pos.y - parentTrans.absPos.y) / parentTrans.absScale.y);
	}
	else{
		absPos = ofVec2f((pos.x*scrSize.x - parentTrans.absPos.x) / parentTrans.absScale.x, (pos.y*scrSize.y - parentTrans.absPos.y) / parentTrans.absScale.y);
	}

	return absPos;
}

ofVec3f MGameFactory::CalsScaleAbsolute(GNode* node, float scaleX, float width, bool absoluteScale, GNode* parent){
	ofVec2f scrSize = COGGetSize();
	EnTransform& parentTrans = parent->GetTransform();
	float calcScale = absoluteScale ? scaleX : (scaleX* scrSize.x / width) / parentTrans.absScale.x;

	return ofVec3f(calcScale);
}

void MGameFactory::SetTransformAbs(GNode* node, ofVec2f pos, bool absolutePos, float scaleX, bool absoluteScale, ofVec2f anchor,int width, int height, GNode* parent){
	ofVec2f scrSize = COGGetSize();

	EnTransform& parentTrans = parent->GetTransform();

	float calcScale = absoluteScale ? scaleX : (scaleX* scrSize.x / width) / parentTrans.absScale.x;
	ofVec2f scale(calcScale);
	ofVec2f absPos;

	if (absolutePos){
		absPos = ofVec2f((pos.x - parentTrans.absPos.x) / parentTrans.absScale.x, (pos.y - parentTrans.absPos.y) / parentTrans.absScale.y);
	}
	else{
		absPos = ofVec2f((pos.x*scrSize.x - parentTrans.absPos.x) / parentTrans.absScale.x, (pos.y*scrSize.y - parentTrans.absPos.y) / parentTrans.absScale.y);
	}


	absPos.x += (0.5f - anchor.x) * width*scale.x;
	absPos.y += (0.5f - anchor.y) * height*scale.y;

	SetTransformRelPixel(node, absPos.x, absPos.y, scale);

	node->GetTransform().CalcAbsTransform(parent->GetTransform());
}

float MGameFactory::TranslateSpeed(float speed){
	// speed = 1 translates over the whole width per 10 seconds
	return speed * 0.001f*MEngine.environmentCtrl->GetWidth();
}


ofVec3f MGameFactory::GetPercentageVec(float xPerc, float yPerc){
	float width = COGGetWidth();
	float height = COGGetHeight();

	return ofVec3f(width*xPerc, height*yPerc);
}



GNode* MGameFactory::CreateRoot(){

	GNode* root = new GNode(ObjType::ROOT, 0, "root");
	sceneManager = new BeSceneManager();
	root->AddBehavior(sceneManager, true);

	return root;
}

void MGameFactory::SwitchToScene(int scene){
	// nothing to do here...
}