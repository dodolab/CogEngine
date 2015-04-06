
#ifndef MSG_H
#define MSG_H

#include "Enums.h"
#include "EnState.h"

class Msg{
private:
	int _action;
	ElemType _category;
	State _traverse;
	SenderType _senderType;
	void* _data;
	void* _response;
	int _ownerId;

public:
	Msg(ElemType cat, State traverse, int action, SenderType senderType, int ownerId, void* data);
	Msg(ElemType cat, int traverse, int action, SenderType senderType, int ownerId, void* data);
	Msg(ElemType cat, int action, SenderType senderType, int ownerId, void* data);

	int GetAction();
	void SetAction(int val);

	ElemType GetCategory();
	void SetCategory(ElemType val);

	State GetTraverse();
	void SetTraverse(State val);

	SenderType GetSenderType();
	void SetSenderType(SenderType val);

	void* GetData();
	void SetData(void* val);

	void* GetResponse();
	void SetResponse(void* val);

	int GetOwnerId();
	void SetOwnerId(int val);
};


#endif
