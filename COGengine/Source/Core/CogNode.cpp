#include "CogNode.h"
#include "CogBehavior.h"
#include "CogCache.h"
#include "CogEngine.h"

namespace Cog {

	// first id is always 1
	int CogNode::idCounter = 1;

	CogNode::CogNode(string tag) : type(CogObjType::OBJECT), subType(0), id(idCounter++), transform(0, 0) {
		Construct();
		if (!tag.empty()) SetTag(tag);
	}

	CogNode::CogNode(CogObjType type, int subType, string tag) : type(type), subType(subType), id(idCounter++), transform(0, 0) {
		Construct();
		if (!tag.empty()) SetTag(tag);
	}

	CogNode::CogNode(const CogNode& copy) : type(copy.type), subType(copy.subType), id(idCounter++), transform(0, 0) {
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

	CogNode::~CogNode() {

		MLOGDEBUG("GNODE", "Destructing node %s", tag->c_str());
		// move elements from collection to insert so they can be removed from classic collections
		InsertElementsForAdding();

		// delete all behaviors
		for (list<CogBehavior*>::iterator it = behaviors.begin(); it != behaviors.end(); ++it)
		{
			CogRemoveBehavior((*it));
			delete (*it);
		}
		behaviors.clear();

		/* don't delete attributes
		// delete attributes
		for (map<int, Attr*>::iterator it = attributes.begin(); it != attributes.end(); ++it){
		delete (it->second);
		}*/

		// delete all children
		for (auto it = children.begin(); it != children.end(); ++it)
		{
			CogRemoveNode((*it));
			delete (*it);
		}
		children.clear();

		delete tag;
		delete groups;
		delete states;
	}

	void CogNode::UpdateTransform(bool deep) {

		if (parent != nullptr) {
			this->transform.CalcAbsTransform(parent->transform);
		}
		else this->transform.SetAbsAsLocal();

		if (deep) {
			for (auto it = children.begin(); it != children.end(); ++it) {
				(*it)->UpdateTransform(true);
			}
		}
	}


	void CogNode::Update(const uint64 delta, const uint64 absolute) {

		if (runMode == RunningMode::PAUSED_ALL || runMode == RunningMode::DISABLED) return;

		if (runMode != RunningMode::PAUSED_ITSELF) {
			// update behaviors
			for (auto it = behaviors.begin(); it != behaviors.end(); ++it) {
				CogBehavior* beh = *it;
				if (!beh->ended && (beh->GetBehState() == CogBehState::ACTIVE_ALL
					|| beh->GetBehState() == CogBehState::ACTIVE_UPDATES)) {
					beh->Update(delta, absolute);
				}
			}
		}

		if (runMode != RunningMode::PAUSED_CHILDREN) {
			// update children
			int childrenCount = children.size();
			int childrenCounter = 0;

			for (auto it = children.begin(); it != children.end(); ++it) {
				// prevent for while-update adding children
				if (childrenCounter++ < childrenCount) {
					(*it)->Update(delta, absolute);
				}
			}
		}

		DeleteElementsForRemoving();
		InsertElementsForAdding();
	}

	void CogNode::Draw(const uint64 delta, const uint64 absolute) {

		if (runMode == RunningMode::INVISIBLE || runMode == RunningMode::DISABLED) return;

		if (this->IsRenderable()) {
			CogPushNodeForRendering(this);
		}

		for (auto it = children.begin(); it != children.end(); ++it) {
			// draw children
			(*it)->Draw(delta, absolute);
		}
	}

	bool CogNode::AddBehavior(CogBehavior* beh) {
		behaviorsToAdd.push_back(beh);

		return true;
	}

	bool CogNode::RemoveBehavior(CogBehavior* beh, bool erase) {
		auto found = find(behaviors.begin(), behaviors.end(), beh);

		bool result = behaviors.end() != found;
		if (result) {
			// behavior found
			// check if there isn't already such behavior marked for deleting
			for (auto it = behaviorToRemove.begin(); it != behaviorToRemove.end(); ++it) {
				if ((*it).first->GetId() == beh->GetId()) return true;
			}

			behaviorToRemove.push_back(std::make_pair(*found, erase));

		}
		return result;
	}

	bool CogNode::RemoveAttr(int key, bool erase) {

		map<int, Attr*>::iterator it = attributes.find(key);

		if (it != attributes.end()) {
			Attr* attr = it->second;
			attributes.erase(it);
			if (erase) delete attr;
			return true;
		}
		return false;
	}

	bool CogNode::HasAttr(int key) const {
		return attributes.find(key) != attributes.end();
	}


	void CogNode::SubmitChanges(bool applyToChildren) {
		InsertElementsForAdding();
		DeleteElementsForRemoving();

		if (applyToChildren) {
			for (auto it = children.begin(); it != children.end(); ++it) {
				CogNode* child = (*it);
				child->SubmitChanges(true);
			}
		}
	}


	bool CogNode::AddChild(CogNode* child) {

		auto existing = std::find(children.begin(), children.end(), child);
		if (existing != children.end()) {
			CogLogError("Attempt to add already existing child: %s into %s", child->tag->c_str(), tag->c_str());
			return false;
		}

		childrenToAdd.push_back(child);

		return true;
	}

	bool CogNode::RemoveChild(CogNode* child, bool erase) {
		auto found = find(children.begin(), children.end(), child);

		bool result = children.end() != found;
		if (result) {
			// check if there isn't already such child marked for deleting
			for (auto it = childrenToRemove.begin(); it != childrenToRemove.end(); ++it) {
				if ((*it).first->GetId() == child->GetId()) return true;
			}

			childrenToRemove.push_back(std::make_pair(child, erase));

		}
		return result;
	}

	bool CogNode::RemoveFromParent(bool erase) {
		CogNode* parent = GetParent();

		if (parent != nullptr) {
			parent->RemoveChild(this, erase);
			return true;
		}
		return false;
	}


	CogNode* CogNode::FindPredecessor(CogObjType type) {
		CogNode* parent = this->parent;

		while (parent != nullptr && parent->type != type) parent = parent->parent;
		return parent;
	}



	string CogNode::GetInfo(bool includeChildren, bool includeAttributes) {
		std::ostringstream ss;
		GetInfo(includeChildren, includeAttributes, ss, 0);
		std::cout << ss.str() << std::endl;
		return ss.str();
	}


	void CogNode::GetInfo(bool includeChildren, bool includeAttributes, std::ostringstream& ss, int level) {
		spaces(level * 4, ss);
		ss << "--" << (*tag) << " " << subType << std::endl;

		for (auto it = behaviors.begin(); it != behaviors.end(); ++it) {
			CogBehavior* beh = (*it);
			spaces(level * 4 + 2, ss);
			ss << "beh " << typeid(*beh).name() << std::endl;
		}

		for (auto it = attributes.begin(); it != attributes.end(); ++it) {
			spaces(level * 4 + 2, ss);
			int key = (*it).first;
			ss << "att " << ofToString(key) << std::endl;
		}

		if (states != nullptr) {
			vector<int> states = this->states->GetAllStates();
			for (auto it = states.begin(); it != states.end(); ++it) {
				spaces(level * 4 + 2, ss);
				int state = (*it);
				ss << "st  " << ofToString(state) << std::endl;
			}
		}

		ss << std::endl;

		if (includeChildren) {
			// write info 
			for (auto it = children.begin(); it != children.end(); ++it) {
				(*it)->GetInfo(includeChildren, includeAttributes, ss, level + 1);
			}
		}
	}

	void CogNode::InsertElementsForAdding() {

		// insert behaviors
		for (auto it = behaviorsToAdd.begin(); it != behaviorsToAdd.end(); ++it) {
			CogBehavior* beh = (*it);
			beh->owner = this;
			behaviors.push_back(beh);
			CogAddBehavior(beh);
			// initialize
			beh->Init();
		}
		behaviorsToAdd.clear();

		// insert children
		for (auto it = childrenToAdd.begin(); it != childrenToAdd.end(); ++it) {
			CogNode* child = (*it);
			children.push_back(child);
			child->parent = this;
			CogAddNode(child);
		}
		childrenToAdd.clear();
	}

	void CogNode::DeleteElementsForRemoving() {

		// delete behaviors
		for (auto it = behaviorToRemove.begin(); it != behaviorToRemove.end(); ++it) {
			std::pair<CogBehavior*, bool> item = (*it);
			CogBehavior* beh = item.first;
			behaviors.remove(beh);
			CogRemoveBehavior(beh);
			beh->owner = nullptr;
			// item.second holds ERASE indicator
			if (item.second) delete item.first;
		}

		behaviorToRemove.clear();

		// delete children
		for (auto it = childrenToRemove.begin(); it != childrenToRemove.end(); ++it) {
			std::pair<CogNode*, bool> item = (*it);
			CogNode* child = item.first;
			children.remove(child);
			CogRemoveNode(child);
			// item.second holds ERASE indicator
			if (item.second) delete item.first;
		}

		childrenToRemove.clear();
	}

}