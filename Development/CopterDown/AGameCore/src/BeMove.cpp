
#include "Attr.h"
#include "BeMove.h"
#include "ABehavior.h"
#include "IwGeomVec2.h"
#include "GNode.h"


BeMove::BeMove() : ABehavior(ElemType::MODEL, EnFlags()){

}

void BeMove::OnMessage(Msg& msg){

}

void BeMove::Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner){
	EnTransform& transform = owner->GetTransform();

	CIwVec2 velocity = owner->GetAttr<CIwVec2>(Attrs::VELOCITY);

	transform.LocalPos.x += velocity.x * delta;
	transform.LocalPos.y += velocity.y * delta;
}

