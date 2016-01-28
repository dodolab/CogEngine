#include "NodeBuilder.h"
#include "CogEngine.h"

namespace Cog {

	void NodeBuilder::SetImageNode(Node* node, string path) {
		spt<ofImage> image = CogPreload2DImage(path);
		node->SetShape(spt<Image>(new Image(image)));
	}

	void NodeBuilder::SetButtonNode(Node* node, string defaultImg, string clickedImg, string disabledImg) {

		spt<ofImage> disabledImgPtr = disabledImg.empty() ? spt<ofImage>() : CogPreload2DImage(disabledImg);

		node->AddBehavior(new HitEvent(-1, false, false));
		node->AddBehavior(new Button(CogPreload2DImage(defaultImg), CogPreload2DImage(clickedImg), disabledImgPtr));
		node->GetStates().SetState(StringHash(STATES_HITTABLE));
	}

	void NodeBuilder::SetSelectionNode(Node* node, string defaultImg, string selectImg, string selectionGroup) {
		node->AddBehavior(new HitEvent(-1, false, false));
		node->AddBehavior(new Selection(CogPreload2DImage(defaultImg), CogPreload2DImage(selectImg), StringHash(selectionGroup)));
		node->GetGroups().SetState(StringHash(selectionGroup));
	}

	void NodeBuilder::SetTextNode(Node* node, string font, float size, ofColor color, string text) {
		spt<ofTrueTypeFont> fontVal = CogGetFont(font, (int)size);

		auto textShape = new Text(fontVal, text, size);
		textShape->SetColor(color);
		node->SetShape(spt<Text>(textShape));
	}

	Node* NodeBuilder::LoadNodeFromXml(spt<ofxXml> xml, Node* parent, Settings& settings) {
		TransformMath math = TransformMath();


		string name = xml->getAttributex("name", "");
		string img = xml->getAttributex("img", "");

		// get reference width and height
		int refWidth = settings.GetSettingValInt("transform", "ref_width");
		int refHeight = settings.GetSettingValInt("transform", "ref_height");


		Node* node = new Node(ObjType::OBJECT, 0, name);
		// set default shape
		node->SetShape(spt<Rectangle>(new Rectangle((float)refWidth, (float)refHeight)));

		if (!img.empty()) {
			SetImageNode(node, img);
		}

		if (xml->attributeExists("img_click")) {
			// set image on click
			string imgClick = xml->getAttributex("img_click", "");
			string imgDisabled = xml->getAttributex("img_disabled", "");

			SetButtonNode(node, img, imgClick, imgDisabled);
		}

		if (xml->attributeExists("img_select")) {
			// set image on selection
			string imgSelect = xml->getAttributex("img_select", "");
			string selectGroup = xml->getAttributex("select_group", "");
			SetSelectionNode(node, img, imgSelect, selectGroup);
		}

		// scene node will always fit to screen size
		if (parent->GetType() == ObjType::SCENE) {
			math.SetSizeToScreen(node, parent);
		}


		// text must be loaded before transform
		if (xml->pushTagIfExists("text")) {
			LoadTextFromXml(xml, node, parent);
			xml->popTag();
		}

		if (xml->pushTagIfExists("transform")) {

			// load transform
			math.LoadTransformFromXml(xml, node, parent, settings);
			xml->popTag();
		}

		if (xml->tagExists("behavior")) {
			int behaviors = xml->getNumTags("behavior");

			for (int i = 0; i < behaviors; i++) {
				xml->pushTag("behavior", i);
				LoadBehaviorFromXml(xml, node);
				xml->popTag();
			}
		}


		if (xml->tagExists("state")) {
			int states = xml->getNumTags("state");

			for (int i = 0; i < states; i++) {
				string stateName = xml->getValue("state", "", i);
				node->GetStates().SetState(StringHash(stateName));
			}
		}

		if (xml->tagExists("node")) {
			int children = xml->getNumTags("node");

			// load children
			for (int i = 0; i < children; i++) {
				xml->pushTag("node", i);

				Node* child = LoadNodeFromXml(xml, node, settings);
				node->AddChild(child);

				xml->popTag();
			}
		}
		return node;
	}

	void NodeBuilder::LoadTextFromXml(spt<ofxXml> xml, Node* node, Node* parent) {
		string font = xml->getAttributex("font", "");
		float size = xml->getAttributex("size", 1.0);
		string value = xml->getValuex("");
		string colorStr = xml->getAttributex("color", "0x000000");
		ofColor color = ofColor::fromHex(ofHexToInt(colorStr));

		SetTextNode(node, font, size, color, value);
	}

	void NodeBuilder::LoadBehaviorFromXml(spt<ofxXml> xml, Node* node) {
		string name = xml->getAttributex("name", "");
		auto newBeh = COGEngine.entityStorage->GetBehaviorPrototype(name)->CreatePrototype();
		newBeh->LoadFromXml(xml);
		node->AddBehavior(newBeh);
	}

} // namespace