
#ifndef MSG_H
#define MSG_H

#include "Enums.h"
#include "EnFlags.h"

class Msg{
private:
	int _action;
	ElemType _category;
	EnFlags _traverse;
	SenderType _senderType;
	void* _data;
	void* _response;
	int _ownerId;

public:
	Msg(ElemType cat, EnFlags traverse, int action, SenderType senderType, int ownerId, void* data);
	Msg(ElemType cat, int action, SenderType senderType, int ownerId, void* data);

	int GetAction();
	void SetAction(int val);

	ElemType GetCategory();
	void SetCategory(ElemType val);

	EnFlags GetTraverse();
	void SetTraverse(EnFlags val);

	SenderType GetSenderType();
	void SetSenderType(SenderType val);

	void* GetData();
	void SetData(void* val);

	void* GetResponse();
	void SetResponse(void* val);

	int GetOwnerId();
	void SetOwnerId(int val);
};


Msg::Msg(ElemType cat, EnFlags traverse, int action, SenderType senderType, int ownerId, void* data){

}
Msg::Msg(ElemType cat, int action, SenderType senderType, int ownerId, void* data){

}

int Msg::GetAction(){
	return 0;
}
void Msg::SetAction(int val){

}

ElemType Msg::GetCategory(){
	return ElemType::ALL;
}
void Msg::SetCategory(ElemType val){

}

EnFlags Msg::GetTraverse(){
	return EnFlags();
}
void Msg::SetTraverse(EnFlags val){

}

SenderType Msg::GetSenderType(){
	return SenderType::ATTR;
}
void Msg::SetSenderType(SenderType val){

}

void* Msg::GetData(){
	return nullptr;
}
void Msg::SetData(void* val){

}

void* Msg::GetResponse(){
	return nullptr;
}
void Msg::SetResponse(void* val){

}

int Msg::GetOwnerId(){
	return 0;
}
void Msg::SetOwnerId(int val){

}


#endif
