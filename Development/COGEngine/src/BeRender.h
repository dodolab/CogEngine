#pragma once

#include "GBehavior.h"
#include "GNode.h"
#include "ofxSmartPointer.h"

/*! User input action enumerator */
enum class RenderType{
	IMAGE, RECTANGLE, POLYGON, ARC, TEXT
};

/**
* Behavior for rendering
*/
class BeRender : public GBehavior{
protected:
	// type of rendering
	RenderType _type;
public:

	/**
	* Creates a new behavior for rendering
	* @param type rendering type
	*/
	BeRender(RenderType type) : GBehavior(ElemType::VIEW){
		this->_type = type;
	}


	virtual void Update(const uint64 delta, const uint64 absolute){

		// load absolute matrix
		ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
		ofLoadMatrix(absM);

		// render
		switch (_type){
		case RenderType::ARC:
			RenderArc(owner);
			break;
		case RenderType::IMAGE:
			RenderImage(owner);
			break;
		case RenderType::POLYGON:
			RenderPolygon(owner);
			break;
		case RenderType::RECTANGLE:
			RenderRectangle(owner);
			break;
		case RenderType::TEXT:
			RenderText(owner);
		}

	}
	
	/**
	* Renders an image
	* @param owner owner node
	*/
	void RenderImage(GNode* owner){
		ofSetColor(0x000000ff);
		spt<ofImage> image = owner->GetAttr<spt<ofImage>>(Attrs::IMGSOURCE);
		image->draw(-image->getWidth()/2,-image->getHeight()/2);
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
		spt<ofTrueTypeFont> font = owner->GetAttr<spt<ofTrueTypeFont>>(Attrs::FONT);
		string text = owner->GetAttr<string>(Attrs::TEXT);
		font->drawString(text, -font->stringWidth(text)/2, -font->stringHeight(text)/2);
	}
};
