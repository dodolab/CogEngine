

#include "BeRender.h"
#include "Iw2D.h"
#include "SmartPointer.h"
#include "GNode.h"

BeRender::BeRender() : ABehavior(ElemType::VIEW, EnFlags()){

}

void BeRender::OnMessage(Msg& msg){

}

void BeRender::Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner){

	Iw2DSetTransformMatrix(absMatrix);
	auto image = owner->GetAttr<spt<CIw2DImage>>(Attrs::IMGSOURCE);
	Iw2DDrawImage(image, CIwFVec2(1,1));

}

