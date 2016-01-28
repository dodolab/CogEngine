#pragma once

#include "Node.h"
#include "Behavior.h"
#include "Renderer.h"
#include "Scene.h"

namespace Cog {

	void Renderer::Init() {
		zIndexes = map<int, vector<Node*>>();

	}

	void Renderer::RestartRenderer() {
		delete renderer;
		renderer = new SpriteSheetRenderer();
		rendererLayers = vector<string>();
	}

	void Renderer::AddTileLayer(spt<ofImage> img, string name, int bufferSize) {
		renderer->loadTexture(&img->getTexture(), name, bufferSize);
		rendererLayers.push_back(name);
	}

	void Renderer::ClearCounters() {
		zIndexes = map<int, vector<Node*>>();
	}

	void Renderer::PushNode(Node* node) {
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

	void Renderer::BeginRender() {
		Vec2i virtualSize = CogGetVirtualScreenSize();
		ofSetupScreenOrtho((float)virtualSize.x, (float)virtualSize.y, -1000.0f, 1000.0f);
		ofBackground(0, 0, 0);
	}

	void Renderer::EndRender() {
		ofVec2f screenSize = CogGetScreenSize();
		ofVec2f virtualSize = CogGetVirtualScreenSize();

		ofLoadMatrix(ofMatrix4x4::newIdentityMatrix());
		ofSetColor(0);

		// draw black rectangles that will cover oversized viewport
		if (virtualSize.x != screenSize.x) {
			// draw left and right
			ofViewport(0.0f, 0.0f, (screenSize.x - virtualSize.x) / 2, virtualSize.y);
			ofDrawRectangle(0.0f, 0.0f, screenSize.x, screenSize.y);
			ofViewport(screenSize.x - (screenSize.x - virtualSize.x) / 2, 0.0f, (screenSize.x - virtualSize.x) / 2, virtualSize.y);
			ofDrawRectangle(0.0f, 0.0f, screenSize.x, screenSize.y);

			// back to actual viewport
			ofViewport((screenSize.x - virtualSize.x) / 2, 0.0f, virtualSize.x, virtualSize.y);
		}
		else if (virtualSize.y != screenSize.y) {
			// draw top and bottom
			ofViewport(0.0f, 0.0f,virtualSize.x,(screenSize.y - virtualSize.y) / 2);
			ofDrawRectangle(0.0f, 0.0f, screenSize.x, screenSize.y);
			ofViewport(0.0f, screenSize.y-(screenSize.y-virtualSize.y)/2, virtualSize.x, (screenSize.y - virtualSize.y) / 2);
			ofDrawRectangle(0.0f, 0.0f, screenSize.x, screenSize.y);
			// back to actual viewport
			ofViewport((screenSize.x - virtualSize.x) / 2, 0.0f, virtualSize.x, virtualSize.y);
		}
		
	}

	void Renderer::InitViewPort(Scene* sceneNode) {
		Vec2i screenSize = CogGetScreenSize();
		Vec2i virtualSize = CogGetVirtualScreenSize();

		// handle custom aspect ratio
		if (virtualSize.x != screenSize.x) {
			ofViewport(sceneNode->GetViewPortOffset().x + (screenSize.x - virtualSize.x) / 2, sceneNode->GetViewPortOffset().y, (float)virtualSize.x, (float)virtualSize.y);
		}
		else if (virtualSize.y != screenSize.y) {
			ofViewport(sceneNode->GetViewPortOffset().x, sceneNode->GetViewPortOffset().y + (screenSize.y - virtualSize.y) / 2, (float)virtualSize.x, (float)virtualSize.y);
		}
		else {
			// back to actual viewport
			ofViewport(sceneNode->GetViewPortOffset().x, sceneNode->GetViewPortOffset().y, (float)virtualSize.x, (float)virtualSize.y);
		}
	}

	void Renderer::Render() {
		
		for (auto it = rendererLayers.begin(); it != rendererLayers.end(); ++it) {
			renderer->clearCounters((*it));
		}

		for (auto it = zIndexes.begin(); it != zIndexes.end(); ++it) {

			vector<Node*>& arr = (*it).second;

			for (auto it2 = arr.begin(); it2 != arr.end(); ++it2) {
				Node* node = (*it2);

				switch (node->GetShape()->GetRenderType()) {
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
					break;
				case RenderType::MULTISPRITE:
					RenderMultiSprite(node);
					break;
				}

			}
		}
		//ofLoadMatrix(ofMatrix4x4::newIdentityMatrix());
		//renderer->draw();
	}

	void Renderer::RenderImage(Node* owner) {
		auto trans = owner->GetTransform();
		// load absolute matrix
		ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
		ofLoadMatrix(absM);

		ofSetColor(0x000000ff);
		spt<Image> imgShp = static_cast<spt<Image>>(owner->GetShape());
		spt<ofImage> image = imgShp->GetImage();

		if (owner->HasAttr(ATTR_IMGBOUNDS)) {
			ofRectangle bound = owner->GetAttr<ofRectangle>(ATTR_IMGBOUNDS);
			image->drawSubsection(0, 0, bound.width, bound.height, bound.x, bound.y);
		}
		else {
			image->draw(0, 0);
		}
	}

	void Renderer::RenderRectangle(Node* owner) {
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

	void Renderer::RenderText(Node* owner) {
		// load absolute matrix
		ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
		ofLoadMatrix(absM);
		spt<Text> shape = owner->GetShape<spt<Text>>();
		ofSetColor(shape->GetColor());

		spt<ofTrueTypeFont> font = shape->GetFont();

		// don't touch it! It works :-)
		font->drawString(shape->GetText(), 0, font->getLineHeight() / 2);
	}

	void Renderer::RenderSprite(Node* owner) {

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

	void Renderer::RenderMultiSprite(Node* owner) {


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

			drawingTile.posX = trans.absPos.x + trans.absScale.x*drawingTile.width / 2.0f;  // [0,0] is topleft corner
			drawingTile.posY = trans.absPos.y + trans.absScale.y*drawingTile.height / 2.0f;
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

}// namespace