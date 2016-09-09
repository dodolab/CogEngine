#include "GNode.h"
#include "GBehavior.h"
#include "MRepository.h"
#include <vector>
#include <list>
#include <string>
#include "MEngine.h"

// first id is always 1
int GNode::idCounter = 1;

GNode::GNode(string tag) : type(ObjType::OBJECT), subType(0), id(idCounter++), transform(0, 0){
	Construct();
	if (!tag.empty()) SetTag(tag);
}

GNode::GNode(ObjType type, int subType, string tag):  type(type), subType(subType), id(idCounter++), transform(0,0){
	Construct();
	if (!tag.empty()) SetTag(tag);
}

GNode::GNode(const GNode& copy) : type(copy.type), subType(copy.subType), id(idCounter++),  transform(0,0){
	Construct();
	tag = copy.tag;
	attributes = copy.attributes;
	behaviors = copy.behaviors;
	parent = copy.parent;
	groups = copy.groups;
	states = copy.states;
	transform = copy.transform;
	runMode = copy.runMode;
}

GNode::~GNode(){

	MLOGDEBUG("Destructing node %s", tag->c_str());
	// move elements from collection to insert so they can be removed from classic collections
	InsertElementsForAdding();

	// delete all behaviors
	for (list<GBehavior*>::iterator it = behaviors.begin(); it != behaviors.end(); ++it)
	{
		COGRemoveBehavior((*it));
		delete (*it);
	}
	behaviors.clear();

	/* don't delete attributes 
	// delete attributes
	for (map<int, GAttr*>::iterator it = attributes.begin(); it != attributes.end(); ++it){
		delete (it->second);
	}*/

	// delete all children
	for (auto it = children.begin(); it != children.end(); ++it)
	{
		COGRemoveNode((*it));
		delete (*it);
	}
	children.clear();

	delete tag;
	delete groups;
	delete states;
}

void GNode::UpdateTransform(bool deep){

	if (parent != nullptr){
		this->transform.CalcAbsTransform(parent->transform);
	}
	else this->transform.SetAbsAsLocal();

	if (deep){
		for (auto it = children.begin(); it != children.end(); ++it){
			(*it)->UpdateTransform(true);
		}
	}
}


void GNode::Update(const uint64 delta, const uint64 absolute){	

	if (runMode == RunningMode::PAUSED_ALL || runMode == RunningMode::DISABLED) return;

	if (runMode != RunningMode::PAUSED_ITSELF){
		// update behaviors
		for (auto it = behaviors.begin(); it != behaviors.end(); ++it){
			GBehavior* beh = *it;
			if (!beh->ended &&  beh->GetElemType() == ElemType::MODEL && (beh->GetBehState() == BehState::ACTIVE_ALL 
				|| beh->GetBehState() == BehState::ACTIVE_UPDATES)){
				beh->Update(delta, absolute);
			}
		}
	}

	if (runMode != RunningMode::PAUSED_CHILDREN){
		// update children
		int childrenCount = children.size();
		int childrenCounter = 0;

		for (auto it = children.begin(); it != children.end(); ++it){
			// prevent for while-update adding children
			if (childrenCounter++ < childrenCount){
				(*it)->Update(delta, absolute);
			}
		}
	}

	DeleteElementsForRemoving();
	InsertElementsForAdding();
}

void GNode::Draw(const uint64 delta, const uint64 absolute){

	if (runMode == RunningMode::INVISIBLE || runMode == RunningMode::DISABLED) return;

	for (auto it = behaviors.begin(); it != behaviors.end(); ++it){
		// draw behaviors
		GBehavior* beh = (*it);
		if (!beh->ended && beh->GetElemType() == ElemType::VIEW && (beh->GetBehState() == BehState::ACTIVE_ALL 
			|| beh->GetBehState() == BehState::ACTIVE_UPDATES)){
			beh->Update(delta, absolute);
		}
	}

	// TODO !!! solve another way
	// sort by z-index
	auto comp = [](GNode* lhs, GNode* rhs) -> bool
	{
		return lhs->GetTransform().localPos.z < rhs->GetTransform().localPos.z;
	};

	 children.sort(comp);

	for (auto it = children.begin(); it != children.end(); ++it){
		// draw children
		(*it)->Draw(delta, absolute);
	}
}

bool GNode::AddBehavior(GBehavior* beh, bool immediately){
	if (immediately){
		beh->owner = this;
		behaviors.push_back(beh);
		COGAddBehavior(beh);
		// initialize
		beh->Init();
	}
	else{
		behaviorsToAdd.push_back(beh);
	}
	return true;
}

bool GNode::RemoveBehavior(GBehavior* beh, bool immediately, bool erase){
	auto found = find(behaviors.begin(), behaviors.end(),beh);
	
	bool result = behaviors.end() != found;
	if (result){
		// behavior found
		if (immediately){
			beh->owner = nullptr;
			behaviors.remove(beh);
			COGRemoveBehavior(beh);
			if (erase) delete beh;
		}
		else{
			// check if there isn't already such behavior marked for deleting
			for (auto it = behaviorToRemove.begin(); it != behaviorToRemove.end(); ++it){
				if ((*it).first->GetId() == beh->GetId()) return true;
			}

			behaviorToRemove.push_back(std::make_pair(*found, erase));
		}
	}
	return result;
}

bool GNode::RemoveAttr(int key, bool erase){
	
	map<int, GAttr*>::iterator it = attributes.find(key);

	if (it != attributes.end()){
		GAttr* attr = it->second;
		attributes.erase(it);
		if(erase) delete attr;
		return true;
	}
	return false;
}

bool GNode::HasAttr(int key) const{
	return attributes.find(key) != attributes.end();
}


bool GNode::AddChild(GNode* child, bool immediately){

	auto existing = std::find(children.begin(), children.end(), child);
	if (existing != children.end()){
		COGLogError("Attempt to add already existing child: %s into %s", child->tag->c_str(), tag->c_str());
		return false;
	}

	if (immediately){
		children.push_back(child);
		child->parent = this;
		COGAddNode(child);
	}
	else{
		childrenToAdd.push_back(child);
	}
	return true;
}

bool GNode::RemoveChild(GNode* child, bool immediately, bool erase){
	auto found = find(children.begin(), children.end(), child);

	bool result = children.end() != found;
	if (result){
		if (immediately){
			children.remove(child);
			COGRemoveNode(child);
			if(erase) delete (child);
		}
		else{
			// check if there isn't already such child marked for deleting
			for (auto it = childrenToRemove.begin(); it != childrenToRemove.end(); ++it){
				if ((*it).first->GetId() == child->GetId()) return true;
			}
		
			childrenToRemove.push_back(std::make_pair(child, erase));
		}
	}
	return result;
}

bool GNode::RemoveFromParent(bool immediately, bool erase){
	GNode* parent = GetParent();

	if (parent != nullptr){
		parent->RemoveChild(this, immediately, erase);
		return true;
	}
	return false;
}


GNode* GNode::FindPredecessor(ObjType type){
	GNode* parent = this->parent;

	while (parent != nullptr && parent->type != type) parent = parent->parent;
	return parent;
}



string GNode::GetInfo(bool includeChildren, bool includeAttributes){
	std::ostringstream ss;
	GetInfo(includeChildren,includeAttributes, ss, 0);
	std::cout << ss.str() << std::endl;
	return ss.str();
}


void GNode::GetInfo(bool includeChildren, bool includeAttributes, std::ostringstream& ss, int level){
	spaces(level * 4, ss);
	ss << "--" << (*tag) << " " << subType << std::endl;
	
	for (auto it = behaviors.begin(); it != behaviors.end(); ++it){
		GBehavior* beh = (*it);
		spaces(level * 4 + 2, ss);
		ss << "beh " << typeid(*beh).name() << std::endl;
	}

	for (auto it = attributes.begin(); it != attributes.end(); ++it){
		spaces(level * 4 + 2, ss);
		int key = (*it).first;
		ss << "att " << Attrs::ToString(key) << std::endl;
	}

	if (states != nullptr){
		vector<int> states = this->states->GetAllStates();
		for (auto it = states.begin(); it != states.end(); ++it){
			spaces(level * 4 + 2, ss);
			int state = (*it);
			ss << "st  " << States::ToString(state) << std::endl;
		}
	}

	ss << std::endl;

	if (includeChildren){
		// write info 
		for (auto it = children.begin(); it != children.end(); ++it){
			(*it)->GetInfo(includeChildren, includeAttributes, ss, level + 1);
		}
	}
}

void GNode::InsertElementsForAdding(){

	// insert behaviors
	for (auto it = behaviorsToAdd.begin(); it != behaviorsToAdd.end(); ++it){
		AddBehavior((*it), true);
	}
	behaviorsToAdd.clear();

	// insert children
	for (auto it = childrenToAdd.begin(); it != childrenToAdd.end(); ++it){
		AddChild((*it), true);
	}
	childrenToAdd.clear();
}

void GNode::DeleteElementsForRemoving(){

	// delete behaviors
	for (auto it = behaviorToRemove.begin(); it != behaviorToRemove.end(); ++it){
		std::pair<GBehavior*, bool> item = (*it);
		GBehavior* beh = item.first;
		behaviors.remove(beh);
		COGRemoveBehavior(beh);
		beh->owner = nullptr;
		// item.second holds ERASE indicator
		if (item.second) delete item.first;
	}

	behaviorToRemove.clear();

	// delete children
	for (auto it = childrenToRemove.begin(); it != childrenToRemove.end(); ++it){
		std::pair<GNode*, bool> item = (*it);
		GNode* child = item.first;
		children.remove(child);
		COGRemoveNode(child);
		// item.second holds ERASE indicator
		if (item.second) delete item.first;
	}

	childrenToRemove.clear();
}
