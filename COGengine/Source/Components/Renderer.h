#pragma once

#include "Behavior.h"
#include "Node.h"

namespace Cog {


	/**
	* Engine for rendering
	*/
	class Renderer : public  Component {

		COMPONENT(Renderer)

	protected:
		map<int, vector<Node*>> zIndexes;
		SpriteSheetRenderer* renderer;
		Tile tempTile;

	public:
		Renderer() {
			tempTile = Tile();
		}

		void Init() {
			zIndexes = map<int, vector<Node*>>();


			// BEGIN UPGRADE
			renderer = new SpriteSheetRenderer(); //declare a new renderer with 1 layer, 10000 tiles per layer, default layer of 0, tile size of 32
			auto img = CogGet2DImage("images/blue.png");

			//	renderer->loadTexture("images/blue.png", 256, 256, GL_LINEAR);
			renderer->loadTexture(&img->getTextureReference(), "blue", 10000, true);

			// END UPGRADE
		}

		void ClearCounters() {
			zIndexes = map<int, vector<Node*>>();
		}

		SpriteSheetRenderer* GetSpriteSheetRenderer() {
			return renderer;
		}

		void PushNode(Node* node) {
			Trans& tr = node->GetTransform();
			int zIndex = (int)(tr.absPos.z);


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



		void Render() {
			renderer->clearCounters("blue");

			for (auto it = zIndexes.begin(); it != zIndexes.end(); ++it) {

				vector<Node*>& arr = (*it).second;

				for (auto it2 = arr.begin(); it2 != arr.end(); ++it2) {
					Node* node = (*it2);

					switch (node->GetShape()->GetRenderType()) {
					case RenderType::ARC:
						RenderArc(node);
						break;
					case RenderType::IMAGE:
						RenderImage(node);
						break;
					case RenderType::POLYGON:
						RenderPolygon(node);
						break;
					case RenderType::RECTANGLE:
						RenderRectangle(node);
						break;
					case RenderType::TEXT:
						RenderText(node);
						break;
					case RenderType::SPRITE:
						RenderSprite(node);
					}
				}
			}

			ofLoadMatrix(ofMatrix4x4::newIdentityMatrix());
			renderer->draw();
		}

	protected:

		/**
		* Renders an image
		* @param owner owner node
		*/
		void RenderImage(Node* owner) {
			// load absolute matrix
			ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
			ofLoadMatrix(absM);

			ofSetColor(0x000000ff);
			spt<Image> imgShp = static_cast<spt<Image>>(owner->GetShape());
			spt<ofImage> image = imgShp->GetImage();

			if (owner->HasAttr(ATTR_IMGBOUNDS)) {
				ofRectangle bound = owner->GetAttr<ofRectangle>(ATTR_IMGBOUNDS);
				image->drawSubsection(-bound.width / 2, -bound.height / 2, bound.width, bound.height, bound.x, bound.y);
			}
			else {
				image->draw(-image->getWidth() / 2, -image->getHeight() / 2);
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


			ofVec3f size = owner->GetAttr<ofVec3f>(ATTR_SIZE);
			ofColor color = owner->GetAttr<ofColor>(ATTR_COLOR);
			ofSetColor(color);
			ofFill();
			ofRect(-size.x / 2, -size.y / 2, size.x, size.y);
			ofEndShape();
		}

		/**
		* Renders an arc
		* @param owner owner node
		*/
		void RenderArc(Node* owner) {
			ofVec3f size = owner->GetAttr<ofVec3f>(ATTR_SIZE);
			ofColor color = owner->GetAttr<ofColor>(ATTR_COLOR);
			ofSetColor(color);

			// todo: draw arc
			//Iw2DFillArc(ofVec3f(0, 0), size, 0, PI * 2, 60);
		}

		/**
		* Renders a text
		* @param owner owner node
		*/
		void RenderText(Node* owner) {
			// load absolute matrix
			ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
			ofLoadMatrix(absM);


			if (owner->HasAttr(ATTR_COLOR)) {
				ofColor color = owner->GetAttr<ofColor>(ATTR_COLOR);
				ofSetColor(color);
			}
			else ofSetColor(0, 0, 0);
			spt<Font> mojo = owner->GetShape<spt<Font>>();
			spt<ofTrueTypeFont> font = mojo->GetFont();
			string text = owner->GetAttr<string>(ATTR_TEXT);
			font->drawString(text, -font->stringWidth(text) / 2, -font->stringHeight(text) / 2);
		}

		void RenderSprite(Node* owner) {
			// todo - dont use this method very often
			//renderer->setActualBuffer("blue");

			// BEGIN UPGRADE
			Trans& trans = owner->GetTransform();
			ofMatrix4x4 abs = owner->GetTransform().GetAbsMatrix();
			spt<SpriteShape> shape = static_cast<spt<SpriteShape>>(owner->GetShape());

			spt<Sprite>& sprite = shape->GetSprite();

			tempTile.height = sprite->GetHeight();
			tempTile.offsetX = sprite->GetPosX();
			tempTile.offsetY = sprite->GetPosY();
			tempTile.posX = abs.getTranslation().x;
			tempTile.posY = abs.getTranslation().y;
			tempTile.posZ = trans.absPos.z;
			tempTile.rotation = trans.rotation / 360;
			tempTile.scaleX = trans.scale.x;
			tempTile.scaleY = trans.scale.y;
			tempTile.width = sprite->GetWidth();


			renderer->addTile(tempTile);
		}
	};


}// namespace