
#include "ABehavior.h"
#include "GNode.h"

int ABehavior::idCounter = 0;

ABehavior::ABehavior(ElemType elemType, EnFlags msgInvoker) : _behState(BehState::ACTIVE_ALL), 
_elemType(elemType), _msgInvoker(msgInvoker), _id(idCounter++) {

}

void ABehavior::SendMessage(EnFlags traverse, int action, void* data, Msg& resp) const{
	Msg msg(_elemType,traverse, action,SenderType::BEHAVIOR,_id,data);
	_node->SendMessage(msg, resp);
}

void ABehavior::SendMessageNoResp(EnFlags traverse, int action, void* data) const{
	Msg resp;
	SendMessage(traverse, action, data, resp);
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

void ABehavior::SetBehState(BehState val){
		_behState = val;
	}

EnFlags& ABehavior::GetMessageInvoker(){
		return _msgInvoker;
	}

const GNode* ABehavior::GetGNode() const{
	return _node;
}


void ABehavior::SetGNode(GNode* node){
	this->_node = node;
	}

