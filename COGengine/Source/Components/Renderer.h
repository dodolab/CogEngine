#pragma once

#include "Component.h"
#include "SpriteSheetRenderer.h"

namespace Cog {

	class Behavior;
	class Node;

	/**
	* Rendering engine
	*/
	class Renderer : public  Component {

	protected:
		// buffer for z-indices of nodes that will be rendered a standard way
		map<int, vector<Node*>> zIndexImageBuffer;
		// buffer for z-indices of nodes that will be rendered using the SpriteSheetRenderer
		map<int, vector<Node*>> zIndexSheetBuffer;
		
		SpriteSheetRenderer* renderer = nullptr;
		// tile regularly filled with data and sent to sprite sheet renderer
		SpriteTile spriteTile;
		// layers used in sprite sheet renderer
		vector<string> rendererLayers;

	public:
		Renderer() {
		}

		void OnInit();

		/**
		* Adds a new tile layer for sprite sheet renderer
		* @param img layer image
		* @param name name of the layer
		* @param bufferSize maximum number of sprites in the layer
		* @param zindex absolute z-index of the layer
		*/
		void AddTileLayer(spt<ofImage> img, string name, int bufferSize, int zindex);

		/**
		* Removes tile layer by name
		*/
		void RemoveTileLayer(string name);

		/**
		* Clears all sprite buffers 
		*/
		void ClearBuffers();

		SpriteSheetRenderer* GetSpriteSheetRenderer() {
			return renderer;
		}

		/**
		* Pushes node that will be later rendered
		* together will all other nodes
		*/
		void PushNode(Node* node);

		/**
		* Begins the rendering procedure
		*/
		void BeginRender();

		/**
		* Ends the rendering procedure
		*/
		void EndRender();

		/**
		* Sets rendering viewport according to the viewport configured
		* in scene. Important especially when aspect ratio is customized and
		* therefore the rendered screen has different size
		*/
		void InitViewPort(Scene* sceneNode);

		/**
		* Renders everything
		*/
		void Render();

	protected:

		/**
		* Renders an image
		* @param owner node with image shape
		*/
		void RenderImage(Node* owner);

		/**
		* Renders a rectangle
		* @param owner node with rectangle shape
		*/
		void RenderRectangle(Node* owner);

		/**
		* Renders a text
		* @param owner node with text shape
		*/
		void RenderText(Node* owner);

		/**
		* Renders a sprite
		* @param owner node with sprite shape
		*/
		void RenderSprite(Node* owner);

		/**
		* Renders a multisprite (collection of sprites)
		* @param owner node with multisprite shape
		*/
		void RenderMultiSprite(Node* owner);

		/**
		* Renders a label
		* @param owner node with label shape
		*/
		void RenderLabel(Node* owner);

		/**
		* Renders a bounding box
		* @param owner node with bounding box shape
		*/
		void RenderBoundingBox(Node* owner);

		virtual void Update(const uint64 delta, const uint64 absolute) {
		}
	};


}// namespace