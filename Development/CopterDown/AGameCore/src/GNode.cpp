#include "GNode.h"
#include "ABehavior.h"
#include "GNodePool.h"
#include <vector>
#include <list>

// first id is always 1
int GNode::idCounter = 1;
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
	delete[] _tag;

	// delete all behaviors
	for (list<ABehavior*>::iterator it = _behaviors.begin(); it != _behaviors.end(); ++it)
	{
		delete (*it);
	}
	_behaviors.clear();

	for (map<int, Attr*>::iterator it = _attributes.begin(); it != _attributes.end(); ++it){
		delete (it->second);
	}
}

void GNode::SendMessage(Msg& msg, Msg& resp){
	GNodePool::Get()->OnMessage(msg);

	if (GNodePool::Get()->IsRegisteredListener(msg.GetAction())){
		if (msg.GetTraverse().HasState(Traverses::ROOT)){
			msg.GetTraverse().ResetState(Traverses::ROOT);

			if (GetRoot() != nullptr){
				GetRoot()->SendMessage(msg, resp);
				return;
			}
		}

		if (msg.GetTraverse().HasState(Traverses::SCENEROOT)){
			msg.GetTraverse().ResetState(Traverses::SCENEROOT);

			if (GetSceneRoot() != nullptr){
				GetSceneRoot()->SendMessage(msg, resp);
				return;
			}
		}

		// check only to prevent from settings traverses to both: child first and beh first
		bool childfirst = false;

		// traverse children at first
		if (msg.GetTraverse().HasState(Traverses::CHILD_FIRST)){
			childfirst = true;

			for (auto it = _children.begin(); it != _children.end(); ++it){
				  (*it)->SendMessage(msg, resp);
			}
		}

		for (auto it = _behaviors.begin(); it != _behaviors.end(); ++it){
			ABehavior* beh = (*it);
			if ((beh->GetBehState() == BehState::ACTIVE_MESSAGES || beh->GetBehState() == BehState::ACTIVE_ALL) &&
				(msg.GetSenderType() != SenderType::BEHAVIOR || beh->GetId() != msg.GetOwnerId()) &&
				(msg.GetCategory() == ElemType::ALL || beh->GetElemType() == msg.GetCategory())){
				if (beh->GetMessageFlags().HasState(msg.GetAction())){
					beh->OnMessage(msg);
				}
			}
		}

		if (!childfirst &&msg.GetTraverse().HasState(Traverses::BEH_FIRST)){
			for (auto it = _children.begin(); it != _children.end(); ++it){
				(*it)->SendMessage(msg, resp);
			}
		}
	}
}

void GNode::SendMessageNoResp(Msg& msg){
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
	for (auto it = _children.begin(); it != _children.end(); ++it){
		(*it)->Draw(delta, absolute);
	}

	for (auto it = _behaviors.begin(); it != _behaviors.end(); ++it){
		ABehavior* beh = (*it);

		if ((beh->GetBehState() == BehState::ACTIVE_ALL || beh->GetBehState() == BehState::ACTIVE_UPDATES) &&
			beh->GetElemType() == ElemType::VIEW){
			beh->Update(delta, absolute);
		}
	}
}

bool GNode::AddBehavior(ABehavior* beh){
	beh->_owner = this;
	_behaviors.push_back(beh);
	return true;
}

bool GNode::RemoveBehavior(ABehavior* beh){
		list<ABehavior*>::iterator found = find(_behaviors.begin(), _behaviors.end(),beh);
	
	bool result = _behaviors.end() != found;
	if (result) _behaviors.remove(*found);

	return result;
}

bool GNode::RemoveAttr(int key){
	
	map<int, Attr*>::iterator it = _attributes.find(key);

	if (it != _attributes.end()){
		Attr* attr = it->second;
		_attributes.erase(it);
		delete attr;
		return true;
	}
	return false;
}

bool GNode::HasAttr(int key) const{
	return _attributes.find(key) != _attributes.end();
}

list<ABehavior*> GNode::GetBehaviorsCopy() const{
	return _behaviors;
}

const list<ABehavior*>& GNode::GetBehaviors() const{
	return _behaviors;
}

list<GNode*> GNode::GetChildrenCopy() const{
	list<GNode*> output;

//	for (list<GNode*>::iterator it = _children.begin(); it != _children.end(); ++it){

	//}

	return _children;
}

const list<GNode*>& GNode::GetChildren() const{
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

