#include "MFactory.h"
#include "ofxSmartPointer.h"
#include "GNode.h"
#include "MEngine.h"
#include "BeRotateAnim.h"
#include "BeHitEvent.h"
#include "BeAnim.h"
#include "MError.h"

ofVec2f MFactory::GetCenter(){
	return ofVec2f(COGGetScreenWidth() / 2, COGGetScreenHeight() / 2);
}

EnTransform MFactory::CalcTransform(GNode* node, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, int width, int height, GNode* parent){
	ofVec2f scrSize = COGGetScreenSize();

	EnTransform& parentTrans = parent->GetTransform();
	EnTransform nodeTransform = EnTransform(0, 0);

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

void MFactory::SetTransform(GNode* node, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, int width, int height, GNode* parent){
	node->SetTransform(CalcTransform(node, pos, posCalc, scaleX, scaleCalc, anchor, width, height, parent));
}

spt<ofImage> MFactory::SetRenderImage(GNode* node, string imgPath, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, GNode* parent){

	spt<ofImage> img = COGEngine.resourceCtrl->Get2DImage(imgPath);
	node->SetShape(spt<EnImageShape>(new EnImageShape(img)));

	SetTransform(node, pos, posCalc, scaleX, scaleCalc, anchor, img->getWidth(), img->getHeight(), parent);
	return img;
}

GNode* MFactory::CreateImageNode(string tag, string imgPath, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, GNode* parent){
	GNode* node = new GNode(tag);
	SetRenderImage(node, imgPath, pos, posCalc, scaleX, scaleCalc, anchor, parent);
	parent->AddChild(node);
	return node;
}

void MFactory::SetFont(GNode* node, spt<ofTrueTypeFont> font, ofColor color, string text){
	node->SetShape(spt<EnFontShape>(new EnFontShape(font)));
	node->AddAttr(Attrs::COLOR, color);
	node->AddAttr(Attrs::TEXT, text);
}

void MFactory::SetRenderFont(GNode* node, spt<ofTrueTypeFont> font, ofColor color, string text, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, GNode* parent){
	node->SetShape(spt<EnFontShape>(new EnFontShape(font)));
	node->AddAttr(Attrs::COLOR, color);
	node->AddAttr(Attrs::TEXT, text);
	SetTransform(node, pos, posCalc, scaleX, scaleCalc, anchor, font->stringWidth(text), font->stringHeight(text), parent);

}


GNode* MFactory::CreateFontNode(string tag, spt<ofTrueTypeFont> font, ofColor color, string text, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, GNode* parent){
	GNode* node = new GNode(tag);
	SetRenderFont(node, font, color, text, pos, posCalc, scaleX, scaleCalc, anchor, parent);
	parent->AddChild(node);
	return node;
}

ofVec2f MFactory::CalcPosition(GNode* node, ofVec2f pos, CalcType posCalc, GNode* parent){
	ofVec2f scrSize = COGGetScreenSize();
	EnTransform& parentTrans = parent->GetTransform();
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

ofVec2f MFactory::CalcPosition(GNode* node, ofVec2f pos, CalcType posCalc, ofVec2f anchor, int width, int height, GNode* parent){
	ofVec2f scrSize = COGGetScreenSize();

	EnTransform& parentTrans = parent->GetTransform();
	EnTransform& nodeTransform = node->GetTransform();

	ofVec3f scale = node->GetTransform().scale;
	ofVec2f absPos = CalcPosition(node, pos, posCalc, parent);

	absPos.x += (0.5f - anchor.x) * width*scale.x;
	absPos.y += (0.5f - anchor.y) * height*scale.y;

	return ofVec3f(absPos.x, absPos.y, node->GetTransform().localPos.z);
}

ofVec3f MFactory::CalcScale(GNode* node, float scaleX, float width, CalcType scaleCalc, GNode* parent){
	ofVec2f scrSize = COGGetScreenSize();
	EnTransform& parentTrans = parent->GetTransform();

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


float MFactory::TranslateSpeed(float speed){
	// speed = 1 translates over the whole width per 10 seconds
	return speed * 0.001f*COGEngine.environmentCtrl->GetWidth();
}

GNode* MFactory::CreateRoot(){

	GNode* root = new GNode(ObjType::ROOT, 1, "root");
	sceneManager = new BeSceneManager();
	root->AddBehavior(sceneManager);
	root->SubmitChanges(true);
	return root;
}



bool MFactory::LoadAnimations(spt<ofxXmlSettings> xml){

	AnimationLoader loader;
	vector<spt<EnAnim>> rootAnims;

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

	vector<EnAnim*> animArray;

	// for each root animation:
	for (auto it = rootAnims.begin(); it != rootAnims.end(); ++it){
		animArray.clear();
		spt<EnAnim> rootAnim = (*it);

		// store into repository
		COGStoreAnimation(rootAnim);
		rootAnim->GetAllNodes(animArray);

		// for each inner node
		for (auto it2 = animArray.begin(); it2 != animArray.end(); ++it2){
			
			EnAnim* anim = (*it2);

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
