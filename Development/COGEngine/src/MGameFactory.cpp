#include "MGameFactory.h"
#include "ofxSmartPointer.h"
#include "GNode.h"
#include "MGameEngine.h"
#include "BeRender.h"
#include "BeRotateAnim.h"
#include "BeHitEvent.h"
#include "BeAnim.h"

ofVec2f MGameFactory::GetCenter(){
	return ofVec2f(COGGetScreenWidth() / 2, COGGetScreenHeight() / 2);
}

void MGameFactory::SetTransform(GNode* node, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, int width, int height, GNode* parent){
	ofVec2f scrSize = COGGetScreenSize();

	EnTransform& parentTrans = parent->GetTransform();
	EnTransform& nodeTransform = node->GetTransform();

	ofVec2f scale = CalcScale(node, scaleX, width, scaleCalc, parent);
	ofVec2f absPos = CalcPosition(node, pos, posCalc, parent);

	absPos.x += (0.5f - anchor.x) * width*scale.x;
	absPos.y += (0.5f - anchor.y) * height*scale.y;

	nodeTransform.localPos = ofVec3f(absPos.x, absPos.y);
	nodeTransform.scale = ofVec3f(scale.x, scale.y, 1);

	// refresh transform
	nodeTransform.CalcAbsTransform(parent->GetTransform());
}

spt<ofImage> MGameFactory::SetRenderImage(GNode* node, string imgPath, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, GNode* parent){

	node->AddBehavior(new BeRender(RenderType::IMAGE), true);
	spt<ofImage> img = MEngine.resourceCtrl->Get2DImage(imgPath);
	node->AddAttr(Attrs::IMGSOURCE, img);

	SetTransform(node, pos, posCalc, scaleX, scaleCalc, anchor, img->getWidth(), img->getHeight(), parent);
	return img;
}

GNode* MGameFactory::CreateImageNode(string tag, string imgPath, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, GNode* parent){
	GNode* node = new GNode(tag);
	SetRenderImage(node, imgPath, pos, posCalc, scaleX, scaleCalc, anchor, parent);
	parent->AddChild(node, true);
	return node;
}

void MGameFactory::SetFont(GNode* node, spt<ofTrueTypeFont> font, ofColor color, string text){
	node->AddAttr(Attrs::FONT, font);
	node->AddBehavior(new BeRender(RenderType::TEXT), true);
	node->AddAttr(Attrs::COLOR, color);
	node->AddAttr(Attrs::TEXT, text);
}

void MGameFactory::SetRenderFont(GNode* node, spt<ofTrueTypeFont> font, ofColor color, string text, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, GNode* parent){
	node->AddAttr(Attrs::FONT, font);
	node->AddBehavior(new BeRender(RenderType::TEXT), true);
	node->AddAttr(Attrs::COLOR, color);
	node->AddAttr(Attrs::TEXT, text);
	SetTransform(node, pos, posCalc, scaleX, scaleCalc, anchor, font->stringWidth(text), font->stringHeight(text), parent);

}


GNode* MGameFactory::CreateFontNode(string tag, spt<ofTrueTypeFont> font, ofColor color, string text, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, GNode* parent){
	GNode* node = new GNode(tag);
	SetRenderFont(node, font, color, text, pos, posCalc, scaleX, scaleCalc, anchor, parent);
	parent->AddChild(node, true);
	return node;
}

ofVec2f MGameFactory::CalcPosition(GNode* node, ofVec2f pos, CalcType posCalc, GNode* parent){
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

ofVec2f MGameFactory::CalcPosition(GNode* node, ofVec2f pos, CalcType posCalc, ofVec2f anchor, int width, int height, GNode* parent){
	ofVec2f scrSize = COGGetScreenSize();

	EnTransform& parentTrans = parent->GetTransform();
	EnTransform& nodeTransform = node->GetTransform();

	ofVec3f scale = node->GetTransform().scale;
	ofVec2f absPos = CalcPosition(node, pos, posCalc, parent);

	absPos.x += (0.5f - anchor.x) * width*scale.x;
	absPos.y += (0.5f - anchor.y) * height*scale.y;

	return ofVec3f(absPos.x, absPos.y, node->GetTransform().localPos.z);
}

ofVec3f MGameFactory::CalcScale(GNode* node, float scaleX, float width, CalcType scaleCalc, GNode* parent){
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


float MGameFactory::TranslateSpeed(float speed){
	// speed = 1 translates over the whole width per 10 seconds
	return speed * 0.001f*MEngine.environmentCtrl->GetWidth();
}

GNode* MGameFactory::CreateRoot(){

	GNode* root = new GNode(ObjType::ROOT, 1, "root");
	sceneManager = new BeSceneManager();
	root->AddBehavior(sceneManager, true);

	return root;
}



void MGameFactory::LoadAnimations(spt<ofxXmlSettings> xml){

	AnimationLoader loader;

	// load root animations
	vector<spt<EnAnim>> rootAnims = loader.LoadAnimations(xml);
	
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
}
