#include "NodeBuilder.h"
#include "Node.h"
#include "Behavior.h"
#include "Scene.h"
#include "HitEvent.h"
#include "Button.h"
#include "TransformMath.h"
#include "CogEngine.h"
#include "BehaviorEnt.h"
#include "Selection.h"
#include "MultiSelection.h"
#include "Scene.h"
#include "EnumConverter.h"
#include "SpriteSheet.h"


namespace Cog {


	void NodeBuilder::CreateButtonNode(Node* node, string defaultImg, string clickedImg, string disabledImg) {

		spt<ofImage> disabledImgPtr = disabledImg.empty() ? spt<ofImage>() : CogPreload2DImage(disabledImg);

		node->AddBehavior(new HitEvent(-1, false, false));
		node->AddBehavior(new Button(CogPreload2DImage(defaultImg), CogPreload2DImage(clickedImg), disabledImgPtr));
		node->GetStates().SetState(StrId(STATES_HITTABLE));
	}


	void NodeBuilder::CreateMultiSelectionNode(Node* node, string defaultImg, string selectImg, string selectionGroup) {
		node->AddBehavior(new HitEvent(-1, false, false));
		node->AddBehavior(new MultiSelection(CogPreload2DImage(defaultImg), CogPreload2DImage(selectImg), StrId(selectionGroup)));
		node->GetGroups().SetState(StrId(selectionGroup));
	}

	void NodeBuilder::CreateSelectionNode(Node* node, string defaultImg, string selectImg) {
		node->AddBehavior(new Selection(CogPreload2DImage(defaultImg), CogPreload2DImage(selectImg)));
	}

	void NodeBuilder::CreateImageNode(Node* node, string path) {
		spt<ofImage> image = CogPreload2DImage(path);
		node->SetMesh(spt<Image>(new Image(image)));
	}

	void NodeBuilder::CreateTextNode(Node* node, string font, float size, ofColor color, string text) {
		spt<ofTrueTypeFont> fontVal = CogGetFont(font, (int)size);

		auto textShape = new Text(fontVal, text);
		textShape->SetColor(color);
		node->SetMesh(spt<Text>(textShape));
	}

	void NodeBuilder::CreateSpriteNode(Scene* scene, Node* node, string layer, string spriteSet, int row, int column) {
		auto spriteShape = CreateSpriteMesh(scene, layer, spriteSet, row, column);
		node->SetMesh(spriteShape);
	}

	void NodeBuilder::CreateSpriteNode(Scene* scene, Node* node, string layer, int row, int column) {
		auto spriteShape = CreateSpriteMesh(scene, layer, row, column);
		node->SetMesh(spriteShape);
	}

	void NodeBuilder::CreatePlaneNode(Node* node, ofVec2f size, ofColor color, bool noFill) {
		auto planeShape = CreatePlaneShape(size, color);
		planeShape->SetNoFill(noFill);
		node->SetMesh(planeShape);
	}

	void NodeBuilder::CreateBoundingBoxNode(Scene* scene, Node* node, ofColor color, float margin, bool renderable) {
		Settings& settings = scene->GetSettings();

		// get reference width and height from scene settings
		int refWidth = settings.GetSettingValInt("transform", "ref_width");
		int refHeight = settings.GetSettingValInt("transform", "ref_height");

		if (refWidth == 0) refWidth = CogGetScreenWidth();
		if (refHeight == 0) refHeight = CogGetScreenHeight();

		// create bounding box
		auto bbox = spt<BoundingBox>(new BoundingBox((float)refWidth, (float)refHeight,margin,renderable));
		bbox->SetColor(color);
		node->SetMesh(bbox);
	}

	spt<SpriteMesh> NodeBuilder::CreateSpriteMesh(Scene* scene, string layer, string spriteSet, int row, int column) {

		LayerEnt layerEntity = scene->FindLayerSettings(layer);
		string spriteSheetName = layerEntity.spriteSheetName;

		auto cache = GETCOMPONENT(ResourceCache);
		auto spriteSheet = cache->GetSpriteSheet(spriteSheetName);

		if (!spriteSheet) CogLogError("NodeBuilder", "Error while loading sprite sheet. SpriteSheet %s not found!", spriteSheetName.c_str());

		spt<SpriteSet> spriteSetEntity;

		if (spriteSet.empty()) {
			// load default
			spriteSetEntity = spriteSheet->GetDefaultSpriteSet();
		}
		else {
			// load by name
			spriteSetEntity = spriteSheet->GetSpriteSetByName(spriteSet);
		}

		if (!spriteSetEntity) CogLogError("NodeBuilder", "Spriteset %s not found!", spriteSet.c_str());

		Sprite sprite = Sprite(spriteSetEntity, row, column);
		auto shape = spt<SpriteMesh>(new SpriteMesh(sprite, spriteSetEntity, layer));
		return shape;
	}

	spt<SpriteMesh> NodeBuilder::CreateSpriteMesh(Scene* scene, string layer, int row, int column) {
		return CreateSpriteMesh(scene, layer, "", row, column);
	}

	spt<Plane> NodeBuilder::CreatePlaneShape(ofVec2f size, ofColor color) {
		spt<Plane> plane = spt<Plane>(new Plane(size.x, size.y));
		plane->SetColor(color);
		return plane;
	}

	Behavior* NodeBuilder::CreateBehavior(spt<BehaviorEnt> entity) {
		Behavior* behavior = nullptr;

		auto resourceCache = GETCOMPONENT(ResourceCache);

		// entity has two attributes: ref and type. If the type is filled,
		// behavior is created directly by its type. If the ref is filled,
		// a reference behavior entity is loaded and the behavior prototype
		// is created according to this entity (BehaviorEnt)

		if (!entity->type.empty()) {
			// create prototype
			behavior = CogGetEntityStorage()->CreateBehaviorPrototype(entity->type);
			if (!entity->setting.Empty()) {
				// use setting loader
				behavior->Load(entity->setting);
			}
		}
		else {
			// load from reference behavior descriptor
			spt<BehaviorEnt> refent = resourceCache->GetEntity<BehaviorEnt>(entity->ref);
			
			behavior = CogGetEntityStorage()->CreateBehaviorPrototype(refent->type);
			if (!refent->setting.Empty()) behavior->Load(refent->setting);
		}

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
		node->SetMesh(spt<Rectangle>(new Rectangle((float)refWidth, (float)refHeight)));

		return node;
	}

	Node* NodeBuilder::LoadNodeFromXml(spt<ofxXml> xml, Node* parent, Scene* scene) {

		TransformMath math = TransformMath();
		Settings& settings = scene->GetSettings();

		string name = xml->getAttributex("name", "");
		string img = xml->getAttributex("img", "");

		Node* node = CreateNode(name, scene);

		if (!img.empty()) {
			CreateImageNode(node, img);
		}

		if (xml->attributeExists("img_click")) {
			// set image on click
			string imgClick = xml->getAttributex("img_click", "");
			string imgDisabled = xml->getAttributex("img_disabled", "");

			CreateButtonNode(node, img, imgClick, imgDisabled);
		}

		if (xml->attributeExists("img_multiselect")) {
			// set image on selection
			string imgSelect = xml->getAttributex("img_multiselect", "");
			string selectGroup = xml->getAttributex("select_group", "");
			CreateMultiSelectionNode(node, img, imgSelect, selectGroup);
		}

		if (xml->attributeExists("img_select")) {
			// set image on selection
			string imgSelect = xml->getAttributex("img_select", "");
			CreateSelectionNode(node, img, imgSelect);
		}

		// scene node will always fit to screen size
		if (parent->GetType() == NodeType::SCENE) {
			math.SetSizeToScreen(node, parent);
		}

		if (xml->pushTagIfExists("shape")) {
			// load mesh
			LoadMeshFromXml(xml, node, scene);
			xml->popTag();
		}

		// text must be loaded before transform
		if (xml->pushTagIfExists("text")) {
			LoadTextFromXml(xml, node, parent);
			xml->popTag();
		}

		if (xml->pushTagIfExists("transform")) {
			// load transformation
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
			// load behaviors
			for (int i = 0; i < behaviors; i++) {
				xml->pushTag("behavior", i);
				LoadBehaviorFromXml(xml, node);
				xml->popTag();
			}
		}

		if (xml->tagExists("state")) {
			int states = xml->getNumTags("state");
			// load states
			for (int i = 0; i < states; i++) {
				string stateName = xml->getValue("state", "", i);
				node->GetStates().SetState(StrId(stateName));
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
		ofColor color = EnumConverter::StrToColor(colorStr);

		CreateTextNode(node, font, size, color, value);
	}

	void NodeBuilder::LoadBehaviorFromXml(spt<ofxXml> xml, Node* node) {
		auto resourceCache = GETCOMPONENT(ResourceCache);
		spt<BehaviorEnt> ent = spt<BehaviorEnt>(new BehaviorEnt());
		auto dummySet = Setting();
		ent->LoadFromXml(xml, dummySet);

		Behavior* behavior = CreateBehavior(ent);

		node->AddBehavior(behavior);
	}

	void NodeBuilder::LoadMeshFromXml(spt<ofxXml> xml, Node* node, Scene* scene) {
		string type = xml->getAttributex("type", "");
		
		// get type of the mesh
		MeshType renderType = EnumConverter::StrToMeshType(type);

		if (renderType == MeshType::IMAGE) {
			string img = xml->getAttributex("img", "");
			this->CreateImageNode(node, img);
		}
		else if (renderType == MeshType::PLANE) {
			float width = 0;
			float height = 0;

			// load other attributes
			if (xml->attributeExists("size")) {
				width = height = xml->getAttributex("size", 1.0);
			}
			else {
				width = xml->getAttributex("width", 0);
				height = xml->getAttributex("height", 0);
			}

			ofVec2f size = ofVec2f(width, height);
			string colorStr = xml->getAttributex("color", "0x000000");
			ofColor color = EnumConverter::StrToColor(colorStr);
			bool noFill = xml->getBoolAttributex("no_fill", false);
			CreatePlaneNode(node, size, color, noFill);
		}
		else if (renderType == MeshType::SPRITE) {
			string layer = xml->getAttributex("layer", "");

			if (layer.empty()) CogLogError("NodeBuilder", "Error while loading sprite sheet. Layer not specified (node %s)", node->GetTag().c_str());

			string spriteSet = xml->getAttributex("spriteset", "");
			int row = xml->getAttributex("row", 0);
			int column = xml->getAttributex("column", 0);

			CreateSpriteNode(scene, node, layer, spriteSet, row, column);
		}
		else if (renderType == MeshType::BOUNDING_BOX) {
			string colorStr = xml->getAttributex("color", "0x000000");
			ofColor color = EnumConverter::StrToColor(colorStr);

			bool renderable = xml->getBoolAttributex("renderable", false);
			float margin = xml->getAttributex("margin", 0.0f);

			CreateBoundingBoxNode(scene, node, color, margin,renderable);
		}
	}

} // namespace