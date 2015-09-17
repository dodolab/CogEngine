#include "MGameFactory.h"
#include "ofxSmartPointer.h"
#include "GNode.h"
#include "MGameEngine.h"
#include "BeRender.h"
#include "BeRotateAnim.h"
#include "BeHitEvent.h"

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

	if (xml->tagExists("animations")){
		xml->pushTag("animations");

		int numberOfAnims = xml->getNumTags("anim");
		// referenced anims will be processed at second phase
		map<string, EnAnim*> referencedAnims;
		vector<string> rootAnimNames;


		// phase 1: load not-referenced animations
		for (int i = 0; i < numberOfAnims; i++){

			xml->pushTag("anim", i);
			// load
			spt<EnAnim> anim = spt<EnAnim>(CreateNonRefAnimationFromXml(xml, referencedAnims));
			// store
			COGStoreAnimation(anim);
			rootAnimNames.push_back(anim->GetName());

			// TODO:: kdyz sem dam spt<EnAnim>, tak se to zacykli kvuli destruktoru !!!
			// obtain array
			vector<EnAnim*> animArray;
			anim->GetAllNodes(animArray);

			// preload all images (TODO: referenced anims could have their own sheets ??? )
			for (auto it = animArray.begin(); it != animArray.end(); ++it){

				EnAnim* anim = (*it);
				if (anim->HasSheets()){
					vector<string> sheets = anim->GetSheetPaths();

					for (auto it2 = sheets.begin(); it2 != sheets.end(); ++it2){
						// path is defined; load image
						COGPreload2DImage((*it2));
					}
				}
			}
			
			animArray.clear();
			xml->popTag();
		}

		// phase 2: load referenced animations
		for (int i = 0; i < numberOfAnims; i++){

			xml->pushTag("anim", i);
			spt<EnAnim> rootAnim = COGGetAnimation(rootAnimNames[i]);
			ProcessRefAnimationFromXml(xml, referencedAnims, rootAnim);

			xml->popTag();
		}

		xml->popTag();
	}
}

EnAnim* MGameFactory::CreateNonRefAnimationFromXml(spt<ofxXmlSettings> xml, map<string, EnAnim*>& referencedAnims){
	int innerAnimations = xml->getNumTags("anim");

	EnAnim* anim = new EnAnim();

	// always set ref and name
	anim->SetRef(xml->getAttribute(":", "ref", anim->GetRef()));
	anim->SetName(xml->getAttribute(":", "name", anim->GetName()));

	if (anim->GetRef().length() != 0){
		// animation is referenced -> push it to the referencedAnims and return
		referencedAnims[anim->GetName()] = anim;
		return anim;
	}
	else{
		// children will be taken from referenced animation
		for (int i = 0; i < innerAnimations; i++){
			xml->pushTag("anim", i);
			spt<EnAnim> newAnim = spt<EnAnim>(CreateNonRefAnimationFromXml(xml, referencedAnims));
			anim->GetChildren().push_back(newAnim);
			xml->popTag();
		}
	}

	// set other attributes
	anim->SetSheetPath(xml->getAttribute(":", "sheet", anim->GetSheetPath()));
	anim->SetFrames(xml->getAttribute(":", "frames", anim->GetFrames()));
	anim->SetLines(xml->getAttribute(":", "lines", anim->GetLines()));
	anim->SetStart(xml->getAttribute(":", "start", anim->GetStart()));
	anim->SetEnd(xml->getAttribute(":", "end", anim->GetEnd()));
	anim->SetIncrement(xml->getAttribute(":", "increment", anim->GetIncrement()));
	anim->SetSpeed(xml->getAttribute(":", "speed", anim->GetSpeed()));
	anim->SetRepeat(xml->getAttribute(":", "repeat", anim->GetRepeat()));
	anim->SetIsRevert(xml->getBoolAttribute(":", "revert", anim->GetIsRevert()));


	return anim;
}

void MGameFactory::ProcessRefAnimationFromXml(spt<ofxXmlSettings> xml, map<string, EnAnim*>& referencedAnims, spt<EnAnim> rootAnim){

	int innerAnimations = xml->getNumTags("anim");

	string ref = (xml->getAttribute(":", "ref", ""));
	string name = (xml->getAttribute(":", "name", ""));

	if (ref.length() != 0){
		EnAnim* refAnim = referencedAnims[name];

		if (ref.find(".") == -1){
			// reference doesn't contain dot - it means that we can find its reference in THIS scope
			spt<EnAnim> reference = rootAnim->FindChild(ref);

			if (reference != spt<EnAnim>()){
				// found reference
				refAnim->GetParametersFromReference(reference);
			}
			else{
				// reference anim doesn't contain dot, but it isn't in this scope... that means it must refer to some other root animation
				spt<EnAnim> rootReference = COGGetAnimation(ref);
				refAnim->GetParametersFromReference(rootReference);
			}
		}
		else{
			// reference contains dot -> referenced animation should be in another scope
			string rootAnim = ref.substr(0, ref.find("."));
			string subAnim = ref.substr(ref.find(".") + 1);
			spt<EnAnim> root = COGGetAnimation(rootAnim);
			spt<EnAnim> scopeAnim = root->FindChild(subAnim);
			refAnim->GetParametersFromReference(scopeAnim);
		}

		// fill remaining attributes from XML
		refAnim->SetSheetPath(xml->getAttribute(":", "sheet", refAnim->GetSheetPath()));
		refAnim->SetFrames(xml->getAttribute(":", "frames", refAnim->GetFrames()));
		refAnim->SetLines(xml->getAttribute(":", "lines", refAnim->GetLines()));
		refAnim->SetStart(xml->getAttribute(":", "start", refAnim->GetStart()));
		refAnim->SetEnd(xml->getAttribute(":", "end", refAnim->GetEnd()));
		refAnim->SetIncrement(xml->getAttribute(":", "increment", refAnim->GetIncrement()));
		refAnim->SetSpeed(xml->getAttribute(":", "speed", refAnim->GetSpeed()));
		refAnim->SetRepeat(xml->getAttribute(":", "repeat", refAnim->GetRepeat()));
		refAnim->SetIsRevert(xml->getBoolAttribute(":", "revert", refAnim->GetIsRevert()));

	}
	else{
		for (int i = 0; i < innerAnimations; i++){
			// use recursion
			xml->pushTag("anim", i);
			ProcessRefAnimationFromXml(xml, referencedAnims, rootAnim);
			xml->popTag();
		}
	}
}