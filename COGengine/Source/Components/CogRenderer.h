#pragma once

#include "CogBehavior.h"
#include "CogNode.h"

namespace Cog {


	/**
	* Engine for rendering
	*/
	class CogRenderer {

	protected:
		map<int, vector<CogNode*>> zIndexes;
		SpriteSheetRenderer* renderer;
		Tile tempTile;

	public:
		CogRenderer() {
			tempTile = Tile();
		}

		void Init() {
			zIndexes = map<int, vector<CogNode*>>();


			// BEGIN UPGRADE
			renderer = new SpriteSheetRenderer(); //declare a new renderer with 1 layer, 10000 tiles per layer, default layer of 0, tile size of 32
			auto img = CogGet2DImage("images/blue.png");

			//	renderer->loadTexture("images/blue.png", 256, 256, GL_LINEAR);
			renderer->loadTexture(&img->getTextureReference(), "blue", 10000, true);

			// END UPGRADE
		}

		void ClearCounters() {
			zIndexes = map<int, vector<CogNode*>>();
		}

		SpriteSheetRenderer* GetSpriteSheetRenderer() {
			return renderer;
		}

		void PushNode(CogNode* node) {
			CogTrans& tr = node->GetTransform();
			int zIndex = (int)(tr.absPos.z);


			auto it = zIndexes.find(zIndex);
			if (it != zIndexes.end()) {
				(*it).second.push_back(node);
			}
			else {
				vector<CogNode*> arr = vector<CogNode*>();
				arr.push_back(node);
				zIndexes[zIndex] = arr;
			}
		}



		void Render() {
			renderer->clearCounters("blue");

			for (auto it = zIndexes.begin(); it != zIndexes.end(); ++it) {

				vector<CogNode*>& arr = (*it).second;

				for (auto it2 = arr.begin(); it2 != arr.end(); ++it2) {
					CogNode* node = (*it2);

					switch (node->GetShape()->GetRenderType()) {
					case CogRenderType::ARC:
						RenderArc(node);
						break;
					case CogRenderType::IMAGE:
						RenderImage(node);
						break;
					case CogRenderType::POLYGON:
						RenderPolygon(node);
						break;
					case CogRenderType::RECTANGLE:
						RenderRectangle(node);
						break;
					case CogRenderType::TEXT:
						RenderText(node);
						break;
					case CogRenderType::SPRITE:
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
		void RenderImage(CogNode* owner) {
			// load absolute matrix
			ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
			ofLoadMatrix(absM);

			ofSetColor(0x000000ff);
			spt<CogImage> imgShp = static_cast<spt<CogImage>>(owner->GetShape());
			spt<ofImage> image = imgShp->GetImage();

			if (owner->HasAttr(Attrs::IMGBOUNDS)) {
				ofRectangle bound = owner->GetAttr<ofRectangle>(Attrs::IMGBOUNDS);
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
		void RenderPolygon(CogNode* owner) {
			// todo: draw polygon
		}

		/**
		* Renders a rectangle
		* @param owner owner node
		*/
		void RenderRectangle(CogNode* owner) {
			// load absolute matrix
			ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
			ofLoadMatrix(absM);


			ofVec3f size = owner->GetAttr<ofVec3f>(Attrs::SIZE);
			ofColor color = owner->GetAttr<ofColor>(Attrs::COLOR);
			ofSetColor(color);
			ofFill();
			ofRect(-size.x / 2, -size.y / 2, size.x, size.y);
			ofEndShape();
		}

		/**
		* Renders an arc
		* @param owner owner node
		*/
		void RenderArc(CogNode* owner) {
			ofVec3f size = owner->GetAttr<ofVec3f>(Attrs::SIZE);
			ofColor color = owner->GetAttr<ofColor>(Attrs::COLOR);
			ofSetColor(color);

			// todo: draw arc
			//Iw2DFillArc(ofVec3f(0, 0), size, 0, PI * 2, 60);
		}

		/**
		* Renders a text
		* @param owner owner node
		*/
		void RenderText(CogNode* owner) {
			// load absolute matrix
			ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
			ofLoadMatrix(absM);


			if (owner->HasAttr(Attrs::COLOR)) {
				ofColor color = owner->GetAttr<ofColor>(Attrs::COLOR);
				ofSetColor(color);
			}
			else ofSetColor(0, 0, 0);
			spt<CogFont> mojo = owner->GetShape<spt<CogFont>>();
			spt<ofTrueTypeFont> font = mojo->GetFont();
			string text = owner->GetAttr<string>(Attrs::TEXT);
			font->drawString(text, -font->stringWidth(text) / 2, -font->stringHeight(text) / 2);
		}

		void RenderSprite(CogNode* owner) {
			// todo - dont use this method very often
			//renderer->setActualBuffer("blue");

			// BEGIN UPGRADE
			CogTrans& trans = owner->GetTransform();
			ofMatrix4x4 abs = owner->GetTransform().GetAbsMatrix();
			spt<CogSpriteShape> shape = static_cast<spt<CogSpriteShape>>(owner->GetShape());

			spt<CogSprite>& sprite = shape->GetSprite();

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


}