#include "GNode.h"
#include "ABehavior.h"

int GNode::ids = 0;

GNode::GNode(ObjType type, int subType, char* tag):_type(type), _subType(subType), _tag(tag), _id(ids++){

}

GNode::GNode(const GNode& copy) : _type(copy._type), _subType(copy._subType), _id(ids++){
	memcpy(copy._tag, _tag, sizeof(copy._tag));
	_attributes = copy._attributes;
	_behaviors = copy._behaviors;
	_parent = copy._parent;
	_groups = copy._groups;
	_states = copy._states;
	_transform = copy._transform;
}

GNode::~GNode(){
	delete _tag;

	// delete all behaviors
	for (CIwArray<ABehavior*>::iterator it = _behaviors.begin(); it != _behaviors.end(); ++it)
	{
		delete (*it);
	}
	_behaviors.clear();
}

void GNode::SendMessage(Msg& msg, Msg& resp) const{

}

void GNode::SendMessageNoResp(Msg& msg) const{
	Msg resp;
	SendMessage(msg, resp);
}

void GNode::Update(const uint64 delta, const uint64 absolute){
	for (auto it = _children.begin(); it != _children.end(); ++it){
		(*it)->UpdateTransform(_transform);

	}

	for (auto it = _children.begin(); it != _children.end(); ++it){
		(*it)->Update(delta, absolute);
	}

	for (auto it = _behaviors.begin(); it != _behaviors.end(); ++it){
		ABehavior* beh = *it;
	    if ((beh->GetBehState() == BehState::ACTIVE_ALL || beh->GetBehState() == BehState::ACTIVE_UPDATES) && beh->GetElemType() == ElemType::MODEL){
			beh->Update(delta, absolute);
		}
	}
}

void GNode::Draw(const uint64 delta, const uint64 absolute){

}

void GNode::AddBehavior(ABehavior* beh){
	_behaviors.push_back(beh);
}

void GNode::RemoveBehavior(ABehavior* beh){
}

void GNode::RemoveAttr(int key){

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

void GNode::UpdateTransform(CIwFMat2D& parent){

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

