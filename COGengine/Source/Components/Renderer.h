#pragma once

#include "Component.h"
#include "SpriteSheetRenderer.h"

namespace Cog {

	class Behavior;
	class Node;

	/**
	* Engine for rendering
	*/
	class Renderer : public  Component {

		OBJECT(Renderer)

	protected:
		// buffer for z-indices and nodes with those indices
		map<int, vector<Node*>> zIndexImageBuffer;
		// buffer for nodes that will be rendered using the SpriteSheetRenderer
		map<int, vector<Node*>> zIndexSheetBuffer;

		SpriteSheetRenderer* renderer = nullptr;
		Tile drawingTile;
		vector<string> rendererLayers = vector<string>();

	public:
		Renderer() {
			drawingTile  = Tile();
		}

		void Init();

		void AddTileLayer(spt<ofImage> img, string name, int bufferSize, int zindex);

		void RemoveTileLayer(string name);

		void ClearCounters();

		SpriteSheetRenderer* GetSpriteSheetRenderer() {
			return renderer;
		}

		void PushNode(Node* node);

		void BeginRender();

		void EndRender();

		void InitViewPort(Scene* sceneNode);

		void Render();

	protected:

		/**
		* Renders an image
		* @param owner owner node
		*/
		void RenderImage(Node* owner);

		/**
		* Renders a polygon
		* @param owner owner node
		*/
		void RenderPolygon(Node* owner) {
			// todo: draw polygon
		}

		/**
		* Renders a rectangle
		* @param owner owner node
		*/
		void RenderPlane(Node* owner);

		/**
		* Renders a text
		* @param owner owner node
		*/
		void RenderText(Node* owner);

		void RenderSprite(Node* owner);

		void RenderMultiSprite(Node* owner);

		void RenderLabel(Node* owner);

		void RenderBoundingBox(Node* owner);
	};


}// namespace