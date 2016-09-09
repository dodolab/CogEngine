#pragma once

#include "GBehavior.h"
#include "GNode.h"
#include "ofxSmartPointer.h"
#include "ofxSpriteSheetRenderer.h"



/**
* Engine for rendering
*/
class MRenderer {

protected:
	map<int, vector<GNode*>> zIndexes;
	ofxSpriteSheetRenderer* renderer;

public:
	MRenderer(){

	}

	void Init(){
		zIndexes = map<int, vector<GNode*>>();
		

		// BEGIN UPGRADE
		renderer = new ofxSpriteSheetRenderer(); //declare a new renderer with 1 layer, 10000 tiles per layer, default layer of 0, tile size of 32
		auto img = COGGet2DImage("images/blue.png");
		
	//	renderer->loadTexture("images/blue.png", 256, 256, GL_LINEAR);
		renderer->loadTexture(&img->getTextureReference(), "blue", 10000, true);

		// END UPGRADE
	}

	void ClearCounters(){
		zIndexes = map<int, vector<GNode*>>();
	}

	ofxSpriteSheetRenderer* GetSpriteSheetRenderer(){
		return renderer;
	}

	void PushNode(GNode* node){
		EnTransform& tr = node->GetTransform();
		int zIndex = (int)(tr.absPos.z);

	
		auto it = zIndexes.find(zIndex);
		if (it != zIndexes.end()){
			(*it).second.push_back(node);
		}
		else{
			vector<GNode*> arr = vector<GNode*>();
			arr.push_back(node);
			zIndexes[zIndex] = arr;
		}
	}



	void Render(){
		renderer->clearCounters("blue");
		renderer->setActualBuffer("blue");

		for (auto it = zIndexes.begin(); it != zIndexes.end(); ++it){
			
			vector<GNode*>& arr = (*it).second;
			
			for (auto it2 = arr.begin(); it2 != arr.end(); ++it2){
				GNode* node = (*it2);

				switch (node->GetShape()->GetRenderType()){
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

		renderer->draw();
	}
	
protected:

	/**
	* Renders an image
	* @param owner owner node
	*/
	void RenderImage(GNode* owner){
		// load absolute matrix
		ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
		ofLoadMatrix(absM);

		ofSetColor(0x000000ff);
		spt<EnImageShape> imgShp = static_cast<spt<EnImageShape>>(owner->GetShape());
		spt<ofImage> image = imgShp->GetImage();

		if (owner->HasAttr(Attrs::IMGBOUNDS)){
			ofRectangle bound = owner->GetAttr<ofRectangle>(Attrs::IMGBOUNDS);
			image->drawSubsection(-bound.width / 2, -bound.height / 2, bound.width, bound.height, bound.x, bound.y);
		}
		else{
			image->draw(-image->getWidth() / 2, -image->getHeight() / 2);
		}
	}

	/**
	* Renders a polygon
	* @param owner owner node
	*/
	void RenderPolygon(GNode* owner){
		// todo: draw polygon
	}

	/**
	* Renders a rectangle
	* @param owner owner node
	*/
	void RenderRectangle(GNode* owner){
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
	void RenderArc(GNode* owner){
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
	void RenderText(GNode* owner){
		// load absolute matrix
		ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
		ofLoadMatrix(absM);


		if (owner->HasAttr(Attrs::COLOR)){
			ofColor color = owner->GetAttr<ofColor>(Attrs::COLOR);
			ofSetColor(color);
		}else ofSetColor(0,0,0);
		spt<EnFontShape> mojo = owner->GetShape<spt<EnFontShape>>();
		spt<ofTrueTypeFont> font = mojo->GetFont();
		string text = owner->GetAttr<string>(Attrs::TEXT);
		font->drawString(text, -font->stringWidth(text)/2, -font->stringHeight(text)/2);
	}

	void RenderSprite(GNode* owner){
		// todo - dont use this method very often
		//renderer->setActualBuffer("blue");

		// BEGIN UPGRADE
		EnTransform& trans = owner->GetTransform();
		ofMatrix4x4 abs = owner->GetTransform().GetAbsMatrix();
		Tile tile = Tile();
		tile.height = 256;
		tile.offsetX = 0;
		tile.offsetY = 0;
		tile.posX = abs.getTranslation().x;
		tile.posY = abs.getTranslation().y;
		tile.posZ = trans.absPos.z;
		tile.rotation = trans.rotation / 360;
		tile.scaleX = trans.scale.x;
		tile.scaleY = trans.scale.y;
		tile.width = 256;

		renderer->addTile(tile);
	}
};
