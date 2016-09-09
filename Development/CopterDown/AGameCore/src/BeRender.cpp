

#include "BeRender.h"
#include "Iw2D.h"
#include "SmartPointer.h"
#include "GNode.h"
#include "EnPolygon.h"

BeRender::BeRender(RenderType type) : ABehavior(ElemType::VIEW, EnFlags()){
	this->_type = type;
}

void BeRender::OnMessage(Msg& msg){

}



void BeRender::Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner){

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

void BeRender::RenderImage(GNode* owner){
	Iw2DSetColour(0xffffffff);
	spt<CIw2DImage> image = owner->GetAttr<spt<CIw2DImage>>(Attrs::IMGSOURCE);
	Iw2DDrawImage(image, CIwFVec2(-image->GetWidth() / 2, -image->GetHeight() / 2));
}

void BeRender::RenderPolygon(GNode* owner){
	spt<EnPolygon> poly = owner->GetAttr<spt<EnPolygon>>(Attrs::POLYGON);
	CIwColour color = owner->GetAttr<CIwColour>(Attrs::COLOR);
	Iw2DSetColour(color); 
	Iw2DFillPolygon(poly->verts, poly->vertCount);
}

void BeRender::RenderRectangle(GNode* owner){
	CIwFVec2 size = owner->GetAttr<CIwFVec2>(Attrs::SIZE);
	CIwColour color = owner->GetAttr<CIwColour>(Attrs::COLOR);
	Iw2DSetColour(color);
	Iw2DFillRect(CIwFVec2(-size.x / 2, -size.y / 2),size);
}

void BeRender::RenderArc(GNode* owner){
	CIwFVec2 size = owner->GetAttr<CIwFVec2>(Attrs::SIZE);
	CIwColour color = owner->GetAttr<CIwColour>(Attrs::COLOR);
	Iw2DSetColour(color);
	Iw2DFillArc(CIwFVec2(0,0), size, 0, PI*2, 60);
}
