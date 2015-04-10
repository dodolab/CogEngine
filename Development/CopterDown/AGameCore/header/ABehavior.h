
#ifndef ABEHAVIOR_H
#define ABEHAVIOR_H

#include "EnFlags.h"
#include "Enums.h"
#include "Gnode.h"
#include "Msg.h"

class ABehavior{

protected:
	static int idCounter;
	ElemType _elemType;
	int _id;
	BehState _behState;
	EnFlags _msgInvoker;
	GNode* _node;

	ABehavior(ElemType elemType, EnFlags msgInvoker);

	Msg* SendMessage(EnFlags traverse, int action, void* data);

public:
	ElemType GetElemType();

	int GetId();

	BehState GetBehState();

	void SetBehState(BehState val);

	EnFlags GetMessageInvoker();

	GNode* GetGNode();

	void SetGNode(GNode* node);

	virtual void OnMessage(Msg msg) const = 0;
	virtual void Update(uint64 delta, uint64 absolute) const = 0;
};




int ABehavior::idCounter = 0;

ABehavior::ABehavior(ElemType elemType, EnFlags msgInvoker) : _behState(BehState::ACTIVE_ALL), _elemType(elemType), _msgInvoker(msgInvoker) {
	this->_id = idCounter++;
}

Msg* ABehavior::SendMessage(EnFlags traverse, int action, void* data){
	Msg* msg = new Msg(_elemType, traverse, action, SenderType::BEHAVIOR, _id, data);
	_node->SendMessage(msg);
	return msg;
}


ElemType ABehavior::GetElemType(){
		return _elemType;
	}

int ABehavior::GetId(){
		return _id;
	}

BehState ABehavior::GetBehState(){
		return _behState;
	}

void ABehavior::SetBehState(BehState val){
		_behState = val;
	}

EnFlags ABehavior::GetMessageInvoker(){
		return _msgInvoker;
	}

GNode* ABehavior::GetGNode(){
		return _node;
	}

void ABehavior::SetGNode(GNode* node){
		this->_node = node;
	}

#endif