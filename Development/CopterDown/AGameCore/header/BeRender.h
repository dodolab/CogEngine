#ifndef BERENDER_H
#define BERENDER_H

#include "GBehavior.h"
#include "GNode.h"
#include "SmartPointer.h"
#include "Iw2D.h"
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

	virtual void Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner){

		Iw2DSetTransformMatrix(absMatrix);

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
		Iw2DSetColour(0xffffffff);
		spt<CIw2DImage> image = owner->GetAttr<spt<CIw2DImage>>(Attrs::IMGSOURCE);
		Iw2DDrawImage(image, Vectorf2(-image->GetWidth() / 2, -image->GetHeight() / 2));
	}

	void RenderPolygon(GNode* owner){
		spt<EnPolygon> poly = owner->GetAttr<spt<EnPolygon>>(Attrs::POLYGON);
		CIwColour color = owner->GetAttr<CIwColour>(Attrs::COLOR);
		Iw2DSetColour(color);
		Iw2DFillPolygon(poly->verts, poly->vertCount);
	}

	void RenderRectangle(GNode* owner){
		Vectorf2 size = owner->GetAttr<Vectorf2>(Attrs::SIZE);
		CIwColour color = owner->GetAttr<CIwColour>(Attrs::COLOR);
		Iw2DSetColour(color);
		Iw2DFillRect(Vectorf2(-size.x / 2, -size.y / 2), size);
	}

	void RenderArc(GNode* owner){
		Vectorf2 size = owner->GetAttr<Vectorf2>(Attrs::SIZE);
		CIwColour color = owner->GetAttr<CIwColour>(Attrs::COLOR);
		Iw2DSetColour(color);
		Iw2DFillArc(Vectorf2(0, 0), size, 0, PI * 2, 60);
	}
};


#endif