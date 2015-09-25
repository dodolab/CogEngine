#pragma once

#include "GBehavior.h"
#include "GNode.h"
#include "ofxSmartPointer.h"



/**
* Engine for rendering
*/
class MRenderer {

protected:
	map<int, vector<GNode*>> zIndexes;

public:

	void Init(){
		zIndexes = map<int, vector<GNode*>>();
	}

	void PushNode(GNode* node){
		EnTransform& tr = node->GetTransform();
		int zIndex = (int)(tr.localPos.z*tr.absScale.z + tr.absPos.z);

	
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

		for (auto it = zIndexes.begin(); it != zIndexes.end(); ++it){
			vector<GNode*>& arr = (*it).second;
			int index = (*it).first;
			cout << index << endl;

			for (auto it2 = arr.begin(); it2 != arr.end(); ++it2){
				GNode* node = (*it2);

				// load absolute matrix
				ofMatrix4x4 absM = node->GetTransform().GetAbsMatrix();
				ofLoadMatrix(absM);


				// render
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
				}
			}
		}
	}
	
protected:

	/**
	* Renders an image
	* @param owner owner node
	*/
	void RenderImage(GNode* owner){
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
		if (owner->HasAttr(Attrs::COLOR)){
			ofColor color = owner->GetAttr<ofColor>(Attrs::COLOR);
			ofSetColor(color);
		}else ofSetColor(0,0,0);
		spt<EnFontShape> mojo = owner->GetShape<spt<EnFontShape>>();
		spt<ofTrueTypeFont> font = mojo->GetFont();
		string text = owner->GetAttr<string>(Attrs::TEXT);
		font->drawString(text, -font->stringWidth(text)/2, -font->stringHeight(text)/2);
	}
};
