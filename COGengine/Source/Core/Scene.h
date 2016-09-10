#pragma once

#include "ofxCogMain.h"
#include "ResourceCache.h"
#include "Node.h"
#include "Component.h"
#include "Engine.h"
#include "TransformMath.h";

namespace Cog
{
	class Scene
	{
	private:
		string name;
		map<string, Setting> settings;
		Node* sceneNode;

	public:

		Node* GetSceneNode() {
			return sceneNode;
		}

		string GetName() {
			return name;
		}

		void SetName(string name) {
			this->name = name;
		}

		map<string, Setting>& GetSettings() {
			return settings;
		}

		Setting& GetSettings(string key) {
			return settings[key];
		}


		void LoadFromXml(spt<ofxXml> xml) {

			auto cache = GETCOMPONENT(ResourceCache);

			SetName(xml->getAttribute(":", "name", ""));

			sceneNode = new Node(ObjType::SCENE, 0, name);

			if (xml->tagExists("scene_settings")) {
				xml->pushTag("scene_settings");

				settings = cache->LoadSettingsFromXml(xml);

				xml->popTag();
			}

			int nodes = xml->getNumTags("node");

			for (int i = 0; i < nodes; i++) {
				xml->pushTag("node", i);

				Node* node = LoadNodeFromXml(xml, sceneNode);
				sceneNode->AddChild(node);

				xml->popTag();
			}
	
			sceneNode->SubmitChanges(true);
		}

		Node* LoadNodeFromXml(spt<ofxXml> xml, Node* parent) {
			TransformMath math = TransformMath();

			string name = xml->getAttribute(":", "name", "");
			string type = xml->getAttribute(":", "type", "");
			string img = xml->getAttribute(":", "img", "");

			Node* node = new Node(ObjType::OBJECT, 0, name);
			if (!img.empty()) {
				// load image
				spt<ofImage> image = CogGet2DImage(img);
				node->SetShape(spt<Image>(new Image(image)));
			}

			if (!type.empty()) {
				if (type.compare("background") == 0) {
					// set background image
					math.SetSizeToScreen(node, parent);
				}
			}

			if (xml->tagExists("transform")) {
				xml->pushTag("transform");
				LoadTransformFromXml(xml, node, parent);
				xml->popTag();
			}

			if (xml->tagExists("node")) {
				int children = xml->getNumTags("node");
				
				for (int i = 0; i < children; i++) {
					xml->pushTag("node", i);

					Node* child = LoadNodeFromXml(xml, node);
					node->AddChild(child);

					xml->popTag();
				}
			}

			if (xml->tagExists("text")) {
				xml->pushTag("text");
				// todo
				xml->popTag();
			}

			return node;
		}

		void LoadTransformFromXml(spt<ofxXml> xml, Node* node, Node* parent) {
			TransformMath math = TransformMath();

			ofVec2f pos = ofVec2f();
			int zIndex = 0;
			CalcType posCalc = CalcType::PER;
			CalcType sizeCalc = CalcType::PER;
			ofVec2f anchor = ofVec2f();
			ofVec2f size = ofVec2f();

			if (xml->attributeExists(":", "pos")) {
				float posF = xml->getAttribute(":", "pos", 0.0);
				pos = ofVec2f(posF, posF);
			}
			else {

				float posX = 0;
				float posY = 0;

				posX = xml->getAttribute(":", "pos_x", 0.0);
				posY = xml->getAttribute(":", "pos_y", 0.0);

				pos = ofVec2f(posX, posY);
			}

			zIndex = xml->getAttribute(":", "z_index", 0);
			posCalc = StrToCalcType(xml->getAttribute(":", "ptype", ""));
			sizeCalc = StrToCalcType(xml->getAttribute(":", "stype", ""));

			float width = 0;
			float height = 0;

			width = xml->getAttribute(":", "width", 0.0);
			height = xml->getAttribute(":", "height", 0.0);

			size = ofVec2f(width, height);

			if (xml->attributeExists(":", "anchor")) {
				float anchorFlt = xml->getAttribute(":", "anchor", 0.0);
				anchor = ofVec2f(anchorFlt);
			}
			else {
				anchor.x = xml->getAttribute(":", "anchor_x", 0.0);
				anchor.y = xml->getAttribute(":", "anchor_y", 0.0);
			}

			int gridWidth = GetSettings("transform").GetItem("grid_width").GetValInt();
			int gridHeight = GetSettings("transform").GetItem("grid_height").GetValInt();


			math.SetTransform(node, parent, pos, zIndex, posCalc, anchor, size, sizeCalc, gridWidth, gridHeight);
		}

		CalcType StrToCalcType(string val) {
			if (val.compare("per") == 0) return CalcType::PER;
			else if (val.compare("abs") == 0) return CalcType::ABS;
			else if (val.compare("grid") == 0) return CalcType::GRID;
			else if (val.compare("absper") == 0) return CalcType::ABS_PER;
			else if (val.compare("loc") == 0) return CalcType::LOC;
			
			return CalcType::PER;
		}
	};

} // namespace
