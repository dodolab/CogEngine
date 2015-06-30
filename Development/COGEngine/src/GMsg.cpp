#include "GMsg.h"

int GMsg::idCounter = 0;

GMsg::GMsg(ElemType cat, Traversation traverse, int action, int behaviorId, GNode* sourceObj, void* data) : _category(cat), _traverse(traverse), _action(action),
_behaviorId(behaviorId), _sourceObj(sourceObj), _data(data), _id(idCounter++) {
	Construct();
}

const int GMsg::GetAction() const{
	return _action;
}

const ElemType GMsg::GetCategory() const{
	return _category;
}

Traversation& GMsg::GetTraverse(){
	return _traverse;
}

const void* GMsg::GetData() const{
	return _data;
}

void* GMsg::GetData(){
	return _data;
}

const int GMsg::GetBehaviorId() const{
	return _behaviorId;
}
