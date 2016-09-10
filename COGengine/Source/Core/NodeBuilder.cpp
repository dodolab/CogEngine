#include "NodeBuilder.h"
#include "CogEngine.h"
#include "BehaviorEnt.h"

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

	Node* NodeBuilder::LoadNodeFromXml(spt<ofxXml> xml, Node* parent, Scene* scene, Settings& settings) {
		TransformMath math = TransformMath();


		string name = xml->getAttributex("name", "");
		string img = xml->getAttributex("img", "");

		// get reference width and height
		int refWidth = settings.GetSettingValInt("transform", "ref_width");
		int refHeight = settings.GetSettingValInt("transform", "ref_height");

		if (refWidth == 0) refWidth = CogGetScreenWidth();
		if (refHeight == 0) refHeight = CogGetScreenHeight();

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

			TransformEnt transformEnt = TransformEnt();
			transformEnt.LoadFromXml(xml, settings.GetSetting("transform"));

			// =================== get grid size (if specified)
			int gridWidth = settings.GetSettingValInt("transform", "grid_width");
			int gridHeight = settings.GetSettingValInt("transform", "grid_height");
			
			TransformMath math = TransformMath();

			// set transform according to the parsed values
			math.SetTransform(node, parent, transformEnt, gridWidth, gridHeight);
			xml->popTag();
		}

		if (xml->pushTagIfExists("shape")) {
			// load shape
			LoadShapeFromXml(xml, node, scene);
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

				Node* child = LoadNodeFromXml(xml, node, scene, settings);
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
		
		string type = xml->getAttributex("type", "");
		string name = xml->getAttributex("name", "");
		auto resourceCache = GETCOMPONENT(ResourceCache);

		Behavior* behavior = nullptr;

		if (!type.empty()) {
			// load directly
			Behavior* prototype = COGEngine.entityStorage->GetBehaviorPrototype(type);

			if (xml->pushTagIfExists("setting")) {
				auto setting = resourceCache->LoadSettingFromXml(xml);
				behavior = prototype->CreatePrototype(setting);
				xml->popTag();
			}
			else {
				behavior = prototype->CreatePrototype();
			}
		}
		else {
			// load from reference
			spt<BehaviorEnt> ent = resourceCache->GetEntityC<BehaviorEnt>(name);
			Behavior* prototype = COGEngine.entityStorage->GetBehaviorPrototype(ent->type);

			if (!ent->setting.Empty()) behavior = prototype->CreatePrototype(ent->setting);
			else behavior = prototype->CreatePrototype();
		}

		if (behavior == nullptr) throw new ConfigErrorException(string_format("Error while parsing %s behavior; no prototype found", type.c_str()));

		node->AddBehavior(behavior);
	}

	void NodeBuilder::LoadShapeFromXml(spt<ofxXml> xml, Node* node, Scene* scene) {
		string type = xml->getAttributex("type", "");


		if (type.compare("image") == 0) {
			string img = xml->getAttributex("img", "");
			this->SetImageNode(node, img);
		}
		else if (type.compare("rectangle") == 0) {
			// todo
		}
		else if (type.compare("polygon") == 0) {
			// todo
		}
		else if (type.compare("text") == 0) {
			// todo
		}
		else if (type.compare("sprite") == 0) {
			string spriteSheet = xml->getAttributex("spritesheet", "");
			string layer = xml->getAttributex("layer", "");

			if(spriteSheet.empty() && layer.empty()) throw IllegalArgumentException("Error while loading sprite sheet. Neither spriteSheet nor layer specified");

			string spriteSet = xml->getAttributex("spriteset", "");

			auto cache = GETCOMPONENT(ResourceCache);

			// user can specify spritesheet directly or by layer name that points to the spritesheet
			if (!layer.empty()) {
				LayerEnt layerEntity = scene->FindLayerSettings(layer);
				spriteSheet = layerEntity.spriteSheetName;
			}
			else {
				layer = spriteSheet;
			}

			auto sheet = cache->GetSpriteSheet(spriteSheet);

			if (sheet == nullptr) throw IllegalArgumentException("Error while loading sprite sheet. No such spritesheed found!");


			spt<SpriteSet> spriteSetEntity;

			if (spriteSet.empty()) {
				// load default
				spriteSetEntity = sheet->GetDefaultSpriteSet();
			}
			else {
				// load by name
				spriteSetEntity = sheet->GetSpriteSetByName(spriteSet);
			}

			int row = xml->getAttributex("row", 0);
			int column = xml->getAttributex("column", 0);

			spt<Sprite> sprite = spt<Sprite>(new Sprite(spriteSetEntity, row, column));

			auto shape = spt<SpriteShape>(new SpriteShape(sprite, layer));
			node->SetShape(shape);
			
		}
		else if (type.compare("multisprite") == 0) {
			// todo
		}
	}

} // namespace