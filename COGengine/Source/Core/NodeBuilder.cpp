#include "NodeBuilder.h"
#include "Node.h"
#include "Behavior.h"
#include "Scene.h"
#include "HitEvent.h"
#include "Button.h"
#include "TransformMath.h"
#include "ofxCogEngine.h"
#include "BehaviorEnt.h"
#include "Selection.h"
#include "MultiSelection.h"
#include "Scene.h"
#include "EnumConverter.h"
#include "SpriteSheet.h"
#include "SheetAnimator.h"

namespace Cog {


	void NodeBuilder::CreateButtonNode(Node* node, string defaultImg, string clickedImg, string disabledImg) {

		spt<ofImage> disabledImgPtr = disabledImg.empty() ? spt<ofImage>() : CogGet2DImage(disabledImg);

		node->AddBehavior(new HitEvent(-1, false, false));
		node->AddBehavior(new Button(CogGet2DImage(defaultImg), CogGet2DImage(clickedImg), disabledImgPtr));
		node->GetStates().SetState(StrId(STATES_HITTABLE));
	}


	void NodeBuilder::CreateMultiSelectionNode(Node* node, string defaultImg, string selectImg, string selectionGroup) {
		node->AddBehavior(new HitEvent(-1, false, false));
		node->AddBehavior(new MultiSelection(CogGet2DImage(defaultImg), CogGet2DImage(selectImg), StrId(selectionGroup)));
		node->GetGroups().SetState(StrId(selectionGroup));
	}

	void NodeBuilder::CreateSelectionNode(Node* node, string defaultImg, string selectImg) {
		node->AddBehavior(new Selection(CogGet2DImage(defaultImg), CogGet2DImage(selectImg)));
	}

	void NodeBuilder::CreateAnimationNode(Node* node, string animName) {
		spt<SheetAnim> animation = CogGetSheetAnimation(animName);
		node->AddBehavior(new SheetAnimator(animation));
	}

	void NodeBuilder::CreateImageNode(Node* node, string path) {
		spt<ofImage> image = CogGet2DImage(path);
		node->SetMesh(spt<Image>(new Image(image)));
	}

	void NodeBuilder::CreateTextNode(Node* node, string font, float size, ofColor color, string text) {
		spt<ofTrueTypeFont> fontVal = CogGetFont(font, (int)size);

		auto textShape = new Text(fontVal, text);
		textShape->SetColor(color);
		node->SetMesh(spt<Text>(textShape));
	}

	void NodeBuilder::CreateLabelNode(Node* node, string font, float width, float size, ofColor color, string text) {
		spt<ofTrueTypeFont> fontVal = CogGetFont(font, (int)size);
		spt<Label> lab = spt<Label>(new Label(fontVal, text, width));
		node->SetMesh(lab);
	}

	void NodeBuilder::CreateSpriteNode(Scene* scene, Node* node, string layer, string spriteSet, int row, int column) {
		auto spriteShape = CreateSpriteMesh(scene, layer, spriteSet, row, column);
		node->SetMesh(spriteShape);
	}

	void NodeBuilder::CreateSpriteNode(Scene* scene, Node* node, string layer, int row, int column) {
		auto spriteShape = CreateSpriteMesh(scene, layer, row, column);
		node->SetMesh(spriteShape);
	}

	void NodeBuilder::CreateRectangleNode(Node* node, ofVec2f size, ofColor color, bool noFill) {
		auto planeShape = CreateRectangleMesh(size, color);
		planeShape->SetNoFill(noFill);
		node->SetMesh(planeShape);
	}


	void NodeBuilder::CreateBoundingBoxNode(Scene* scene, Node* node, ofColor color, float margin, bool renderable) {
		Settings& settings = scene->GetSceneSettings();

		// get reference width and height from scene settings
		int refWidth = settings.GetSettingValInt("transform", "ref_width");
		int refHeight = settings.GetSettingValInt("transform", "ref_height");

		if (refWidth == 0) refWidth = CogGetScreenWidth();
		if (refHeight == 0) refHeight = CogGetScreenHeight();

		// create bounding box
		auto bbox = spt<BoundingBox>(new BoundingBox((float)refWidth, (float)refHeight, margin, renderable));
		bbox->SetColor(color);
		node->SetMesh(bbox);
	}

	spt<SpriteMesh> NodeBuilder::CreateSpriteMesh(Scene* scene, string layer, string spriteSet, int row, int column) {

		LayerEnt layerEntity = scene->FindLayerSettings(layer);
		string spriteSheetName = layerEntity.spriteSheetName;

		auto cache = GETCOMPONENT(Resources);
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

	spt<Rectangle> NodeBuilder::CreateRectangleMesh(ofVec2f size, ofColor color) {
		spt<Rectangle> plane = spt<Rectangle>(new Rectangle(size.x, size.y));
		plane->SetColor(color);
		return plane;
	}


	Behavior* NodeBuilder::CreateBehavior(spt<BehaviorEnt> entity) {
		Behavior* behavior = nullptr;

		auto resources = GETCOMPONENT(Resources);

		// entity has two attributes: ref and type. If the type is filled,
		// behavior is created directly by its type. If the ref is filled,
		// a reference behavior entity is loaded and the behavior prototype
		// is created according to this entity (BehaviorEnt)

		if (!entity->type.empty()) {
			// create prototype
			behavior = CogGetComponentStorage()->CreateBehaviorPrototype(entity->type);
			if (!entity->setting.Empty()) {
				// use setting loader
				behavior->Load(entity->setting);
			}
		}
		else {
			// load from reference behavior descriptor
			spt<BehaviorEnt> refent = resources->GetEntity<BehaviorEnt>(entity->ref);

			behavior = CogGetComponentStorage()->CreateBehaviorPrototype(refent->type);
			if (!refent->setting.Empty()) behavior->Load(refent->setting);
		}

		return behavior;
	}

	Node* NodeBuilder::CreateNode(string name, Scene* scene) {

		Node* node = new Node(NodeType::OBJECT, 0, name);
		return node;
	}


	Node* NodeBuilder::LoadNodeFromXml(spt<ofxXml> xml, Node* parent, Scene* scene) {

		if (xml->getAttributex("ref", "").length() != 0) {
			// load referenced node
			Node* nodeToReturn = LoadRefNodeFromXml(xml, xml->getAttributex("ref", ""), parent, scene);
			return nodeToReturn;
		}

		TransformMath math = TransformMath();
		Settings& settings = scene->GetSceneSettings();

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

		if (xml->attributeExists("animation")) {
			// set animation
			string animName = xml->getAttributex("animation", "");
			CreateAnimationNode(node, animName);
		}

		if (xml->pushTagIfExists("mesh")) {
			// load mesh
			LoadMeshFromXml(xml, node, scene);
			xml->popTag();
		}

		// text must be loaded before transform
		if (xml->pushTagIfExists("text")) {
			LoadTextFromXml(xml, node, parent);
			xml->popTag();
		}

		if (xml->pushTagIfExists("label")) {
			LoadLabelFromXml(xml, node, parent);
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
		else {
			// set transform to match the whole scene
			TransformMath math = TransformMath();
			math.SetSizeToScreen(node, parent);
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
				if (child != nullptr) {
					node->AddChild(child);
				}
				xml->popTag();
			}
		}
		return node;
	}

	Node* NodeBuilder::LoadRefNodeFromXml(spt<ofxXml> contextXml, string nodeName, Node* parent, Scene* scene) {
		string nodes = ofToDataPath(PATH_NODES);

		if (ofFile(nodes.c_str()).exists()) {
			// load the file from scratch
			spt<ofxXml> nodesXml = spt<ofxXml>(new ofxXml());
			nodesXml->loadFile(nodes);

			nodesXml->pushTag("resources");
			int nodes = nodesXml->getNumTags("node");

			for (int i = 0; i < nodes; i++) {
				nodesXml->pushTag("node", i);
				string name = nodesXml->getAttributex("name", "");
				if (name.compare(nodeName) == 0) {
					// use nodesXml to load its child 
					Node* nodeToReturn = LoadNodeFromXml(nodesXml, parent, scene);
					return nodeToReturn;
				}
				nodesXml->popTag();
			}
		}

		CogLogError("NodeBuilder", "Error while loading referenced node %s. All referenced nodes must be located in nodes.xml file!", nodeName.c_str());
		return nullptr;
	}

	void NodeBuilder::LoadTextFromXml(spt<ofxXml> xml, Node* node, Node* parent) {
		string font = xml->getAttributex("font", "");
		float size = xml->getAttributex("size", 1.0);
		string value = checkResource(xml->getValuex(""));
		string colorStr = xml->getAttributex("color", "0x000000");
		ofColor color = EnumConverter::StrToColor(colorStr);

		CreateTextNode(node, font, size, color, value);
	}

	void NodeBuilder::LoadLabelFromXml(spt<ofxXml> xml, Node* node, Node* parent) {
		string font = xml->getAttributex("font", "");
		float width = xml->getAttributex("width", 640);
		float size = xml->getAttributex("font_size", 1.0);
		string value = checkResource(xml->getValuex(""));
		string colorStr = xml->getAttributex("color", "0x000000");
		ofColor color = EnumConverter::StrToColor(colorStr);

		CreateLabelNode(node, font, width, size, color, value);
	}

	void NodeBuilder::LoadBehaviorFromXml(spt<ofxXml> xml, Node* node) {
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
		else if (renderType == MeshType::TEXT) {
			this->LoadTextFromXml(xml, node, node->GetParent());
		}
		else if (renderType == MeshType::LABEL) {
			this->LoadLabelFromXml(xml, node, node->GetParent());
		}
		else if (renderType == MeshType::RECTANGLE) {
			float width = 0;
			float height = 0;

			// load other attributes
			if (xml->attributeExists("size")) {
				width = height = xml->getAttributex("size", 1.0);
			}
			else {
				// default size 1x1
				width = xml->getAttributex("width", 1);
				height = xml->getAttributex("height", 1);
			}

			ofVec2f size = ofVec2f(width, height);
			string colorStr = xml->getAttributex("color", "0x000000");
			ofColor color = EnumConverter::StrToColor(colorStr);
			bool noFill = xml->getBoolAttributex("no_fill", false);
			CreateRectangleNode(node, size, color, noFill);
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

			CreateBoundingBoxNode(scene, node, color, margin, renderable);
		}

	}

	string NodeBuilder::checkResource(string value) {
		if (!value.empty() && value.at(0) == '@') {
			int slashIndex = value.find('/');

			if (slashIndex != -1) {
				string resourceType = value.substr(1, slashIndex - 1);
				string resourceKey = value.substr(slashIndex + 1);
				auto resources = GETCOMPONENT(Resources);
				string realValue = resources->GetResourceStr(resourceType, resourceKey);
				return realValue;
			}
			else {
				return value;
			}
		}
		else {
			return value;
		}
	}

} // namespace