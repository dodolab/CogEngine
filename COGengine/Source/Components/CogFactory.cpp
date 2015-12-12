#include "CogFactory.h"
#include "CogNode.h"
#include "CogEngine.h"
#include "CogRotateAnim.h"
#include "CogHitEvent.h"
#include "CogAnimator.h"

namespace Cog {

	ofVec2f CogFactory::GetCenter() {
		return ofVec2f(CogGetScreenWidth() / 2, CogGetScreenHeight() / 2);
	}

	CogTrans CogFactory::CalcTransform(CogNode* node, ofVec2f pos, CogCalcType posCalc, float scaleX, CogCalcType scaleCalc, ofVec2f anchor, int width, int height, CogNode* parent) {
		ofVec2f scrSize = CogGetScreenSize();

		CogTrans& parentTrans = parent->GetTransform();
		CogTrans nodeTransform = CogTrans(0, 0);

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

	void CogFactory::SetTransform(CogNode* node, ofVec2f pos, CogCalcType posCalc, float scaleX, CogCalcType scaleCalc, ofVec2f anchor, int width, int height, CogNode* parent) {
		node->SetTransform(CalcTransform(node, pos, posCalc, scaleX, scaleCalc, anchor, width, height, parent));
	}

	spt<ofImage> CogFactory::SetRenderImage(CogNode* node, string imgPath, ofVec2f pos, CogCalcType posCalc, float scaleX, CogCalcType scaleCalc, ofVec2f anchor, CogNode* parent) {

		spt<ofImage> img = COGEngine.resourceCtrl->Get2DImage(imgPath);
		node->SetShape(spt<CogImage>(new CogImage(img)));

		SetTransform(node, pos, posCalc, scaleX, scaleCalc, anchor, img->getWidth(), img->getHeight(), parent);
		return img;
	}

	CogNode* CogFactory::CreateImageNode(string tag, string imgPath, ofVec2f pos, CogCalcType posCalc, float scaleX, CogCalcType scaleCalc, ofVec2f anchor, CogNode* parent) {
		CogNode* node = new CogNode(tag);
		SetRenderImage(node, imgPath, pos, posCalc, scaleX, scaleCalc, anchor, parent);
		parent->AddChild(node);
		return node;
	}

	void CogFactory::SetFont(CogNode* node, spt<ofTrueTypeFont> font, ofColor color, string text) {
		node->SetShape(spt<CogFont>(new CogFont(font)));
		node->AddAttr(Attrs::COLOR, color);
		node->AddAttr(Attrs::TEXT, text);
	}

	void CogFactory::SetRenderFont(CogNode* node, spt<ofTrueTypeFont> font, ofColor color, string text, ofVec2f pos, CogCalcType posCalc, float scaleX, CogCalcType scaleCalc, ofVec2f anchor, CogNode* parent) {
		node->SetShape(spt<CogFont>(new CogFont(font)));
		node->AddAttr(Attrs::COLOR, color);
		node->AddAttr(Attrs::TEXT, text);
		SetTransform(node, pos, posCalc, scaleX, scaleCalc, anchor, font->stringWidth(text), font->stringHeight(text), parent);

	}


	CogNode* CogFactory::CreateFontNode(string tag, spt<ofTrueTypeFont> font, ofColor color, string text, ofVec2f pos, CogCalcType posCalc, float scaleX, CogCalcType scaleCalc, ofVec2f anchor, CogNode* parent) {
		CogNode* node = new CogNode(tag);
		SetRenderFont(node, font, color, text, pos, posCalc, scaleX, scaleCalc, anchor, parent);
		parent->AddChild(node);
		return node;
	}

	ofVec2f CogFactory::CalcPosition(CogNode* node, ofVec2f pos, CogCalcType posCalc, CogNode* parent) {
		ofVec2f scrSize = CogGetScreenSize();
		CogTrans& parentTrans = parent->GetTransform();
		ofVec2f absPos;

		switch (posCalc) {
		case CogCalcType::ABS:
			absPos = ofVec2f((pos.x - parentTrans.absPos.x) / parentTrans.absScale.x, (pos.y - parentTrans.absPos.y) / parentTrans.absScale.y);
			break;
		case CogCalcType::LOC:
			absPos = pos;
			break;
		case CogCalcType::PER:
			absPos = ofVec2f((pos.x*scrSize.x - parentTrans.absPos.x) / parentTrans.absScale.x, (pos.y*scrSize.y - parentTrans.absPos.y) / parentTrans.absScale.y);
			break;
		}

		return absPos;
	}

	ofVec2f CogFactory::CalcPosition(CogNode* node, ofVec2f pos, CogCalcType posCalc, ofVec2f anchor, int width, int height, CogNode* parent) {
		ofVec2f scrSize = CogGetScreenSize();

		CogTrans& parentTrans = parent->GetTransform();
		CogTrans& nodeTransform = node->GetTransform();

		ofVec3f scale = node->GetTransform().scale;
		ofVec2f absPos = CalcPosition(node, pos, posCalc, parent);

		absPos.x += (0.5f - anchor.x) * width*scale.x;
		absPos.y += (0.5f - anchor.y) * height*scale.y;

		return ofVec3f(absPos.x, absPos.y, node->GetTransform().localPos.z);
	}

	ofVec3f CogFactory::CalcScale(CogNode* node, float scaleX, float width, CogCalcType scaleCalc, CogNode* parent) {
		ofVec2f scrSize = CogGetScreenSize();
		CogTrans& parentTrans = parent->GetTransform();

		float calcScale = 1;

		switch (scaleCalc) {
		case CogCalcType::ABS:
			calcScale = scaleX / parentTrans.absScale.x;
			break;
		case CogCalcType::LOC:
			calcScale = scaleX;
			break;
		case CogCalcType::PER:
			calcScale = (scaleX* scrSize.x / width) / parentTrans.absScale.x;
			break;
		}

		return ofVec3f(calcScale);
	}


	float CogFactory::TranslateSpeed(float speed) {
		// speed = 1 translates over the whole width per 10 seconds
		return speed * 0.001f*COGEngine.environmentCtrl->GetWidth();
	}

	CogNode* CogFactory::CreateRoot() {

		CogNode* root = new CogNode(CogObjType::ROOT, 1, "root");
		sceneManager = new CogSceneManager();
		root->AddBehavior(sceneManager);
		root->SubmitChanges(true);
		return root;
	}



	bool CogFactory::LoadAnimations(spt<ofxXmlSettings> xml) {

		CogAnimationLoader loader;
		vector<spt<CogAnim>> rootAnims;

		try {
			// load root animations
			loader.LoadAnimations(xml, rootAnims);
		}
		catch (IllegalArgumentException& err) {
			CogLogError("CogFactory", err.what());
			CogLogError("CogFactory", "Animations couldn't be loaded");
			return false;
		}
		catch (ConfigErrorException& err) {
			CogLogError("CogFactory", err.what());
			CogLogError("CogFactory", "Animations couldn't be loaded");
			return false;
		}

		vector<CogAnim*> animArray;

		// for each root animation:
		for (auto it = rootAnims.begin(); it != rootAnims.end(); ++it) {
			animArray.clear();
			spt<CogAnim> rootAnim = (*it);

			// store into repository
			CogStoreAnimation(rootAnim);
			rootAnim->GetAllNodes(animArray);

			// for each inner node
			for (auto it2 = animArray.begin(); it2 != animArray.end(); ++it2) {

				CogAnim* anim = (*it2);

				// initialize sheets and load images
				if (anim->HasSheets()) {
					anim->InitSheets();

					vector<string>& sheetPaths = anim->GetSheetPaths();

					for (string sheetPath : sheetPaths) {
						CogPreload2DImage(sheetPath);
					}
				}
			}
		}

		return true;
	}

}