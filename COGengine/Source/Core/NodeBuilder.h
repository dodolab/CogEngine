#pragma once

#include "ofxCogCommon.h"
#include "Button.h"
#include "HitEvent.h"
#include "Node.h"
#include "ResourceCache.h"
#include "TransformMath.h"
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

		void SetMultiSelectionNode(Node* node, string defaultImg, string selectImg, string selectionGroup);

		void SetSelectionNode(Node* node, string defaultImg, string selectImg);

		void SetTextNode(Node* node, string font, float size, ofColor color, string text);

		void SetSpriteNode(Scene* scene, Node* node, string layer, int row, int column);

		void SetSpriteNode(Scene* scene, Node* node, string layer, string spriteSet, int row, int column);

		void SetPlaneNode(Node* node, ofVec2f size, ofColor color, bool noFill);

		void SetBoundingBoxNode(Scene* scene, Node* node, ofColor color, float margin, bool renderable);

		spt<SpriteShape> CreateSpriteShape(Scene* scene, string layer, int row, int column);

		spt<SpriteShape> CreateSpriteShape(Scene* scene, string layer, string spriteSet, int row, int column);

		spt<Plane> CreatePlaneShape(ofVec2f size, ofColor color);

		Behavior* CreateBehavior(spt<BehaviorEnt> behEntity);

		Node* CreateNode(string name, Scene* scene);

		/**
		* Loads node from XML
		*/
		Node* LoadNodeFromXml(spt<ofxXml> xml, Node* parent, Scene* scene);

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