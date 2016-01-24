#pragma once

#include "Behavior.h"
#include "Node.h"
#include "Component.h"

namespace Cog {


	/**
	* Engine for rendering
	*/
	class Renderer : public  Component {

		OBJECT(Renderer)

	protected:
		map<int, vector<Node*>> zIndexes;
		SpriteSheetRenderer* renderer = nullptr;
		Tile drawingTile;
		vector<string> rendererLayers = vector<string>();

	public:
		Renderer() {
			drawingTile  = Tile();
		}

		void Init() {
			zIndexes = map<int, vector<Node*>>();

		}

		void RestartRenderer() {
			delete renderer;
			renderer = new SpriteSheetRenderer();
			rendererLayers = vector<string>();
		}

		void AddTileLayer(spt<ofImage> img, string name, int bufferSize) {
			renderer->loadTexture(&img->getTextureReference(), name, bufferSize);
			rendererLayers.push_back(name);
		}

		void ClearCounters() {
			zIndexes = map<int, vector<Node*>>();
		}

		SpriteSheetRenderer* GetSpriteSheetRenderer() {
			return renderer;
		}

		void PushNode(Node* node) {
			Trans& tr = node->GetTransform();
			// zIndex will be taken always from local position
			int zIndex = (int)(tr.localPos.z);

			auto it = zIndexes.find(zIndex);
			if (it != zIndexes.end()) {
				(*it).second.push_back(node);
			}
			else {
				vector<Node*> arr = vector<Node*>();
				arr.push_back(node);
				zIndexes[zIndex] = arr;
			}
		}

		void BeginRender();

		void EndRender();

		void InitViewPort(Scene* sceneNode);

		void Render();

	protected:

		/**
		* Renders an image
		* @param owner owner node
		*/
		void RenderImage(Node* owner) {
			auto trans = owner->GetTransform();
			// load absolute matrix
			ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
			ofLoadMatrix(absM);

			ofSetColor(0x000000ff);
			spt<Image> imgShp = static_cast<spt<Image>>(owner->GetShape());
			spt<ofImage> image = imgShp->GetImage();

			if (owner->HasAttr(ATTR_IMGBOUNDS)) {
				ofRectangle bound = owner->GetAttr<ofRectangle>(ATTR_IMGBOUNDS);
				image->drawSubsection(0,0, bound.width, bound.height, bound.x, bound.y);
			}
			else {
				image->draw(0, 0);
			}
		}

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
		void RenderRectangle(Node* owner) {
			// load absolute matrix
			ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
			ofLoadMatrix(absM);

/*
			ofVec3f size = owner->GetAttr<ofVec3f>(ATTR_SIZE);
			ofColor color = owner->GetAttr<ofColor>(ATTR_COLOR);
			ofSetColor(color);
			ofFill();
			ofRect(-size.x / 2, -size.y / 2, size.x, size.y);*/
			ofEndShape();
		}

		/**
		* Renders a text
		* @param owner owner node
		*/
		void RenderText(Node* owner) {
			// load absolute matrix
			ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
			ofLoadMatrix(absM);
			spt<Text> shape = owner->GetShape<spt<Text>>();
			ofSetColor(shape->GetColor());
			
			spt<ofTrueTypeFont> font = shape->GetFont();
		
			// don't touch it! It works :-)
			font->drawString(shape->GetText(), 0, font->getLineHeight()/2);
		}

		void RenderSprite(Node* owner) {
			
			// BEGIN UPGRADE
			Trans& trans = owner->GetTransform();
			ofMatrix4x4 abs = owner->GetTransform().GetAbsMatrix();
			spt<SpriteShape> shape = static_cast<spt<SpriteShape>>(owner->GetShape());


			spt<Sprite>& sprite = shape->GetSprite();

			drawingTile.height = sprite->GetHeight();
			drawingTile.offsetX = sprite->GetPosX();
			drawingTile.offsetY = sprite->GetPosY();
			drawingTile.posX = abs.getTranslation().x;
			drawingTile.posY = abs.getTranslation().y;
			drawingTile.posZ = trans.absPos.z;
			drawingTile.rotation = trans.rotation / 360;
			drawingTile.scaleX = trans.scale.x;
			drawingTile.scaleY = trans.scale.y;
			drawingTile.width = sprite->GetWidth();

			renderer->addTile(drawingTile);
		}

		void RenderMultiSprite(Node* owner) {
			

			spt<SpritesShape> shape = static_cast<spt<SpritesShape>>(owner->GetShape());
			renderer->setActualBuffer(shape->GetName());
			vector<spt<MSpriteCrate>> sprites = shape->GetSprites();

			for (auto it = sprites.begin(); it != sprites.end(); ++it) {
				spt<MSpriteCrate> crate = (*it);
				spt<Sprite> sprite = crate->sprite;
				Trans trans = crate->transform;
				trans.CalcAbsTransform(owner->GetTransform());

				drawingTile.height = sprite->GetHeight();
				drawingTile.offsetX = sprite->GetPosX();
				drawingTile.offsetY = sprite->GetPosY();

				drawingTile.posX = trans.absPos.x + trans.absScale.x*drawingTile.width/2;  // [0,0] is topleft corner
				drawingTile.posY = trans.absPos.y + trans.absScale.y*drawingTile.height/2;
				drawingTile.posZ = trans.absPos.z;
				drawingTile.rotation = trans.rotation / 360;
				drawingTile.scaleX = trans.absScale.x;
				drawingTile.scaleY = trans.absScale.y;
				drawingTile.width = sprite->GetWidth();

				renderer->addTile(drawingTile);
			}

			ofLoadMatrix(ofMatrix4x4::newIdentityMatrix());
			renderer->draw();
		}
	};


}// namespace