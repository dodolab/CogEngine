
#include "GBehavior.h"
#include "GNode.h"

int GBehavior::idCounter = 0;
GMsg GBehavior::_dummyMsg = GMsg();

GBehavior::GBehavior(ElemType elemType, EnFlags msgFlags) : _behState(BehState::ACTIVE_ALL),
_elemType(elemType), _msgFlags(msgFlags), _id(idCounter++) {

}

void GBehavior::SendMessage(EnFlags traverse, int action, void* data, GMsg& resp, GNode* target) const{
	GMsg msg(_elemType,traverse, action,SenderType::BEHAVIOR,_id,data);
	target->SendMessage(msg, resp);
}

void GBehavior::SendMessageNoResp(EnFlags traverse, int action, void* data, GNode* target) const{
	SendMessage(traverse, action, data, _dummyMsg, target);
}