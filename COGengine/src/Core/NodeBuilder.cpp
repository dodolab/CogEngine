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
		node->GetStates().SetState(StrId(STATE_HITTABLE));
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
		int refWidth = settings.GetSettingVal<int>("transform", "ref_width");
		int refHeight = settings.GetSettingVal<int>("transform", "ref_height");

		if (refWidth == 0) refWidth = CogGetScreenWidth();
		if (refHeight == 0) refHeight = CogGetScreenHeight();

		// create bounding box
		auto bbox = spt<BoundingBoxMesh>(new BoundingBoxMesh((float)refWidth, (float)refHeight, margin, renderable));
		bbox->SetColor(color);
		node->SetMesh(bbox);
	}

	spt<SpriteMesh> NodeBuilder::CreateSpriteMesh(Scene* scene, string layer, string spriteSheet, int row, int column) {

		auto cache = GETCOMPONENT(Resources);
		auto sheet = cache->GetSpriteSheet(spriteSheet);

		if (!sheet) CogLogError("NodeBuilder", "Error while loading sprite sheet. SpriteSheet %s not found!", spriteSheet.c_str());


		Sprite sprite = Sprite(sheet, row, column);
		auto shape = spt<SpriteMesh>(new SpriteMesh(sprite, layer));
		return shape;
	}

	spt<SpriteMesh> NodeBuilder::CreateSpriteMesh(Scene* scene, string layer, int row, int column) {
		return CreateSpriteMesh(scene, layer, "", row, column);
	}

	spt<FRect> NodeBuilder::CreateRectangleMesh(ofVec2f size, ofColor color) {
		spt<FRect> plane = spt<FRect>(new FRect(size.x, size.y));
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

		Node* node = new Node(NODETYPE_OBJECT, 0, name);
		return node;
	}


	Node* NodeBuilder::LoadNodeFromXml(xml_node& xml, Node* parent, Scene* scene) {

		auto refAttrib = xml.attribute("ref");
		if (refAttrib) {
			// load referenced node
			Node* nodeToReturn = LoadRefNodeFromXml(xml, refAttrib.as_string(""), parent, scene);
			return nodeToReturn;
		}

		TransformMath math = TransformMath();
		Settings& settings = scene->GetSceneSettings();

		// in xml the tag can be under either tag or name attribute
		string name = xml.attribute("tag").as_string(xml.attribute("name").as_string());
		string img = xml.attribute("img").as_string();

		Node* node = CreateNode(name, scene);

		if (!img.empty()) {
			CreateImageNode(node, img);
		}

		auto clickAttrib = xml.attribute("img_click");
		if (clickAttrib) {
			// set image on click
			string imgClick = clickAttrib.as_string();
			string imgDisabled = xml.attribute("img_disabled").as_string();

			CreateButtonNode(node, img, imgClick, imgDisabled);
		}

		auto multiSelAttrib = xml.attribute("img_multiselect");
		if (multiSelAttrib) {
			// set image on selection
			string imgSelect = multiSelAttrib.as_string();
			string selectGroup = xml.attribute("select_group").as_string();
			CreateMultiSelectionNode(node, img, imgSelect, selectGroup);
		}

		auto selectAttrib = xml.attribute("img_select");
		if (selectAttrib) {
			// set image on selection
			string imgSelect = selectAttrib.as_string();
			CreateSelectionNode(node, img, imgSelect);
		}

		auto animAttrib = xml.attribute("animation");
		if (animAttrib) {
			// set animation
			string animName = animAttrib.as_string();
			CreateAnimationNode(node, animName);
		}

		auto meshXml = xml.child("mesh");
		if (meshXml) {
			// load mesh
			LoadMeshFromXml(meshXml, node, scene);
		}

		// text must be loaded before transform
		auto textXml = xml.child("text");
		if (textXml) {
			LoadTextFromXml(textXml, node, parent);
		}

		auto labelXml = xml.child("label");
		if (labelXml) {
			LoadLabelFromXml(labelXml, node, parent);
		}

		auto transXml = xml.child("transform");
		if (transXml) {
			// load transformation
			TransformEnt transformEnt = TransformEnt();
			transformEnt.LoadFromXml(transXml, settings.GetSetting("transform"));

			// =================== get grid size (if specified)
			int gridWidth = settings.GetSettingVal<int>("transform", "grid_width");
			int gridHeight = settings.GetSettingVal<int>("transform", "grid_height");

			TransformMath math = TransformMath();
			// set transform according to the parsed values
			math.SetTransform(node, parent, transformEnt, gridWidth, gridHeight);
		}
		else {
			// set transform to match the whole scene
			TransformMath math = TransformMath();
			math.SetSizeToScreen(node, parent);
		}

		// load behaviors
		for (auto behavior : xml.children("behavior")) {
			LoadBehaviorFromXml(behavior, node);
		}


		// load states
		for (auto stateNode : xml.children("state")) {
			string stateName = stateNode.value();
			node->GetStates().SetState(StrId(stateName));
		}

		// load children
		for (auto nodeXml : xml.children("node")) {
			Node* child = LoadNodeFromXml(nodeXml, node, scene);
			if (child != nullptr) {
				node->AddChild(child);
			}
		}
		return node;
	}

	Node* NodeBuilder::LoadRefNodeFromXml(xml_node& contextXml, string nodeName, Node* parent, Scene* scene) {
		string nodes = ofToDataPath(PATH_NODES);

		// load the file from scratch
		auto nodesXml = CogPreloadXMLFile(nodes);

		for (auto nodeXmlPath : nodesXml->select_nodes("/resources/node")) {
			auto nodeXml = nodeXmlPath.node();
			string name = nodeXml.attribute("tag").as_string(nodeXml.attribute("name").as_string());
			if (name.compare(nodeName) == 0) {
				// use nodesXml to load its child 
				Node* nodeToReturn = LoadNodeFromXml(nodeXml, parent, scene);
				return nodeToReturn;
			}
		}


		CogLogError("NodeBuilder", "Error while loading referenced node %s. All referenced nodes must be located in nodes.xml file!", nodeName.c_str());
		return nullptr;
	}

	void NodeBuilder::LoadTextFromXml(xml_node& xml, Node* node, Node* parent) {
		string font = xml.attribute("font").as_string();
		float size = xml.attribute("size").as_float(1.0f);
		string value = checkResource(xml.text().as_string());
		string colorStr = xml.attribute("color").as_string("0x000000");
		ofColor color = EnumConverter::StrToColor(colorStr);

		CreateTextNode(node, font, size, color, value);
	}

	void NodeBuilder::LoadLabelFromXml(xml_node& xml, Node* node, Node* parent) {
		string font = xml.attribute("font").as_string();
		float width = xml.attribute("width").as_float(640);
		float size = xml.attribute("font_size").as_float(1.0);
		string value = checkResource(xml.text().as_string());
		string colorStr = xml.attribute("color").as_string("0x000000");
		ofColor color = EnumConverter::StrToColor(colorStr);

		CreateLabelNode(node, font, width, size, color, value);
	}

	void NodeBuilder::LoadBehaviorFromXml(xml_node& xml, Node* node) {
		spt<BehaviorEnt> ent = spt<BehaviorEnt>(new BehaviorEnt());
		auto dummySet = Setting();
		ent->LoadFromXml(xml, dummySet);

		Behavior* behavior = CreateBehavior(ent);

		node->AddBehavior(behavior);
	}

	void NodeBuilder::LoadMeshFromXml(xml_node& xml, Node* node, Scene* scene) {
		string type = xml.attribute("type").as_string();

		// get type of the mesh
		int renderType = EnumConverter::StrToMeshType(type);

		if (renderType == MESH_IMAGE) {
			string img = xml.attribute("img").as_string();
			this->CreateImageNode(node, img);
		}
		else if (renderType == MESH_TEXT) {
			this->LoadTextFromXml(xml, node, node->GetParent());
		}
		else if (renderType == MESH_LABEL) {
			this->LoadLabelFromXml(xml, node, node->GetParent());
		}
		else if (renderType == MESH_RECTANGLE) {
			float width = 0;
			float height = 0;

			// load other attributes
			auto sizeAttr = xml.attribute("size");
			if (sizeAttr) {
				width = height = sizeAttr.as_float(1.0f);
			}
			else {
				// default size 1x1
				width = xml.attribute("width").as_float(1);
				height = xml.attribute("height").as_float(1);
			}

			ofVec2f size = ofVec2f(width, height);
			string colorStr = xml.attribute("color").as_string("0x000000");
			ofColor color = EnumConverter::StrToColor(colorStr);
			bool noFill = xml.attribute("no_fill").as_bool(false);
			CreateRectangleNode(node, size, color, noFill);
		}
		else if (renderType == MESH_SPRITE) {
			string layer = xml.attribute("layer").as_string();

			if (layer.empty()) CogLogError("NodeBuilder", "Error while loading sprite sheet. Layer not specified (node %s)", node->GetTag().c_str());

			string spriteSet = xml.attribute("spriteset").as_string();
			int row = xml.attribute("row").as_int(0);
			int column = xml.attribute("column").as_int(0);

			CreateSpriteNode(scene, node, layer, spriteSet, row, column);
		}
		else if (renderType == MESH_BOUNDING_BOX) {
			string colorStr = xml.attribute("color").as_string("0x000000");
			ofColor color = EnumConverter::StrToColor(colorStr);

			bool renderable = xml.attribute("renderable").as_bool(false);
			float margin = xml.attribute("margin").as_float(0.0f);

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