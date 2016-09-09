
#ifndef ABEHAVIOR_H
#define ABEHAVIOR_H

#include "EnState.h"
#include "Enums.h"
#include "Gnode.h"
#include "Msg.h"

class ABehavior{
private:
	static int idCounter;
	ElemType _elemType;
	int _id;
	BehState _behState;
	State _msgInvoker;
	GNode _node;

protected:
	ABehavior(ElemType elemType, State msgInvoker);

	Msg SendMessage(State traverse, int action, void* data);
	Msg SendMessage(int traverse, int action, void* data);

public:
	ElemType GetElemType();
	int GetId();

	BehState GetBehState();
	void SetBehState(BehState val);
	State GetMessageInvoker();

	GNode GetGNode();
	void SetGNode(GNode node);

	virtual void OnMessage(Msg msg) const = 0;
	virtual void Update(int delta, int absolute) const = 0;
};

#endif
