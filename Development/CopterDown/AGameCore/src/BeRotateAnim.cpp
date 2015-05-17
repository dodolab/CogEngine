
#include "BeRotateAnim.h"


BeRotateAnim::BeRotateAnim(float from, float to, float speed, bool additive) : ABehavior(ElemType::MODEL, EnFlags()), _from(from), _to(to), _speed(speed), _additive(additive), _stopped(false){

}

void BeRotateAnim::OnMessage(Msg& msg){

}

void BeRotateAnim::Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner){

}

