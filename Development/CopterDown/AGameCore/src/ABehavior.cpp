
#include "ABehavior.h"
#include "GNode.h"

int ABehavior::idCounter = 0;
Msg ABehavior::_dummyMsg = Msg();

ABehavior::ABehavior(ElemType elemType, EnFlags msgFlags) : _behState(BehState::ACTIVE_ALL),
_elemType(elemType), _msgFlags(msgFlags), _id(idCounter++) {

}

void ABehavior::SendMessage(EnFlags traverse, int action, void* data, Msg& resp, GNode* target) const{
	Msg msg(_elemType,traverse, action,SenderType::BEHAVIOR,_id,data);
	target->SendMessage(msg, resp);
}

void ABehavior::SendMessageNoResp(EnFlags traverse, int action, void* data, GNode* target) const{
	SendMessage(traverse, action, data, _dummyMsg, target);
}