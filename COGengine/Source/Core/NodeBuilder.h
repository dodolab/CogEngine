#pragma once

#include "ofxCogMain.h"
#include "Button.h"
#include "HitEvent.h"
#include "Node.h"
#include "ResourceCache.h"
#include "TransformMath.h"

namespace Cog {

	/**
	* Node is complex object itself, so construction mechanism has been moved to separate class
	*/
	class NodeBuilder {

	private:

	public:

		void AssignImage(Node* node, string path) {
			spt<ofImage> image = CogGet2DImage(path);
			node->SetShape(spt<Image>(new Image(image)));
		}

		void AssignButton(Node* node, string defaultImg, string clickedImg, string disabledImg) {

			spt<ofImage> disabledImgPtr = disabledImg.empty() ? spt<ofImage>() : CogGet2DImage(disabledImg);

			node->AddBehavior(new HitEvent(-1, false));
			node->AddBehavior(new Button(CogGet2DImage(defaultImg),
				CogGet2DImage(clickedImg), disabledImgPtr));
			node->SetState(StringHash(STATES_HITTABLE));
		}

		void AssignText(Node* node, string font, double size, ofColor color, string text) {
			spt<ofTrueTypeFont> fontVal = CogGetFont(font, size);

			auto textShape = new Text(fontVal, text, size);
			textShape->SetColor(color);
			node->SetShape(spt<Text>(textShape));
		}

		/**
		* Loads node from XML
		*/
		Node* LoadNodeFromXml(spt<ofxXml> xml, Node* parent, Settings& settings) {
			TransformMath math = TransformMath();


			string name = xml->getAttribute(":", "name", "");
			string type = xml->getAttribute(":", "type", "");
			string img = xml->getAttribute(":", "img", "");

			Node* node = new Node(ObjType::OBJECT, 0, name);
			if (!img.empty()) {
				AssignImage(node, img);
			}

			if (xml->attributeExists(":", "img_click")) {
				// set image on click
				string imgClick = xml->getAttribute(":", "img_click", "");
				string imgDisabled = xml->getAttribute(":", "img_disabled", "");

				AssignButton(node, img, imgClick, imgDisabled);
			}

			if (!type.empty()) {
				if (type.compare("background") == 0) {
					// set background image
					math.SetSizeToScreen(node, parent);
				}
			}

			// text must be loaded before transform
			if (xml->tagExists("text")) {
				xml->pushTag("text");
				LoadTextFromXml(xml, node, parent);
				xml->popTag();
			}

			if (xml->tagExists("transform")) {
			// load transform
				xml->pushTag("transform");
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
					node->SetState(StringHash(stateName));
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

		void LoadTextFromXml(spt<ofxXml> xml, Node* node, Node* parent) {
			string font = xml->getAttribute(":", "font", "");
			double size = xml->getAttribute(":", "size", 1.0);
			string value = xml->getValue(":", "");
			string colorStr = xml->getAttribute(":", "color", "0x000000");
			ofColor color = ofColor::fromHex(ofHexToInt(colorStr));

			AssignText(node, font, size, color, value);
		}

		void LoadBehaviorFromXml(spt<ofxXml> xml, Node* node);

	};

}// namespace