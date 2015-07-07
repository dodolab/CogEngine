#include "GNode.h"
#include "GBehavior.h"
#include "MGameStorage.h"
#include <vector>
#include <list>
#include <string>
#include "MGameEngine.h"

// first id is always 1
int GNode::idCounter = 1;

GNode::GNode(ObjType type, int subType, string tag):  _type(type), _subType(subType), _id(idCounter++), _transform(0,0){
	Construct();
	if (!tag.empty()) SetTag(tag);
}

GNode::GNode(const GNode& copy) : _type(copy._type), _subType(copy._subType), _id(idCounter++),  _transform(0,0){
	Construct();
	_tag = copy._tag;
	_attributes = copy._attributes;
	_behaviors = copy._behaviors;
	_parent = copy._parent;
	_groups = copy._groups;
	_states = copy._states;
	_transform = copy._transform;
	_runMode = copy._runMode;
}

GNode::~GNode(){
	
	delete _tag;
	delete _groups;
	delete _states;

	// delete all behaviors
	for (list<GBehavior*>::iterator it = _behaviors.begin(); it != _behaviors.end(); ++it)
	{
		MEngine.storage->RemoveBehavior((*it));
		delete (*it);
	}
	_behaviors.clear();

	// delete attributes
	for (map<int, GAttr*>::iterator it = _attributes.begin(); it != _attributes.end(); ++it){
		delete (it->second);
	}

	// delete all game objects
	for (auto it = _children.begin(); it != _children.end(); ++it)
	{
		MEngine.storage->RemoveGameObject((*it));
		delete (*it);
	}
	_children.clear();
}

void GNode::UpdateTransforms(){

	if (_parent != nullptr){
		this->_transform.CalcAbsTransform(_parent->_transform);
	}
	else this->_transform.SetAbsAsLocal();

	for (auto it = _children.begin(); it != _children.end(); ++it){
		(*it)->UpdateTransforms();
	}

}


void GNode::Update(const uint64 delta, const uint64 absolute){	

	if (_runMode == RunningMode::PAUSED_ALL) return;

	if (_runMode != RunningMode::PAUSED_ITSELF){
		for (auto it = _behaviors.begin(); it != _behaviors.end(); ++it){
			GBehavior* beh = *it;
			if (beh->GetElemType() == ElemType::MODEL && (beh->GetBehState() == BehState::ACTIVE_ALL || beh->GetBehState() == BehState::ACTIVE_UPDATES)){
				beh->Update(delta, absolute);
			}
		}
	}

	if (_runMode != RunningMode::PAUSED_CHILDREN){
		for (auto it = _children.begin(); it != _children.end(); ++it){
			(*it)->Update(delta, absolute);
		}
	}

	ClearElementsForRemoving();
}

void GNode::ClearElementsForRemoving(){
	for (auto it = _behaviorToRemove.begin(); it != _behaviorToRemove.end(); ++it){
		std::pair<GBehavior*, bool> item = (*it);
		GBehavior* beh = item.first;
		beh->owner = nullptr;
		_behaviors.remove(beh);
		MEngine.storage->RemoveBehavior(beh);
		if (item.second) delete item.first;
	}


	for (auto it = _childrenToRemove.begin(); it != _childrenToRemove.end(); ++it){
		std::pair<GNode*, bool> item = (*it);
		GNode* child = item.first;
		_children.remove(child);
		MEngine.storage->RemoveGameObject(child);
		if (item.second) delete item.first;
	}

	_childrenToRemove.clear();
	_behaviorToRemove.clear();
}

void GNode::Draw(const uint64 delta, const uint64 absolute){

	for (auto it = _behaviors.begin(); it != _behaviors.end(); ++it){
		GBehavior* beh = (*it);

		if (beh->GetElemType() == ElemType::VIEW && (beh->GetBehState() == BehState::ACTIVE_ALL || beh->GetBehState() == BehState::ACTIVE_UPDATES)){
			beh->Update(delta, absolute);
		}
	}

	for (auto it = _children.begin(); it != _children.end(); ++it){
		(*it)->Draw(delta, absolute);
	}
}

bool GNode::AddBehavior(GBehavior* beh){
	beh->owner = this;
	_behaviors.push_back(beh);
	MEngine.storage->AddBehavior(beh);
	return true;
}

bool GNode::RemoveBehavior(GBehavior* beh, bool immediately, bool erase){  
	auto found = find(_behaviors.begin(), _behaviors.end(),beh);
	
	bool result = _behaviors.end() != found;
	if (result){
		if (immediately){
			beh->owner = nullptr;
			_behaviors.remove(beh);
			MEngine.storage->RemoveBehavior(beh);
			if (erase) delete beh;
		}
		else{
			// check if there isn't already such behavior
			for (auto it = _behaviorToRemove.begin(); it != _behaviorToRemove.end(); ++it){
				if ((*it).first->GetId() == beh->GetId()) return true;
			}

			_behaviorToRemove.push_back(std::make_pair(*found, erase));
		}
	}
	return result;
}

bool GNode::RemoveAttr(int key, bool erase){
	
	map<int, GAttr*>::iterator it = _attributes.find(key);

	if (it != _attributes.end()){
		GAttr* attr = it->second;
		_attributes.erase(it);
		if(erase) delete attr;
		return true;
	}
	return false;
}

bool GNode::HasAttr(int key) const{
	return _attributes.find(key) != _attributes.end();
}

list<GBehavior*> GNode::GetBehaviorsCopy() const{
	return _behaviors;
}

const list<GBehavior*>& GNode::GetBehaviors() const{
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
	MEngine.storage->AddGameObject(child);
	return true;
}

bool GNode::RemoveChild(GNode* child, bool immediately, bool erase){
	auto found = find(_children.begin(), _children.end(), child);

	bool result = _children.end() != found;
	if (result){
		if (immediately){
			_children.remove(child);
			MEngine.storage->RemoveGameObject(child);
			if(erase) delete (child);
		}
		else{
			// check if there isn't already such behavior
			for (auto it = _childrenToRemove.begin(); it != _childrenToRemove.end(); ++it){
				if ((*it).first->GetId() == child->GetId()) return true;
			}
		
			_childrenToRemove.push_back(std::make_pair(child, erase));
		}
	}
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

string GNode::GetInfo(bool complete){
	std::ostringstream ss;
	GetInfo(complete, ss, 0);
	std::cout << ss.str() << std::endl;
	return ss.str();
}

void spaces(int howMany, std::ostringstream& ss){
	for (int i = 0; i < howMany; i++) ss << " ";
}

void GNode::GetInfo(bool complete, std::ostringstream& ss, int level){
	spaces(level * 4, ss);
	ss << (*_tag) << " " << _subType << std::endl;
	
	for (auto it = _behaviors.begin(); it != _behaviors.end(); ++it){
		GBehavior* beh = (*it);
		spaces(level * 4 + 2, ss);
		ss << typeid(*beh).name() << std::endl;
	}

	ss << std::endl;

	if (complete){
		for (auto it = _children.begin(); it != _children.end(); ++it){
			(*it)->GetInfo(complete, ss, level + 1);
		}
	}
}
