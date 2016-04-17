#include "Node.h"
#include "Behavior.h"
#include "Renderer.h"
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

	void Renderer::ClearCounters() {
		zIndexImageBuffer = map<int, vector<Node*>>();
		zIndexSheetBuffer = map<int, vector<Node*>>();
	}

	void Renderer::PushNode(Node* node) {

		auto renderType = node->GetMesh()->GetMeshType();
		auto& buffer = (renderType == MeshType::SPRITE || renderType == MeshType::MULTISPRITE)
			? zIndexSheetBuffer : zIndexImageBuffer;


		Trans& tr = node->GetTransform();
		// zIndex will be taken always from local position
		int zIndex = (int)(tr.localPos.z);

		auto it = buffer.find(zIndex);
		if (it != buffer.end()) {
			(*it).second.push_back(node);
		}
		else {
			vector<Node*> arr;
			arr.push_back(node);
			buffer[zIndex] = arr;
		}

	}

	void Renderer::BeginRender() {
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

			for (auto it = rendererLayers.begin(); it != rendererLayers.end(); ++it) {
				renderer->ClearCounters((*it));
			}

			for (auto it = zIndexSheetBuffer.begin(); it != zIndexSheetBuffer.end(); ++it) {

				vector<Node*>& arr = (*it).second;

				for (auto it2 = arr.begin(); it2 != arr.end(); ++it2) {
					Node* node = (*it2);

					switch (node->GetMesh()->GetMeshType()) {
					case MeshType::IMAGE:
					case MeshType::RECTANGLE:
					case MeshType::PLANE:
					case MeshType::TEXT:
					case MeshType::LABEL:
					case MeshType::BOUNDING_BOX:
						throw IllegalOperationException("Trying to render non-sprite node by sprite sheet renderer!");
					case MeshType::SPRITE:
						RenderSprite(node);
						break;
					case MeshType::MULTISPRITE:
						RenderMultiSprite(node);
						break;
					}
				}
			}
	
			ofLoadMatrix(ofMatrix4x4::newIdentityMatrix());
			ofFill();
			renderer->Draw();
		}
		
		COGMEASURE_END("RENDER_LAYERS");
		COGMEASURE_BEGIN("RENDER_IMAGES");

		for (auto it = zIndexImageBuffer.begin(); it != zIndexImageBuffer.end(); ++it) {

			vector<Node*>& arr = (*it).second;

			for (auto it2 = arr.begin(); it2 != arr.end(); ++it2) {
				Node* node = (*it2);

				switch (node->GetMesh()->GetMeshType()) { 
				case MeshType::IMAGE:
					RenderImage(node);
					break;
				case MeshType::PLANE:
					RenderPlane(node);
					break;
				case MeshType::TEXT:
					RenderText(node);
					break;
				case MeshType::LABEL:
					RenderLabel(node);
					break;
				case MeshType::BOUNDING_BOX:
					RenderBoundingBox(node);
					break;
				case MeshType::SPRITE:
				case MeshType::MULTISPRITE:
					throw IllegalOperationException("Trying to render sprite node by default renderer!");
				}
			}
		}

		COGMEASURE_END("RENDER_IMAGES");
		COGMEASURE_END("RENDER");
	}

	void Renderer::RenderImage(Node* owner) {
		auto trans = owner->GetTransform();
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

	void Renderer::RenderPlane(Node* owner) {
		auto trans = owner->GetTransform();
		// load absolute matrix
		ofMatrix4x4 absM = owner->GetTransform().CalcAbsMatrix();
		ofLoadMatrix(absM);

		ofSetColor(0x000000ff);
		spt<Plane> rect = static_pointer_cast<Plane>(owner->GetMesh());

		ofColor color = rect->GetColor();
		ofSetColor(color);
		
		if (rect->IsNoFill()) {
			ofNoFill();
			ofSetLineWidth(1);
		}
		else {
			ofFill();
			ofSetLineWidth(0);
		}
		
		ofRect(0, 0, 0, rect->GetWidth(), rect->GetHeight());

	}

	void Renderer::RenderText(Node* owner) {
		// load absolute matrix
		ofMatrix4x4 absM = owner->GetTransform().CalcAbsMatrix();
		ofLoadMatrix(absM);
		spt<Text> shape = owner->GetMesh<Text>();
		ofSetColor(shape->GetColor());

		spt<ofTrueTypeFont> font = shape->GetFont();

		// don't touch it! It works :-)
		font->drawString(shape->GetText(), 0, font->getLineHeight() / 2);
	}

	void Renderer::RenderSprite(Node* owner) {

		spt<SpriteMesh> shape = static_pointer_cast<SpriteMesh>(owner->GetMesh());
		Sprite& sprite = shape->GetSprite();
		Trans& trans = owner->GetTransform();
		renderer->SetActualBuffer(shape->GetLayerName());

		drawingTile.width = sprite.GetWidth();
		drawingTile.height = sprite.GetHeight();
		drawingTile.offsetX = sprite.GetPosX();
		drawingTile.offsetY = sprite.GetPosY();

		drawingTile.posX = trans.absPos.x +trans.absScale.x*drawingTile.width / 2.0f;  // [0,0] is topleft corner
		drawingTile.posY = trans.absPos.y +trans.absScale.y*drawingTile.height / 2.0f;
		drawingTile.posZ = trans.absPos.z;
		drawingTile.rotation = trans.rotation*DEG_TO_RAD;
		drawingTile.scaleX = trans.absScale.x;
		drawingTile.scaleY = trans.absScale.y;
		
		renderer->AddTile(drawingTile);
	}

	void Renderer::RenderMultiSprite(Node* owner) {

		COGMEASURE_BEGIN("RENDER_PREPARE_MULTISPRITE");

		spt<MultiSpriteMesh> shape = static_pointer_cast<MultiSpriteMesh>(owner->GetMesh());
		renderer->SetActualBuffer(shape->GetLayerName());
		auto& sprites = shape->GetSprites();

		for (auto& spr : sprites) {
			Sprite& sprite = spr->sprite;
			Trans& trans = spr->transform;

			trans.CalcAbsTransform(owner->GetTransform());

			drawingTile.width = sprite.GetWidth();
			drawingTile.height = sprite.GetHeight();
			drawingTile.offsetX = sprite.GetPosX();
			drawingTile.offsetY = sprite.GetPosY();

			drawingTile.posX = trans.absPos.x + trans.absScale.x*drawingTile.width / 2.0f;  // [0,0] is topleft corner
			drawingTile.posY = trans.absPos.y + trans.absScale.y*drawingTile.height / 2.0f;
			drawingTile.posZ = trans.absPos.z;
			drawingTile.rotation = trans.rotation*DEG_TO_RAD;
			drawingTile.scaleX = trans.absScale.x;
			drawingTile.scaleY = trans.absScale.y;
			
			renderer->AddTile(drawingTile);
		}

		COGMEASURE_END("RENDER_PREPARE_MULTISPRITE");
	}

	void Renderer::RenderLabel(Node* owner) {
		// load absolute matrix
		auto& trans = owner->GetTransform();
		ofLoadMatrix(ofMatrix4x4::newIdentityMatrix());

		spt<Label> shape = owner->GetMesh<Label>();
		ofSetColor(shape->GetColor());

		spt<ofTrueTypeFont> font = shape->GetFont();

		vector<string> textLines;
		ofRectangle textBounds = ofRectangle();
		ofRectangle drawBounds = ofRectangle(trans.absPos.x, trans.absPos.y, shape->GetLabelWidth(), CogGetScreenHeight()-trans.absPos.y);
		ofxTextLabel::stringToLines(*font, shape->GetText(), drawBounds.width, textLines, textBounds);
		auto text = shape->GetText();


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
				lineY += font->stringHeight("Ay");  // Easiest way to get ascender height.
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