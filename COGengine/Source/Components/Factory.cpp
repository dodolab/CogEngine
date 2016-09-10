#include "Factory.h"
#include "Node.h"
#include "CogEngine.h"
#include "RotateAnim.h"
#include "HitEvent.h"
#include "Animator.h"

namespace Cog {

	ofVec2f Factory::GetCenter() {
		return ofVec2f(CogGetScreenWidth() / 2, CogGetScreenHeight() / 2);
	}

	Trans Factory::CalcTransform(Node* node, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, int width, int height, Node* parent) {
		ofVec2f scrSize = CogGetScreenSize();

		Trans& parentTrans = parent->GetTransform();
		Trans nodeTransform = Trans(0, 0);

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

	void Factory::SetTransform(Node* node, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, int width, int height, Node* parent) {
		node->SetTransform(CalcTransform(node, pos, posCalc, scaleX, scaleCalc, anchor, width, height, parent));
	}

	spt<ofImage> Factory::SetRenderImage(Node* node, string imgPath, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, Node* parent) {

		spt<ofImage> img = COGEngine.resourceCache->Get2DImage(imgPath);
		node->SetShape(spt<Image>(new Image(img)));

		SetTransform(node, pos, posCalc, scaleX, scaleCalc, anchor, img->getWidth(), img->getHeight(), parent);
		return img;
	}

	Node* Factory::CreateImageNode(string tag, string imgPath, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, Node* parent) {
		Node* node = new Node(tag);
		SetRenderImage(node, imgPath, pos, posCalc, scaleX, scaleCalc, anchor, parent);
		parent->AddChild(node);
		return node;
	}

	void Factory::SetFont(Node* node, spt<ofTrueTypeFont> font, ofColor color, string text) {
	/*	node->SetShape(spt<Font>(new Font(font,2)));
		node->AddAttr(ATTR_COLOR, color);
		node->AddAttr(ATTR_TEXT, text);*/
	}

	void Factory::SetRenderFont(Node* node, spt<ofTrueTypeFont> font, ofColor color, string text, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, Node* parent) {
	/*	node->SetShape(spt<Font>(new Font(font,2)));
		node->AddAttr(ATTR_COLOR, color);
		node->AddAttr(ATTR_TEXT, text);
		SetTransform(node, pos, posCalc, scaleX, scaleCalc, anchor, font->stringWidth(text), font->stringHeight(text), parent);*/

	}


	Node* Factory::CreateFontNode(string tag, spt<ofTrueTypeFont> font, ofColor color, string text, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, Node* parent) {
		Node* node = new Node(tag);
		SetRenderFont(node, font, color, text, pos, posCalc, scaleX, scaleCalc, anchor, parent);
		parent->AddChild(node);
		return node;
	}

	ofVec2f Factory::CalcPosition(Node* node, ofVec2f pos, CalcType posCalc, Node* parent) {
		ofVec2f scrSize = CogGetScreenSize();
		Trans& parentTrans = parent->GetTransform();
		ofVec2f absPos;

		switch (posCalc) {
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

	ofVec2f Factory::CalcPosition(Node* node, ofVec2f pos, CalcType posCalc, ofVec2f anchor, int width, int height, Node* parent) {
		ofVec2f scrSize = CogGetScreenSize();

		Trans& parentTrans = parent->GetTransform();
		Trans& nodeTransform = node->GetTransform();

		ofVec3f scale = node->GetTransform().scale;
		ofVec2f absPos = CalcPosition(node, pos, posCalc, parent);

		absPos.x += (0.5f - anchor.x) * width*scale.x;
		absPos.y += (0.5f - anchor.y) * height*scale.y;

		return ofVec3f(absPos.x, absPos.y, node->GetTransform().localPos.z);
	}

	ofVec3f Factory::CalcScale(Node* node, float scaleX, float width, CalcType scaleCalc, Node* parent) {
		ofVec2f scrSize = CogGetScreenSize();
		Trans& parentTrans = parent->GetTransform();

		float calcScale = 1;

		switch (scaleCalc) {
		case CalcType::ABS:
			calcScale = scaleX / parentTrans.absScale.x;
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


	float Factory::TranslateSpeed(float speed) {
		// speed = 1 translates over the whole width per 10 seconds
		return speed * 0.001f*COGEngine.environment->GetWidth();
	}

	Node* Factory::CreateRoot() {

		Node* root = new Node(ObjType::ROOT, 1, "root");
		sceneManager = new SceneManager();
		root->AddBehavior(sceneManager);
		root->SubmitChanges(true);
		return root;
	}



	bool Factory::LoadAnimations(spt<ofxXmlSettings> xml) {

		AnimationLoader loader;
		vector<spt<Anim>> rootAnims;

		try {
			// load root animations
			loader.LoadAnimations(xml, rootAnims);
		}
		catch (IllegalArgumentException& err) {
			CogLogError("Factory", err.what());
			CogLogError("Factory", "Animations couldn't be loaded");
			return false;
		}
		catch (ConfigErrorException& err) {
			CogLogError("Factory", err.what());
			CogLogError("Factory", "Animations couldn't be loaded");
			return false;
		}

		vector<Anim*> animArray;

		// for each root animation:
		for (auto it = rootAnims.begin(); it != rootAnims.end(); ++it) {
			animArray.clear();
			spt<Anim> rootAnim = (*it);

			// store into repository
			CogStoreAnimation(rootAnim);
			rootAnim->GetAllNodes(animArray);

			// for each inner node
			for (auto it2 = animArray.begin(); it2 != animArray.end(); ++it2) {

				Anim* anim = (*it2);

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

}// namespace