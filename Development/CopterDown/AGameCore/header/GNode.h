
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
	map<int, Attr> _attributes;
	vector<ABehavior*> _behaviors;
	GNode* _parent;
	vector<GNode*> _children;

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

	void SendMessage(Msg* msg);

	void Update(int delta, int absolute);

	void Draw(int delta, int absolute);

	void AddBehavior(ABehavior* beh);

	void RemoveBehavior(ABehavior* beh);

	void RemoveAttr(int key);

	template<class T> Attrx<T> FindAtt(int id);

	template<class T> T FindAttValue(int id);

	vector<Attr*> GetAttributes();

	vector<ABehavior*> GetBehaviors();

	vector<GNode*> GetChildren();

	void AddChild(GNode* child);

	void RemoveChild(GNode* child);

	GNode GetParent();
	void SetParent(GNode val);

	int GetId();
	void SetId(int val);

	char* GetTag();
	void SetTag(char* tag);

	ObjType GetType();
	void SetType(ObjType val);

	int GetSubType();
	void SetSubType(int val);

	GNode FindParent(ObjType type);

	GNode GetSceneRoot();
	
	GNode GetRoot();

	CIwFMat2D GetTransform();
	CIwFMat2D SetTransform(CIwFMat2D val);

	template<class T> void AddAttribute(ElemType elemType, int key, T value);

	EnFlags GetGroups();
	void SetGroups(EnFlags val);

	EnFlags GetStates();
	void SetStates(EnFlags val);
};


GNode::GNode(ObjType type, int subType, char* tag){

}

GNode::~GNode(){

}

void GNode::SendMessage(Msg* msg){

}

void GNode::Update(int delta, int absolute){

}

void GNode::Draw(int delta, int absolute){

}

void GNode::AddBehavior(ABehavior* beh){

}

void GNode::RemoveBehavior(ABehavior* beh){
}

void GNode::RemoveAttr(int key){

}

template<class T> Attrx<T> GNode::FindAtt(int id){
	return Attrx<T>();
}

template<class T> T GNode::FindAttValue(int id){
	return T();
}

vector<Attr*> GNode::GetAttributes(){
	return vector<Attr*>();
}

vector<ABehavior*> GNode::GetBehaviors(){
	return vector<ABehavior*>();
}

vector<GNode*> GNode::GetChildren(){
	return vector<GNode*>();
}

void GNode::AddChild(GNode* child){

}

void GNode::RemoveChild(GNode* child){

}

GNode GNode::GetParent(){
	return GNode(ObjType::HUD, 12, nullptr);
}

void GNode::SetParent(GNode val){

}

int GNode::GetId(){
	return 0;
}

void GNode::SetId(int val){

}

char* GNode::GetTag(){
	return new char[5];
}

void GNode::SetTag(char* tag){

}

ObjType GNode::GetType(){
	return ObjType::HUD;
}

void GNode::SetType(ObjType val){

}

int GNode::GetSubType(){
	return 0;
}

void GNode::SetSubType(int val){

}

GNode GNode::FindParent(ObjType type){
	return GNode(ObjType::HUD, 12, nullptr);
}

GNode GNode::GetSceneRoot(){
	return GNode(ObjType::HUD, 12, nullptr);
}

GNode GNode::GetRoot(){
	return GNode(ObjType::HUD, 12, nullptr);
}

CIwFMat2D GNode::GetTransform(){
	return CIwFMat2D();
}

CIwFMat2D GNode::SetTransform(CIwFMat2D val){
	return CIwFMat2D();
}

template<class T> void GNode::AddAttribute(ElemType elemType, int key, T value){

}

EnFlags GNode::GetGroups(){
	return EnFlags();
}

void GNode::SetGroups(EnFlags val){

}

EnFlags GNode::GetStates(){
	return EnFlags();
}

void GNode::SetStates(EnFlags val){

}

#endif
