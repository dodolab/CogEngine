
#ifndef GNODE_H
#define GNODE_H

#include <map>
#include "Attr.h"
#include "ABehavior.h"
#include "Msg.h"

using namespace std;

class Attr;
class ABehavior;

class GNode{
protected:
	map<int, Attr*> _attributes;
	CIwArray<ABehavior*> _behaviors;
	GNode* _parent;
	CIwArray<GNode*> _children;

	int _id;
	char* _tag;
	ObjType _type;
	int _subType;
	EnFlags _groups;
	EnFlags _states;
	CIwFMat2D  _transform;

	static int ids;


public:

	GNode(ObjType type, int subType, char* tag);

	~GNode();

	void SendMessage(Msg& msg, Msg& resp) const;

	void SendMessageNoResp(Msg& msg) const;

	void Update(const uint64 delta, const uint64 absolute);

	void Draw(const uint64 delta, const uint64 absolute);

	void AddBehavior(ABehavior* beh);

	void RemoveBehavior(ABehavior* beh);

	void RemoveAttr(int key);

	template<class T> Attrx<T>* FindAtt(int id) const;

	template<class T> T* FindAttValue(int id) const;

	const CIwArray<ABehavior*> GetBehaviors() const;

	const CIwArray<GNode*> GetChildren() const;

	void AddChild(GNode* child);

	void RemoveChild(GNode* child);

	GNode* GetParent() const;
	void SetParent(GNode* val);

	int GetId() const;
	void SetId(int val);

	char* GetTag() const;
	void SetTag(char* tag);

	ObjType GetType() const;
	void SetType(ObjType val);

	int GetSubType() const;
	void SetSubType(int val);

	GNode* FindParent(ObjType type) const;

	GNode* GetSceneRoot() const;
	
	GNode* GetRoot() const;

	CIwFMat2D GetTransform() const;
	CIwFMat2D SetTransform(CIwFMat2D val);

	template<class T> void AddAttribute(ElemType elemType, int key, T& value);

	EnFlags GetGroups() const;
	void SetGroups(EnFlags val);

	EnFlags GetStates() const;
	void SetStates(EnFlags val);
};


GNode::GNode(ObjType type, int subType, char* tag){

}

GNode::~GNode(){

}

void GNode::SendMessage(Msg& msg, Msg& resp) const{

}

void GNode::SendMessageNoResp(Msg& msg) const{
	Msg resp;
	SendMessage(msg, resp);
}

void GNode::Update(const uint64 delta, const uint64 absolute){

}

void GNode::Draw(const uint64 delta, const uint64 absolute){

}

void GNode::AddBehavior(ABehavior* beh){

}

void GNode::RemoveBehavior(ABehavior* beh){
}

void GNode::RemoveAttr(int key){

}

template<typename T> Attrx<T>* GNode::FindAtt(int id) const{
	return nullptr;
}

template<class T> T* GNode::FindAttValue(int id) const{
	return nullptr;
}

const CIwArray<ABehavior*> GNode::GetBehaviors() const{
	return _behaviors;
}

const CIwArray<GNode*> GNode::GetChildren() const{
	return _children;
}

void GNode::AddChild(GNode* child){

}

void GNode::RemoveChild(GNode* child){

}

GNode* GNode::GetParent() const{
	return nullptr;
}

void GNode::SetParent(GNode* val){

}

int GNode::GetId() const{
	return 0;
}

void GNode::SetId(int val){

}

char* GNode::GetTag() const{
	return new char[5];
}

void GNode::SetTag(char* tag){

}

ObjType GNode::GetType() const{
	return ObjType::HUD;
}

void GNode::SetType(ObjType val){

}

int GNode::GetSubType() const{
	return 0;
}

void GNode::SetSubType(int val){

}

GNode* GNode::FindParent(ObjType type) const{
	return nullptr;
}

GNode* GNode::GetSceneRoot() const{
	return nullptr;
}

GNode* GNode::GetRoot() const{
	return nullptr;
}

CIwFMat2D GNode::GetTransform() const{
	return CIwFMat2D();
}

CIwFMat2D GNode::SetTransform(CIwFMat2D val){
	return CIwFMat2D();
}

template<class T> void GNode::AddAttribute(ElemType elemType, int key, T& value){

}

EnFlags GNode::GetGroups() const{
	return EnFlags();
}

void GNode::SetGroups(EnFlags val){

}

EnFlags GNode::GetStates() const{
	return EnFlags();
}

void GNode::SetStates(EnFlags val){

}

#endif
