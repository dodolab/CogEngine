#include "GMsg.h"

int GMsg::idCounter = 0;

GMsg::GMsg(ElemType cat, EnFlags traverse, int action, SenderType senderType, int ownerId, void* data) : _category(cat), _traverse(traverse), _action(action),
_senderType(senderType), _ownerId(ownerId), _data(data), _id(idCounter++) {
}

GMsg::GMsg(ElemType cat, int action, SenderType senderType, int ownerId, void* data) : _category(cat), _traverse(EnFlags(Traverses::SCENEROOT)), _action(action),
_senderType(senderType), _ownerId(ownerId), _data(data), _id(idCounter++) {
}

const int GMsg::GetAction() const{
	return _action;
}

const ElemType GMsg::GetCategory() const{
	return _category;
}

EnFlags& GMsg::GetTraverse(){
	return _traverse;
}

const SenderType GMsg::GetSenderType() const{
	return _senderType;
}

const void* GMsg::GetData() const{
	return _data;
}

void* GMsg::GetData(){
	return _data;
}

const int GMsg::GetOwnerId() const{
	return _ownerId;
}

const bool GMsg::IsInitialized() const{
	return _id == 0;
}

bool GMsg::Initialize(ElemType cat, EnFlags traverse, int action, SenderType senderType, int ownerId, void* data){
	if (!IsInitialized()){
		_id = idCounter++;

		_category = cat;
		_traverse = traverse;
		_action = action;
		_senderType = senderType;
		_ownerId = ownerId;
		_data = data;

		return true;
	}
	else return false;
}

