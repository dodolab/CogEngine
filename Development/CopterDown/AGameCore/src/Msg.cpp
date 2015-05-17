#include "Msg.h"

int Msg::idCounter = 0;

Msg::Msg(ElemType cat, EnFlags traverse, int action, SenderType senderType, int ownerId, void* data) : _category(cat), _traverse(traverse), _action(action),
_senderType(senderType), _ownerId(ownerId), _data(data), _id(idCounter++) {
}

Msg::Msg(ElemType cat, int action, SenderType senderType, int ownerId, void* data) : _category(cat), _traverse(EnFlags(Traverses::SCENEROOT)), _action(action),
_senderType(senderType), _ownerId(ownerId), _data(data), _id(idCounter++) {
}

const int Msg::GetAction() const{
	return _action;
}

const ElemType Msg::GetCategory() const{
	return _category;
}

EnFlags& Msg::GetTraverse(){
	return _traverse;
}

const SenderType Msg::GetSenderType() const{
	return _senderType;
}

const void* Msg::GetData() const{
	return _data;
}

void* Msg::GetData(){
	return _data;
}

const int Msg::GetOwnerId() const{
	return _ownerId;
}

const bool Msg::IsInitialized() const{
	return _id == 0;
}

bool Msg::Initialize(ElemType cat, EnFlags traverse, int action, SenderType senderType, int ownerId, void* data){
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

