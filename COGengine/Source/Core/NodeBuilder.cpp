#include "NodeBuilder.h"
#include "CogEngine.h"
#include "BehaviorEnt.h"
#include "Selection.h"
#include "MultiSelection.h"

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


	void NodeBuilder::SetMultiSelectionNode(Node* node, string defaultImg, string selectImg, string selectionGroup) {
		node->AddBehavior(new HitEvent(-1, false, false));
		node->AddBehavior(new MultiSelection(CogPreload2DImage(defaultImg), CogPreload2DImage(selectImg), StringHash(selectionGroup)));
		node->GetGroups().SetState(StringHash(selectionGroup));
	}

	void NodeBuilder::SetSelectionNode(Node* node, string defaultImg, string selectImg) {
		node->AddBehavior(new Selection(CogPreload2DImage(defaultImg), CogPreload2DImage(selectImg)));
	}

	void NodeBuilder::SetTextNode(Node* node, string font, float size, ofColor color, string text) {
		spt<ofTrueTypeFont> fontVal = CogGetFont(font, (int)size);

		auto textShape = new Text(fontVal, text);
		textShape->SetColor(color);
		node->SetShape(spt<Text>(textShape));
	}

	void NodeBuilder::SetSpriteNode(Scene* scene, Node* node, string layer, string spriteSet, int row, int column) {
		auto spriteShape = CreateSpriteShape(scene, layer, spriteSet, row, column);
		node->SetShape(spriteShape);
	}

	void NodeBuilder::SetSpriteNode(Scene* scene, Node* node, string layer, int row, int column) {
		auto spriteShape = CreateSpriteShape(scene, layer, row, column);
		node->SetShape(spriteShape);
	}

	void NodeBuilder::SetPlaneNode(Node* node, ofVec2f size, ofColor color) {
		auto planeShape = CreatePlaneShape(size, color);
		node->SetShape(planeShape);
	}

	void NodeBuilder::SetBoundingBoxNode(Scene* scene, Node* node, ofColor color, float margin, bool renderable) {
		Settings& settings = scene->GetSettings();

		// get reference width and height
		int refWidth = settings.GetSettingValInt("transform", "ref_width");
		int refHeight = settings.GetSettingValInt("transform", "ref_height");

		if (refWidth == 0) refWidth = CogGetScreenWidth();
		if (refHeight == 0) refHeight = CogGetScreenHeight();

		auto bbox = spt<BoundingBox>(new BoundingBox((float)refWidth, (float)refHeight,margin,renderable));
		bbox->SetColor(color);
		node->SetShape(bbox);
	}

	spt<SpriteShape> NodeBuilder::CreateSpriteShape(Scene* scene, string layer, string spriteSet, int row, int column) {

		LayerEnt layerEntity = scene->FindLayerSettings(layer);
		string spriteSheetName = layerEntity.spriteSheetName;

		auto cache = GETCOMPONENT(ResourceCache);
		auto spriteSheet = cache->GetSpriteSheet(spriteSheetName);

		if (!spriteSheet) throw IllegalArgumentException("Error while loading sprite sheet. No such spritesheed found!");

		spt<SpriteSet> spriteSetEntity;

		if (spriteSet.empty()) {
			// load default
			spriteSetEntity = spriteSheet->GetDefaultSpriteSet();
		}
		else {
			// load by name
			spriteSetEntity = spriteSheet->GetSpriteSetByName(spriteSet);
		}

		if (!spriteSetEntity) throw IllegalArgumentException(string_format("Spriteset %s not found!", spriteSet.c_str()));

		spt<Sprite> sprite = spt<Sprite>(new Sprite(spriteSetEntity, row, column));
		auto shape = spt<SpriteShape>(new SpriteShape(sprite, layer));
		return shape;
	}

	spt<SpriteShape> NodeBuilder::CreateSpriteShape(Scene* scene, string layer, int row, int column) {
		return CreateSpriteShape(scene, layer, "", row, column);
	}

	spt<Plane> NodeBuilder::CreatePlaneShape(ofVec2f size, ofColor color) {
		spt<Plane> plane = spt<Plane>(new Plane(size.x, size.y));
		plane->SetColor(color);
		return plane;
	}

	Behavior* NodeBuilder::CreateBehavior(spt<BehaviorEnt> entity) {
		Behavior* behavior = nullptr;

		auto resourceCache = GETCOMPONENT(ResourceCache);

		if (!entity->type.empty()) {
			// load directly
			Behavior* prototype = CogGetEntityStorage()->GetBehaviorPrototype(entity->type);

			if (!entity->setting.Empty()) {
				behavior = prototype->CreatePrototype(entity->setting);
			}
			else {
				behavior = prototype->CreatePrototype();
			}
		}
		else {
			// load from reference
			spt<BehaviorEnt> refent = resourceCache->GetEntityC<BehaviorEnt>(entity->ref);
			Behavior* prototype = CogGetEntityStorage()->GetBehaviorPrototype(refent->type);

			if (!refent->setting.Empty()) behavior = prototype->CreatePrototype(refent->setting);
			else behavior = prototype->CreatePrototype();
		}

		if (behavior == nullptr) throw IllegalArgumentException(string_format("Error while parsing %s behavior; no prototype found", entity->type.c_str()));

		return behavior;
	}

	Node* NodeBuilder::CreateNode(string name, Scene* scene) {
		Settings& settings = scene->GetSettings();

		// get reference width and height
		int refWidth = settings.GetSettingValInt("transform", "ref_width");
		int refHeight = settings.GetSettingValInt("transform", "ref_height");

		if (refWidth == 0) refWidth = CogGetScreenWidth();
		if (refHeight == 0) refHeight = CogGetScreenHeight();

		Node* node = new Node(NodeType::OBJECT, 0, name);
		// set default shape
		node->SetShape(spt<Rectangle>(new Rectangle((float)refWidth, (float)refHeight)));


		return node;
	}

	Node* NodeBuilder::LoadNodeFromXml(spt<ofxXml> xml, Node* parent, Scene* scene) {

		TransformMath math = TransformMath();
		Settings& settings = scene->GetSettings();

		string name = xml->getAttributex("name", "");
		string img = xml->getAttributex("img", "");

		Node* node = CreateNode(name, scene);

		if (!img.empty()) {
			SetImageNode(node, img);
		}

		if (xml->attributeExists("img_click")) {
			// set image on click
			string imgClick = xml->getAttributex("img_click", "");
			string imgDisabled = xml->getAttributex("img_disabled", "");

			SetButtonNode(node, img, imgClick, imgDisabled);
		}

		if (xml->attributeExists("img_multiselect")) {
			// set image on selection
			string imgSelect = xml->getAttributex("img_multiselect", "");
			string selectGroup = xml->getAttributex("select_group", "");
			SetMultiSelectionNode(node, img, imgSelect, selectGroup);
		}

		if (xml->attributeExists("img_select")) {
			// set image on selection
			string imgSelect = xml->getAttributex("img_select", "");
			SetSelectionNode(node, img, imgSelect);
		}

		// scene node will always fit to screen size
		if (parent->GetType() == NodeType::SCENE) {
			math.SetSizeToScreen(node, parent);
		}

		if (xml->pushTagIfExists("shape")) {
			// load shape
			LoadShapeFromXml(xml, node, scene);
			xml->popTag();
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

				Node* child = LoadNodeFromXml(xml, node, scene);
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
		ofColor color = StringToColor(colorStr);

		SetTextNode(node, font, size, color, value);
	}

	void NodeBuilder::LoadBehaviorFromXml(spt<ofxXml> xml, Node* node) {
		
		
		auto resourceCache = GETCOMPONENT(ResourceCache);
		spt<BehaviorEnt> ent = spt<BehaviorEnt>(new BehaviorEnt());
		auto dummySet = Setting();
		ent->LoadFromXml(xml, dummySet);

		Behavior* behavior = CreateBehavior(ent);

		node->AddBehavior(behavior);
	}

	void NodeBuilder::LoadShapeFromXml(spt<ofxXml> xml, Node* node, Scene* scene) {
		string type = xml->getAttributex("type", "");
		RenderType renderType = RenderTypeConverter::StrToRenderType(type);

		if (renderType == RenderType::IMAGE) {
			string img = xml->getAttributex("img", "");
			this->SetImageNode(node, img);
		}
		else if (renderType == RenderType::PLANE) {
			float width = 0;
			float height = 0;

			if (xml->attributeExists("size")) {
				width = height = xml->getAttributex("size", 1.0);
			}
			else {
				width = xml->getAttributex("width", 0);
				height = xml->getAttributex("height", 0);
			}

			ofVec2f size = ofVec2f(width, height);
			string colorStr = xml->getAttributex("color", "0x000000");
			ofColor color = StringToColor(colorStr);

			SetPlaneNode(node, size, color);
		}
		else if (renderType == RenderType::SPRITE) {
			string layer = xml->getAttributex("layer", "");

			if (layer.empty()) throw IllegalArgumentException("Error while loading sprite sheet. Layer not specified");

			string spriteSet = xml->getAttributex("spriteset", "");
			int row = xml->getAttributex("row", 0);
			int column = xml->getAttributex("column", 0);

			SetSpriteNode(scene, node, layer, spriteSet, row, column);
		}
		else if (renderType == RenderType::BOUNDING_BOX) {
			string colorStr = xml->getAttributex("color", "0x000000");
			ofColor color = StringToColor(colorStr);

			bool renderable = xml->getBoolAttributex("renderable", false);
			float margin = xml->getAttributex("margin", 0.0f);

			SetBoundingBoxNode(scene, node, color, margin,renderable);
		}
	}

} // namespace