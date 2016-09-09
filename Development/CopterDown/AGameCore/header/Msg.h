
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

	int GetAction() const;
	void SetAction(int val);

	ElemType GetCategory() const;
	void SetCategory(ElemType val);

	EnFlags GetTraverse() const;
	void SetTraverse(EnFlags val);

	SenderType GetSenderType() const;
	void SetSenderType(SenderType val);

	void* GetData() const;
	void SetData(void* val);

	void* GetResponse() const;
	void SetResponse(void* val);

	int GetOwnerId() const;
	void SetOwnerId(int val);
};


Msg::Msg(ElemType cat, EnFlags traverse, int action, SenderType senderType, int ownerId, void* data){

}
Msg::Msg(ElemType cat, int action, SenderType senderType, int ownerId, void* data){

}

int Msg::GetAction() const{
	return 0;
}
void Msg::SetAction(int val){

}

ElemType Msg::GetCategory() const{
	return ElemType::ALL;
}
void Msg::SetCategory(ElemType val){

}

EnFlags Msg::GetTraverse() const{
	return EnFlags();
}
void Msg::SetTraverse(EnFlags val){

}

SenderType Msg::GetSenderType() const{
	return SenderType::ATTR;
}
void Msg::SetSenderType(SenderType val){

}

void* Msg::GetData() const{
	return nullptr;
}
void Msg::SetData(void* val){

}

void* Msg::GetResponse() const{
	return nullptr;
}
void Msg::SetResponse(void* val){

}

int Msg::GetOwnerId() const{
	return 0;
}
void Msg::SetOwnerId(int val){

}


#endif
