
#ifndef ABEHAVIOR_H
#define ABEHAVIOR_H

#include "EnFlags.h"
#include "Enums.h"
#include "Gnode.h"
#include "Msg.h"

class ABehavior{

protected:
	static int idCounter;
	const ElemType _elemType;
	const int _id;
	BehState _behState;
	EnFlags _msgInvoker;
	GNode* _node;

	ABehavior(ElemType elemType, EnFlags msgInvoker);

	void SendMessage(EnFlags traverse, int action, void* data, Msg& resp) const;

	void SendMessageNoResp(EnFlags traverse, int action, void* data) const;

public:
	const ElemType GetElemType() const;

	const int GetId() const;

	const BehState GetBehState() const;

	void SetBehState(BehState val);

	EnFlags& GetMessageInvoker();

	const GNode* GetGNode() const;

	void SetGNode(GNode* node);

	virtual void OnMessage(Msg& msg) const = 0;
	virtual void Update(const uint64 delta, const uint64 absolute) const = 0;
};


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

#endif