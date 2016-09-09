
#ifndef ABEHAVIOR_H
#define ABEHAVIOR_H

#include "EnFlags.h"
#include "Enums.h"
#include "Msg.h"

class GNode;

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


#endif