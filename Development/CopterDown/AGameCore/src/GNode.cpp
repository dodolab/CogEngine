#include "GNode.h"
#include "ABehavior.h"

int GNode::idCounter = 0;
Msg GNode::_dummyMsg = Msg();

GNode::GNode(ObjType type, int subType, char* tag):_type(type), _subType(subType), _tag(tag), _id(idCounter++){

}

GNode::GNode(const GNode& copy) : _type(copy._type), _subType(copy._subType), _id(idCounter++){
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
	for (CIwList<ABehavior*>::iterator it = _behaviors.begin(); it != _behaviors.end(); ++it)
	{
		delete (*it);
	}
	_behaviors.clear();
}

void GNode::SendMessage(Msg& msg, Msg& resp) const{

}

void GNode::SendMessageNoResp(Msg& msg) const{
	SendMessage(msg, _dummyMsg);
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

bool GNode::AddBehavior(ABehavior* beh){
	beh->_owner = this;
	_behaviors.push_back(beh);
	return true;
}

bool GNode::RemoveBehavior(ABehavior* beh){
	return true;
}

bool GNode::RemoveAttr(int key){
	return true;
}

bool GNode::HasAttr(int key) const{
	return false;
}

CIwList<ABehavior*> GNode::GetBehaviorsCopy() const{
	return _behaviors;
}

const CIwList<ABehavior*>& GNode::GetBehaviors() const{
	return _behaviors;
}

CIwList<GNode*> GNode::GetChildrenCopy() const{
	return _children;
}

const CIwList<GNode*>& GNode::GetChildren() const{
	return _children;
}

bool GNode::AddChild(GNode* child){
	return true;
}

bool GNode::RemoveChild(GNode* child){
	return true;
}

GNode* GNode::GetParent() const{
	return nullptr;
}

void GNode::SetParent(GNode* val){

}

GNode* GNode::FindPredecessor(ObjType type) const{
	return nullptr;
}

GNode* GNode::GetSceneRoot() const{
	return nullptr;
}

GNode* GNode::GetRoot() const{
	return nullptr;
}

int GNode::GetId() const{
	return 0;
}
	
char* GNode::GetTag() const{
	return new char[5];
}

void GNode::SetTag(char* tag){

}

ObjType GNode::GetType() const{
	return ObjType::HUD;
}

int GNode::GetSubType() const{
	return 0;
}

void GNode::SetSubType(int val){

}

CIwFMat2D& GNode::GetTransform() const{
	return CIwFMat2D();
}

void GNode::SetTransform(CIwFMat2D& val){
	
}

void GNode::UpdateTransform(CIwFMat2D& parent){

}

EnFlags& GNode::GetGroups() const{
	return EnFlags();
}

void GNode::SetGroups(EnFlags& val){

}

EnFlags& GNode::GetStates() const{
	return EnFlags();
}

void GNode::SetStates(EnFlags& val){

}

