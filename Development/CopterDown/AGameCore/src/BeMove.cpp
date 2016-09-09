
#include "Attr.h"
#include "BeMove.h"
#include "ABehavior.h"
#include "IwGeomVec2.h"
#include "GNode.h"


BeMove::BeMove() : ABehavior(ElemType::MODEL, EnFlags()){

}

void BeMove::OnMessage(Msg& msg){

}

void BeMove::Update(const uint64 delta, const uint64 absolute){
	CIwFMat2D transform =_node->GetTransform();

	CIwVec2 velocity = _node->FindAtt<CIwVec2>(Attrs::VELOCITY)->GetValue();

	transform.t.x += velocity.x * delta;
	transform.t.y += velocity.y * delta;
}

