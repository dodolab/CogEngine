#ifndef BERENDER_H
#define BERENDER_H

#include "GBehavior.h"
#include "GNode.h"
#include "SmartPointer.h"
#include "EnPolygon.h"

/*! User input action enumerator */
enum class RenderType{
	IMAGE, RECTANGLE, POLYGON, ARC
};

/**
* Behavior for rendering
*/
class BeRender : public GBehavior{
private:
	RenderType _type;
public:
	BeRender(RenderType type) : GBehavior(ElemType::VIEW, EnFlags()){
		this->_type = type;
	}

	virtual void OnMessage(GMsg& msg){

	}


	virtual void Update(const uint64 delta, const uint64 absolute, const ofMatrix4x4& absMatrix, GNode* owner){

		ofLoadMatrix(absMatrix);

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
		}

	}
	
	void RenderImage(GNode* owner){
		ofSetHexColor(0xffffff);

		spt<ofImage> image = owner->GetAttr<spt<ofImage>>(Attrs::IMGSOURCE);
		image->draw(-image->width/2,-image->height/2);
	}

	void RenderPolygon(GNode* owner){
		spt<EnPolygon> poly = owner->GetAttr<spt<EnPolygon>>(Attrs::POLYGON);

		ofColor color = owner->GetAttr<ofColor>(Attrs::COLOR);
		ofSetColor(color);
		// todo: draw polygon
	}

	void RenderRectangle(GNode* owner){
		Vectorf3 size = owner->GetAttr<Vectorf3>(Attrs::SIZE);
		ofColor color = owner->GetAttr<ofColor>(Attrs::COLOR);
		ofSetColor(color);
		ofFill();  
		ofRect(-size.x / 2, -size.y / 2, size.x, size.y);  
		ofEndShape();
	}

	void RenderArc(GNode* owner){
		Vectorf3 size = owner->GetAttr<Vectorf3>(Attrs::SIZE);
		ofColor color = owner->GetAttr<ofColor>(Attrs::COLOR);
		ofSetColor(color);

		// todo: draw arc
		//Iw2DFillArc(Vectorf3(0, 0), size, 0, PI * 2, 60);
	}
};


#endif