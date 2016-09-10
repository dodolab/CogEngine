#pragma once

#include "ofxCogCommon.h"
#include "Button.h"
#include "HitEvent.h"
#include "Node.h"
#include "ResourceCache.h"
#include "TransformMath.h"
#include "Selection.h"
#include "BehaviorEnt.h"

namespace Cog {

	/**
	* Node is complex object itself, so construction mechanism has been moved to separate class
	*/
	class NodeBuilder {

	private:

	public:

		void SetImageNode(Node* node, string path);

		void SetButtonNode(Node* node, string defaultImg, string clickedImg, string disabledImg);

		void SetSelectionNode(Node* node, string defaultImg, string selectImg, string selectionGroup);

		void SetTextNode(Node* node, string font, float size, ofColor color, string text);

		Behavior* CreateBehavior(spt<BehaviorEnt> behEntity);

		/**
		* Loads node from XML
		*/
		Node* LoadNodeFromXml(spt<ofxXml> xml, Node* parent, Scene* scene, Settings& settings);

		/**
		* Loads text node from XML
		*/
		void LoadTextFromXml(spt<ofxXml> xml, Node* node, Node* parent);

		/**
		* Loads behavior from XML
		*/
		void LoadBehaviorFromXml(spt<ofxXml> xml, Node* node);

		/**
		* Loads shape from xml
		*/
		void LoadShapeFromXml(spt<ofxXml> xml, Node* node, Scene* scene);
	};

}// namespace