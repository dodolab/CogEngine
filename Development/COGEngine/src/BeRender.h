#pragma once

#include "GBehavior.h"
#include "GNode.h"
#include "SmartPointer.h"
#include "EnPolygon.h"

/*! User input action enumerator */
enum class RenderType{
	IMAGE, RECTANGLE, POLYGON, ARC, TEXT
};

/**
* Behavior for rendering
*/
class BeRender : public GBehavior{
private:
	RenderType _type;
public:
	BeRender(RenderType type) : GBehavior(ElemType::VIEW){
		this->_type = type;
	}


	virtual void Update(const uint64 delta, const uint64 absolute){

		ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
		ofLoadMatrix(absM);
		int mojo = owner->GetSubType();
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
	
	void RenderImage(GNode* owner){
		ofSetColor(0x000000ff);
		spt<ofImage> image = owner->GetAttr<spt<ofImage>>(Attrs::IMGSOURCE);
		image->draw(-image->getWidth()/2,-image->getHeight()/2);
		/*spt<ofVboMesh> mesh = MEngine.resourceCtrl->GetMesh("images/blue.png");
		image->bind();
		mesh->draw();
		image->unbind();*/
	}

	void RenderPolygon(GNode* owner){
		spt<EnPolygon> poly = owner->GetAttr<spt<EnPolygon>>(Attrs::POLYGON);

		ofColor color = owner->GetAttr<ofColor>(Attrs::COLOR);
		ofSetColor(color);
		// todo: draw polygon
	}

	void RenderRectangle(GNode* owner){
		ofVec3f size = owner->GetAttr<ofVec3f>(Attrs::SIZE);
		ofColor color = owner->GetAttr<ofColor>(Attrs::COLOR);
		ofSetColor(color);
		ofFill();  
		ofRect(-size.x / 2, -size.y / 2, size.x, size.y);  
		ofEndShape();
	}

	void RenderArc(GNode* owner){
		ofVec3f size = owner->GetAttr<ofVec3f>(Attrs::SIZE);
		ofColor color = owner->GetAttr<ofColor>(Attrs::COLOR);
		ofSetColor(color);

		// todo: draw arc
		//Iw2DFillArc(ofVec3f(0, 0), size, 0, PI * 2, 60);
	}

	void RenderText(GNode* owner){
		ofSetColor(0,0,0);
		spt<ofTrueTypeFont> font = owner->GetAttr<spt<ofTrueTypeFont>>(Attrs::FONT);
		string text = owner->GetAttr<string>(Attrs::TEXT);
		font->drawString(text, 0, 0);
	}
};
