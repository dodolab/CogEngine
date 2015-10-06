#include "ofxArcFactory.h"
#include "ofxAreNode.h"
#include "ofxAreEngine.h"
#include "ofxArbRotateAnim.h"
#include "ofxArbHitEvent.h"
#include "ofxArbAnim.h"
#include "ofxAreError.h"

ofVec2f ofxArcFactory::GetCenter(){
	return ofVec2f(COGGetScreenWidth() / 2, COGGetScreenHeight() / 2);
}

ofxAraTransform ofxArcFactory::CalcTransform(ofxAreNode* node, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, int width, int height, ofxAreNode* parent){
	ofVec2f scrSize = COGGetScreenSize();

	ofxAraTransform& parentTrans = parent->GetTransform();
	ofxAraTransform nodeTransform = ofxAraTransform(0, 0);

	ofVec2f scale = CalcScale(node, scaleX, width, scaleCalc, parent);
	ofVec2f absPos = CalcPosition(node, pos, posCalc, parent);

	absPos.x += (0.5f - anchor.x) * width*scale.x;
	absPos.y += (0.5f - anchor.y) * height*scale.y;

	nodeTransform.localPos = ofVec3f(absPos.x, absPos.y, node->GetTransform().localPos.z);
	nodeTransform.scale = ofVec3f(scale.x, scale.y, 1);

	// refresh transform
	nodeTransform.CalcAbsTransform(parent->GetTransform());

	return nodeTransform;
}

void ofxArcFactory::SetTransform(ofxAreNode* node, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, int width, int height, ofxAreNode* parent){
	node->SetTransform(CalcTransform(node, pos, posCalc, scaleX, scaleCalc, anchor, width, height, parent));
}

spt<ofImage> ofxArcFactory::SetRenderImage(ofxAreNode* node, string imgPath, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, ofxAreNode* parent){

	spt<ofImage> img = COGEngine.resourceCtrl->Get2DImage(imgPath);
	node->SetShape(spt<EnImageShape>(new EnImageShape(img)));

	SetTransform(node, pos, posCalc, scaleX, scaleCalc, anchor, img->getWidth(), img->getHeight(), parent);
	return img;
}

ofxAreNode* ofxArcFactory::CreateImageNode(string tag, string imgPath, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, ofxAreNode* parent){
	ofxAreNode* node = new ofxAreNode(tag);
	SetRenderImage(node, imgPath, pos, posCalc, scaleX, scaleCalc, anchor, parent);
	parent->AddChild(node);
	return node;
}

void ofxArcFactory::SetFont(ofxAreNode* node, spt<ofTrueTypeFont> font, ofColor color, string text){
	node->SetShape(spt<EnFontShape>(new EnFontShape(font)));
	node->AddAttr(Attrs::COLOR, color);
	node->AddAttr(Attrs::TEXT, text);
}

void ofxArcFactory::SetRenderFont(ofxAreNode* node, spt<ofTrueTypeFont> font, ofColor color, string text, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, ofxAreNode* parent){
	node->SetShape(spt<EnFontShape>(new EnFontShape(font)));
	node->AddAttr(Attrs::COLOR, color);
	node->AddAttr(Attrs::TEXT, text);
	SetTransform(node, pos, posCalc, scaleX, scaleCalc, anchor, font->stringWidth(text), font->stringHeight(text), parent);

}


ofxAreNode* ofxArcFactory::CreateFontNode(string tag, spt<ofTrueTypeFont> font, ofColor color, string text, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, ofxAreNode* parent){
	ofxAreNode* node = new ofxAreNode(tag);
	SetRenderFont(node, font, color, text, pos, posCalc, scaleX, scaleCalc, anchor, parent);
	parent->AddChild(node);
	return node;
}

ofVec2f ofxArcFactory::CalcPosition(ofxAreNode* node, ofVec2f pos, CalcType posCalc, ofxAreNode* parent){
	ofVec2f scrSize = COGGetScreenSize();
	ofxAraTransform& parentTrans = parent->GetTransform();
	ofVec2f absPos;

	switch (posCalc){
	case CalcType::ABS:
		absPos = ofVec2f((pos.x - parentTrans.absPos.x) / parentTrans.absScale.x, (pos.y - parentTrans.absPos.y) / parentTrans.absScale.y);
		break;
	case CalcType::LOC:
		absPos = pos;
		break;
	case CalcType::PER:
		absPos = ofVec2f((pos.x*scrSize.x - parentTrans.absPos.x) / parentTrans.absScale.x, (pos.y*scrSize.y - parentTrans.absPos.y) / parentTrans.absScale.y);
		break;
	}

	return absPos;
}

ofVec2f ofxArcFactory::CalcPosition(ofxAreNode* node, ofVec2f pos, CalcType posCalc, ofVec2f anchor, int width, int height, ofxAreNode* parent){
	ofVec2f scrSize = COGGetScreenSize();

	ofxAraTransform& parentTrans = parent->GetTransform();
	ofxAraTransform& nodeTransform = node->GetTransform();

	ofVec3f scale = node->GetTransform().scale;
	ofVec2f absPos = CalcPosition(node, pos, posCalc, parent);

	absPos.x += (0.5f - anchor.x) * width*scale.x;
	absPos.y += (0.5f - anchor.y) * height*scale.y;

	return ofVec3f(absPos.x, absPos.y, node->GetTransform().localPos.z);
}

ofVec3f ofxArcFactory::CalcScale(ofxAreNode* node, float scaleX, float width, CalcType scaleCalc, ofxAreNode* parent){
	ofVec2f scrSize = COGGetScreenSize();
	ofxAraTransform& parentTrans = parent->GetTransform();

	float calcScale = 1;

	switch (scaleCalc){
	case CalcType::ABS:
		calcScale = scaleX/parentTrans.absScale.x;
		break;
	case CalcType::LOC:
		calcScale = scaleX;
		break;
	case CalcType::PER:
		calcScale = (scaleX* scrSize.x / width) / parentTrans.absScale.x;
		break;
	}

	return ofVec3f(calcScale);
}


float ofxArcFactory::TranslateSpeed(float speed){
	// speed = 1 translates over the whole width per 10 seconds
	return speed * 0.001f*COGEngine.environmentCtrl->GetWidth();
}

ofxAreNode* ofxArcFactory::CreateRoot(){

	ofxAreNode* root = new ofxAreNode(ObjType::ROOT, 1, "root");
	sceneManager = new ofxArbSceneManager();
	root->AddBehavior(sceneManager);
	root->SubmitChanges(true);
	return root;
}



bool ofxArcFactory::LoadAnimations(spt<ofxXmlSettings> xml){

	AnimationLoader loader;
	vector<spt<ofxAraAnim>> rootAnims;

	try{
		// load root animations
		loader.LoadAnimations(xml, rootAnims);
	}
	catch (IllegalArgumentException& err){
		COGLogError("Factory",err.what());
		COGLogError("Factory", "Animations couldn't be loaded");
		return false;
	}
	catch (ConfigErrorException& err){
		COGLogError("Factory",err.what());
		COGLogError("Factory","Animations couldn't be loaded");
		return false;
	}

	vector<ofxAraAnim*> animArray;

	// for each root animation:
	for (auto it = rootAnims.begin(); it != rootAnims.end(); ++it){
		animArray.clear();
		spt<ofxAraAnim> rootAnim = (*it);

		// store into repository
		COGStoreAnimation(rootAnim);
		rootAnim->GetAllNodes(animArray);

		// for each inner node
		for (auto it2 = animArray.begin(); it2 != animArray.end(); ++it2){
			
			ofxAraAnim* anim = (*it2);

			// initialize sheets and load images
			if (anim->HasSheets()){
				anim->InitSheets();

				vector<string>& sheetPaths = anim->GetSheetPaths();

				for (string sheetPath : sheetPaths){
					COGPreload2DImage(sheetPath);
				}
			}
		}
	}

	return true;
}
