
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

	void SendMessage(EnFlags traverse, int action, Msg& msg, void* data) const;

public:
	ElemType GetElemType() const;

	int GetId() const;

	BehState GetBehState() const;

	void SetBehState(BehState val) ;

	EnFlags& GetMessageInvoker();

	const GNode* GetGNode() const;

	void SetGNode(const GNode* node);

	virtual void OnMessage(Msg& msg) const = 0;
	virtual void Update(const uint64 delta, const uint64 absolute) const = 0;
};




int ABehavior::idCounter = 0;

ABehavior::ABehavior(ElemType elemType, EnFlags msgInvoker) : _behState(BehState::ACTIVE_ALL), 
_elemType(elemType), _msgInvoker(msgInvoker), _id(idCounter++) {

}

void ABehavior::SendMessage(EnFlags traverse, int action, Msg& msg, void* data) const{
	msg.SetCategory(_elemType);
	msg.SetTraverse(traverse);
	msg.SetAction(action);
	msg.SetSenderType(SenderType::BEHAVIOR);
	msg.SetOwnerId(_id);
	msg.SetData(data);
	
	_node->SendMessage(msg);
}


ElemType ABehavior::GetElemType() const{
		return _elemType;
	}

int ABehavior::GetId() const{
		return _id;
	}

BehState ABehavior::GetBehState() const{
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

void ABehavior::SetGNode(const GNode* node){
	this->_node = const_cast<GNode*>(node);
	}

#endif