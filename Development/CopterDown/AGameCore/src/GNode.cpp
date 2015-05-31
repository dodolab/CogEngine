#include "GNode.h"
#include "ABehavior.h"
#include "GNodePool.h"
#include <vector>
#include <list>
#include <string>

// first id is always 1
int GNode::idCounter = 1;
Msg GNode::_dummyMsg = Msg();

GNode::GNode(ObjType type, int subType, string tag):  _type(type), _subType(subType), _id(idCounter++){
	if (!tag.empty()) SetTag(tag);
}

GNode::GNode(const GNode& copy) : _type(copy._type), _subType(copy._subType), _id(idCounter++){
	_tag = copy._tag;
	_attributes = copy._attributes;
	_behaviors = copy._behaviors;
	_parent = copy._parent;
	_groups = copy._groups;
	_states = copy._states;
	_transform = copy._transform;
}

GNode::~GNode(){
	
	delete _tag;
	delete _groups;
	delete _states;

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

void GNode::Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix){

	CIwFMat2D absoluteMatrix = _transform.GetMatrix()*absMatrix;
	

	for (auto it = _children.begin(); it != _children.end(); ++it){
		(*it)->Update(delta, absolute, absoluteMatrix);
	}

	for (auto it = _behaviors.begin(); it != _behaviors.end(); ++it){
		ABehavior* beh = *it;
	    if ((beh->GetBehState() == BehState::ACTIVE_ALL || beh->GetBehState() == BehState::ACTIVE_UPDATES) && beh->GetElemType() == ElemType::MODEL){
			beh->Update(delta, absolute, absoluteMatrix, this);
		}
	}
}

void GNode::Draw(const uint64 delta, const uint64 absolute, CIwFMat2D& absMatrix){

	CIwFMat2D absoluteMatrix = _transform.GetMatrix()*absMatrix;

	for (auto it = _behaviors.begin(); it != _behaviors.end(); ++it){
		ABehavior* beh = (*it);

		if (beh->GetElemType() == ElemType::VIEW && (beh->GetBehState() == BehState::ACTIVE_ALL || beh->GetBehState() == BehState::ACTIVE_UPDATES)){
			beh->Update(delta, absolute, absoluteMatrix, this);
		}
	}

	for (auto it = _children.begin(); it != _children.end(); ++it){
		(*it)->Draw(delta, absolute, absoluteMatrix);
	}
}

bool GNode::AddBehavior(ABehavior* beh){
	_behaviors.push_back(beh);
	return true;
}

bool GNode::RemoveBehavior(ABehavior* beh){  
	auto found = find(_behaviors.begin(), _behaviors.end(),beh);
	
	bool result = _behaviors.end() != found;
	if (result){
		_behaviors.remove(*found);
	}
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
	return _children;
}

const list<GNode*>& GNode::GetChildren() const{
	return _children;
}

bool GNode::AddChild(GNode* child){
	_children.push_back(child);
	child->_parent = this;
	return true;
}

bool GNode::RemoveChild(GNode* child){
	auto found = find(_children.begin(), _children.end(), child);

	bool result = _children.end() != found;
	if (result) _children.remove(*found);

	return result;
}

GNode* GNode::GetParent(){
	return _parent;
}

void GNode::SetParent(GNode* val){
	_parent = val;
}

GNode* GNode::FindPredecessor(ObjType type){
	GNode* parent = _parent;

	while (parent != nullptr && parent->_type != type) parent = parent->_parent;
	return parent;
}

GNode* GNode::GetSceneRoot(){
	if (_type == ObjType::SCENE) return this;
	else return FindPredecessor(ObjType::SCENE);
}

GNode* GNode::GetRoot(){
	if (_type == ObjType::ROOT) return this;
	else return FindPredecessor(ObjType::ROOT);
}

int GNode::GetId() const{
	return _id;
}
	
string GNode::GetTag() const{
	if (_tag == nullptr) return "";
	else return string(*_tag);
}

void GNode::SetTag(string tag){
	delete _tag;
	this->_tag = new string(tag);
}

ObjType GNode::GetType() const{
	return this->_type;
}

int GNode::GetSubType() const{
	return _subType;
}

void GNode::SetSubType(int val){
	this->_subType = val;
}

EnTransform& GNode::GetTransform(){
	return _transform;
}

void GNode::SetTransform(EnTransform val){
	this->_transform = val;
}

EnFlags& GNode::GetGroups(){
	if (_groups == nullptr) _groups = new EnFlags();
	return *_groups;
}

void GNode::SetGroups(EnFlags val){
	delete _groups;
	this->_groups = new EnFlags(val);
}

EnFlags& GNode::GetStates() {
	if (_states == nullptr) _states = new EnFlags();
	return *_states;
}

void GNode::SetStates(EnFlags val){
	delete _states;
	this->_states = new EnFlags(val);
}

