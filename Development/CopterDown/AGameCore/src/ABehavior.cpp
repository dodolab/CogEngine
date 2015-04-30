
#include "ABehavior.h"
#include "GNode.h"

int ABehavior::idCounter = 0;
Msg ABehavior::_dummyMsg = Msg();

ABehavior::ABehavior(ElemType elemType, EnFlags msgFlags) : _behState(BehState::ACTIVE_ALL),
_elemType(elemType), _msgFlags(msgFlags), _id(idCounter++) {

}

void ABehavior::SendMessage(EnFlags traverse, int action, void* data, Msg& resp) const{
	Msg msg(_elemType,traverse, action,SenderType::BEHAVIOR,_id,data);
	_owner->SendMessage(msg, resp);
}

void ABehavior::SendMessageNoResp(EnFlags traverse, int action, void* data) const{
	SendMessage(traverse, action, data, _dummyMsg);
}

const ElemType ABehavior::GetElemType() const{
		return _elemType;
	}

const int ABehavior::GetId() const{
		return _id;
	}

const BehState ABehavior::GetBehState() const{
		return _behState;
	}

const GNode* ABehavior::GetOwnerNode() const{
	return _owner;
}

const EnFlags& ABehavior::GetMessageFlags() const{
	return _msgFlags;
}


void ABehavior::SetBehState(BehState val){
		_behState = val;
	}
