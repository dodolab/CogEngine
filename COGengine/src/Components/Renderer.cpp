#include "Renderer.h"
#include "ofGraphics.h"
#include "Node.h"
#include "Behavior.h"
#include "Scene.h"
#include "ofxTextLabel.h"

namespace Cog {

	void Renderer::OnInit() {
		zIndexImageBuffer = map<int, vector<Node*>>();
		zIndexSheetBuffer = map<int, vector<Node*>>();
		renderer = new SpriteSheetRenderer();
		rendererLayers = vector<string>();
	}


	void Renderer::AddTileLayer(spt<ofImage> img, string name, int bufferSize, int zindex) {
		renderer->LoadTexture(&img->getTexture(), name, bufferSize, zindex);
		rendererLayers.push_back(name);
	}

	void Renderer::RemoveTileLayer(string name) {
		auto layer = find(rendererLayers.begin(), rendererLayers.end(), name);
		if (layer != rendererLayers.end()) rendererLayers.erase(layer);

		renderer->ClearTexture(name);
	}

	void Renderer::ClearBuffers() {
		zIndexImageBuffer = map<int, vector<Node*>>();
		zIndexSheetBuffer = map<int, vector<Node*>>();
	}

	void Renderer::PushNode(Node* node) {

		auto renderType = node->GetMesh()->GetMeshType();
		auto& buffer = (renderType == MESH_SPRITE || renderType == MESH_MULTISPRITE)
			? zIndexSheetBuffer : zIndexImageBuffer;


		Trans& tr = node->GetTransform();
		// zIndex will be taken always from local position
		int zIndex = (int)(tr.localPos.z);

		auto it = buffer.find(zIndex);
		if (it != buffer.end()) {
			(*it).second.push_back(node);
		}
		else {
			buffer[zIndex].push_back(node);
		}

	}

	void Renderer::BeginRender() {
		// set projection and clear background with black color
		Vec2i virtualSize = CogGetVirtualScreenSize();
		ofSetupScreenOrtho((float)virtualSize.x, (float)virtualSize.y, -1000.0f, 1000.0f);
		ofBackground(0);
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

		COGMEASURE_BEGIN("RENDER");
		COGMEASURE_BEGIN("RENDER_LAYERS");

		if (rendererLayers.size() != 0) {

			// reset all layers
			for (auto it = rendererLayers.begin(); it != rendererLayers.end(); ++it) {
				renderer->ClearCounters((*it));
			}

			// draw sprites
			for (auto it = zIndexSheetBuffer.begin(); it != zIndexSheetBuffer.end(); ++it) {

				vector<Node*>& arr = (*it).second;

				for (auto it2 = arr.begin(); it2 != arr.end(); ++it2) {
					Node* node = (*it2);

					switch (node->GetMesh()->GetMeshType()) {
					case MESH_IMAGE:
					case MESH_RECTANGLE:
					case MESH_TEXT:
					case MESH_LABEL:
					case MESH_BOUNDING_BOX:
						throw IllegalOperationException("Trying to render non-sprite node with sprite sheet renderer!");
					case MESH_SPRITE:
						RenderSprite(node);
						break;
					case MESH_MULTISPRITE:
						RenderMultiSprite(node);
						break;
					}
				}
			}
	
			// call sprite sheet renderer at the very end
			ofLoadMatrix(ofMatrix4x4::newIdentityMatrix());
			ofFill();
			renderer->Draw();
		}
		
		COGMEASURE_END("RENDER_LAYERS");
		COGMEASURE_BEGIN("RENDER_IMAGES");

		// draw images, planes, texts and labels
		for (auto it = zIndexImageBuffer.begin(); it != zIndexImageBuffer.end(); ++it) {

			vector<Node*>& arr = (*it).second;

			for (auto it2 = arr.begin(); it2 != arr.end(); ++it2) {
				Node* node = (*it2);

				switch (node->GetMesh()->GetMeshType()) { 
				case MESH_IMAGE:
					RenderImage(node);
					break;
				case MESH_RECTANGLE:
					RenderRectangle(node);
					break;
				case MESH_TEXT:
					RenderText(node);
					break;
				case MESH_LABEL:
					RenderLabel(node);
					break;
				case MESH_BOUNDING_BOX:
					RenderBoundingBox(node);
					break;
				case MESH_SPRITE:
				case MESH_MULTISPRITE:
					throw IllegalOperationException("Trying to render sprite node with default renderer!");
				}
			}
		}

		COGMEASURE_END("RENDER_IMAGES");
		COGMEASURE_END("RENDER");
	}

	void Renderer::RenderImage(Node* owner) {
		// load absolute matrix
		ofMatrix4x4 absM = owner->GetTransform().CalcAbsMatrix();
		ofLoadMatrix(absM);

		ofSetColor(0x000000ff);
		spt<Image> imgShp = static_pointer_cast<Image>(owner->GetMesh());
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
		spt<Rectangle> rect = static_pointer_cast<Rectangle>(owner->GetMesh());

		if (rect->IsRenderable()) {
			// load absolute matrix
			ofMatrix4x4 absM = owner->GetTransform().CalcAbsMatrix();
			ofLoadMatrix(absM);

			ofSetColor(0x000000ff);


			ofColor color = rect->GetColor();
			ofSetColor(color);


			if (rect->IsNoFill()) {
				// render just border
				ofNoFill();
				ofSetLineWidth(1);
			}
			else {
				ofFill();
				ofSetLineWidth(0);
			}

			ofRect(0, 0, 0, rect->GetWidth(), rect->GetHeight());
		}
	}

	void Renderer::RenderText(Node* owner) {
		// load absolute matrix
		ofMatrix4x4 absM = owner->GetTransform().CalcAbsMatrix();
		ofLoadMatrix(absM);
		spt<Text> shape = owner->GetMesh<Text>();
		ofSetColor(shape->GetColor());

		spt<ofTrueTypeFont> font = shape->GetFont();

		font->drawString(shape->GetText(), 0, font->getLineHeight() / 2);
	}

	void Renderer::RenderSprite(Node* owner) {

		spt<SpriteMesh> shape = static_pointer_cast<SpriteMesh>(owner->GetMesh());
		Sprite& sprite = shape->GetSprite();
		Trans& trans = owner->GetTransform();
		renderer->SetActualBuffer(shape->GetLayerName());

		// fill tile with all data and send it to the sprite manager
		spriteTile.width = sprite.GetWidth();
		spriteTile.height = sprite.GetHeight();
		spriteTile.offsetX = sprite.GetPosX();
		spriteTile.offsetY = sprite.GetPosY();

		spriteTile.posX = trans.absPos.x +trans.absScale.x*spriteTile.width / 2.0f;  // [0,0] is topleft corner
		spriteTile.posY = trans.absPos.y +trans.absScale.y*spriteTile.height / 2.0f;
		spriteTile.posZ = trans.absPos.z;
		spriteTile.rotation = trans.rotation*DEG_TO_RAD;
		spriteTile.scaleX = trans.absScale.x;
		spriteTile.scaleY = trans.absScale.y;
		
		renderer->AddTile(spriteTile);
	}

	void Renderer::RenderMultiSprite(Node* owner) {

		COGMEASURE_BEGIN("RENDER_PREPARE_MULTISPRITE");

		// Multi-sprites are clear choice when drawing thousands of objects, because they all
		// will be rendered at once 

		spt<MultiSpriteMesh> shape = static_pointer_cast<MultiSpriteMesh>(owner->GetMesh());
		renderer->SetActualBuffer(shape->GetLayerName());
		
		auto& sprites = shape->GetSprites();

		for (auto& spr : sprites) {
			Sprite& sprite = spr->sprite;
			Trans& trans = spr->transform;

			trans.CalcAbsTransform(owner->GetTransform());

			spriteTile.width = sprite.GetWidth();
			spriteTile.height = sprite.GetHeight();
			spriteTile.offsetX = sprite.GetPosX();
			spriteTile.offsetY = sprite.GetPosY();

			spriteTile.posX = trans.absPos.x + trans.absScale.x*spriteTile.width / 2.0f;  // [0,0] is topleft corner
			spriteTile.posY = trans.absPos.y + trans.absScale.y*spriteTile.height / 2.0f;
			spriteTile.posZ = trans.absPos.z;
			spriteTile.rotation = trans.rotation*DEG_TO_RAD;
			spriteTile.scaleX = trans.absScale.x;
			spriteTile.scaleY = trans.absScale.y;
			
			renderer->AddTile(spriteTile);
		}

		COGMEASURE_END("RENDER_PREPARE_MULTISPRITE");
	}

	void Renderer::RenderLabel(Node* owner) {
		
		auto& trans = owner->GetTransform();

		// label doesn't depend on transform !
		ofLoadMatrix(ofMatrix4x4::newIdentityMatrix());

		spt<Label> shape = owner->GetMesh<Label>();
		ofSetColor(shape->GetColor());

		spt<ofTrueTypeFont> font = shape->GetFont();

		vector<string> textLines;
		ofRectangle textBounds = ofRectangle();
		// use absolute positions
		ofRectangle drawBounds = ofRectangle(trans.absPos.x, trans.absPos.y, shape->GetLabelWidth(), CogGetScreenHeight()-trans.absPos.y);
		ofxTextLabel::stringToLines(*font, shape->GetText(), drawBounds.width, textLines, textBounds);
		auto text = shape->GetText();

		// draw lines one by one and calculate offsets for each line
		float lineX, lineY;
		lineX = drawBounds.x;
		lineY = drawBounds.y;
		int counter = 0;

		int lineHeight = font->stringHeight("Ay"); 
		int linesToDraw = (drawBounds.height / (1.5f*lineHeight));
		int startingIndex = textLines.size() - linesToDraw;
		if (startingIndex < 0) startingIndex = 0;

		// draw only lines that should be drawn
		for (auto it = (textLines.begin() + startingIndex); it != textLines.end(); ++it) {
			if (counter++ == 0) {
				lineY += font->stringHeight("Ay");  // easiest way to get ascender height.
			}
			else {
				lineY += font->getLineHeight();
			}

			font->drawString(*it, lineX, lineY);
		}
	}

	void Renderer::RenderBoundingBox(Node* owner) {
		spt<BoundingBox> shape = static_pointer_cast<BoundingBox>(owner->GetMesh());
		
		if (shape->IsRenderable()) {
			// draw only when it is set as renderable

			auto bbox = shape->GetBoundingBox();
			ofLoadMatrix(ofMatrix4x4::newIdentityMatrix());

			ofSetColor(0x000000ff);

			ofColor color = shape->GetColor();
			shape->Recalc(owner);
			ofSetColor(color);
			ofFill();

			ofRect(bbox.x, bbox.y, 0, bbox.width, bbox.height);
		}
	}

}// namespace