#include "Node.h"
#include "Behavior.h"
#include "ResourceCache.h"
#include "CogEngine.h"
#include "Scene.h"

namespace Cog {

	// first id is always 1
	int Node::idCounter = 1;

	Node::Node(string tag) : type(NodeType::OBJECT), subType(0), id(idCounter++), transform(0, 0) {
		if (!tag.empty()) SetTag(tag);
	}

	Node::Node(NodeType type, int subType, string tag) : type(type), subType(subType), id(idCounter++), transform(0, 0) {
		if (!tag.empty()) SetTag(tag);
	}

	Node::Node(const Node& copy) : type(copy.type), subType(copy.subType), id(idCounter++), transform(0, 0) {
		tag = copy.tag;
		attributes = copy.attributes;
		behaviors = copy.behaviors;
		parent = copy.parent;
		groups = copy.groups;
		states = copy.states;
		transform = copy.transform;
		runMode = copy.runMode;
	}

	Node::~Node() {

		COGLOGDEBUG("GNODE", "Destructing node %s", tag == nullptr ? "<noname>" : tag->c_str());
		// move elements from collection to insert so they can be removed from classic collections
		InsertElementsForAdding(false, false);


			// delete all behaviors
		for (list<Behavior*>::iterator it = behaviors.begin(); it != behaviors.end(); ++it)
		{
			if (scene != nullptr) {
				this->scene->RemoveBehavior(*it);
			}
			if (!(*it)->IsExternal()) {
				delete (*it);
			}
		}
		behaviors.clear();

		/* don't delete attributes
		// delete attributes
		for (map<int, Attr*>::iterator it = attributes.begin(); it != attributes.end(); ++it){
		delete (it->second);
		}*/

		// root node doesn't deallocate its children
		if (this->type != NodeType::ROOT) {
			// delete all children
			for (auto it = children.begin(); it != children.end(); ++it)
			{
				if (scene != nullptr) {
					scene->RemoveNode(*it);
				}

				if (!(*it)->IsExternal()) {
					delete (*it);
				}
			}
			children.clear();
		}

		delete tag;
		delete groups;
		delete states;
	}

	void Node::UpdateTransform(bool deep) {

		COGMEASURE_BEGIN("NOTE_UPDATE_TRANSFORM");

		if (parent != nullptr) {
			this->transform.CalcAbsTransform(parent->transform);
		}
		else this->transform.SetAbsAsLocal();

		COGMEASURE_END("NOTE_UPDATE_TRANSFORM");

		if (deep) {
			for (auto it = children.begin(); it != children.end(); ++it) {
				(*it)->UpdateTransform(true);
			}
		}
	}


	void Node::Update(const uint64 delta, const uint64 absolute) {

		if (runMode == RunningMode::PAUSED_ALL || runMode == RunningMode::DISABLED) return;

		if (runMode != RunningMode::PAUSED_ITSELF) {

			COGMEASURE_BEGIN("NODE_UPDATE_BEHAVIORS");
			// update behaviors
			for (auto it = behaviors.begin(); it != behaviors.end(); ++it) {
				Behavior* beh = *it;
				if (!beh->IsFinished() && (beh->GetListenerState() == ListenerState::ACTIVE_ALL
					|| beh->GetListenerState() == ListenerState::ACTIVE_UPDATES)) {
					beh->Update(delta, absolute);
				}
			}

			COGMEASURE_END("NODE_UPDATE_BEHAVIORS");
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

		DeleteElementsForRemoving(false);
		InsertElementsForAdding(false, true);
	}

	void Node::Draw(const uint64 delta, const uint64 absolute) {

		if (runMode == RunningMode::INVISIBLE || runMode == RunningMode::DISABLED) return;

		if (this->IsRenderable()) {
			CogPushNodeForRendering(this);
		}

		for (auto it = children.begin(); it != children.end(); ++it) {
			// draw children
			(*it)->Draw(delta, absolute);
		}
	}

	bool Node::AddBehavior(Behavior* beh) {

		COGLOGDEBUG("Node", "Adding behavior %s into node %d (%s)", typeid(*beh).name(), id, tag != nullptr ? tag->c_str() : "<noname>");
		behaviorsToAdd.push_back(beh);

		return true;
	}

	bool Node::RemoveBehavior(Behavior* beh, bool erase) {
		auto found = find(behaviors.begin(), behaviors.end(), beh);

		bool result = behaviors.end() != found;
		if (result) {
			COGLOGDEBUG("Node", "Removing behavior %s from node %d (%s)", typeid(*beh).name(), id, tag != nullptr ? tag->c_str() : "<noname>");
			// behavior found
			// check if there isn't already such behavior marked for deleting
			for (auto it = behaviorToRemove.begin(); it != behaviorToRemove.end(); ++it) {
				if ((*it).first->GetId() == beh->GetId()) return true;
			}

			behaviorToRemove.push_back(std::make_pair(*found, erase));

		}
		return result;
	}

	bool Node::RemoveAttr(StringHash key, bool erase) {

		map<StringHash, Attr*>::iterator it = attributes.find(key);

		if (it != attributes.end()) {
			Attr* attr = it->second;
			attributes.erase(it);

			CogSendMessageToListeners(ACT_ATTR_CHANGED, 0, new AttributeChangeEvent(key, AttrChange::REMOVE), this, -1);

			if (erase) delete attr;
			return true;
		}
		return false;
	}

	bool Node::HasAttr(StringHash key) const {
		return attributes.find(key) != attributes.end();
	}


	void Node::SubmitChanges(bool applyToChildren) {
		InsertElementsForAdding(applyToChildren, true);
		DeleteElementsForRemoving(applyToChildren);
	}


	bool Node::AddChild(Node* child) {

		COGLOGDEBUG("Node", "Adding child %d (%s) into node %d (%s)",child->id, child->tag != nullptr ? 
			child->tag->c_str() : "<noname>", id, tag != nullptr ? tag->c_str() : "<noname>");
		auto existing = std::find(children.begin(), children.end(), child);
		if (existing != children.end()) {
			CogLogError("Attempt to add already existing child: %s into %s", child->tag->c_str(), tag->c_str());
			return false;
		}

		childrenToAdd.push_back(child);

		return true;
	}

	bool Node::RemoveChild(Node* child, bool erase) {
		auto found = find(children.begin(), children.end(), child);

		bool result = children.end() != found;
		if (result) {
			COGLOGDEBUG("Node", "Removing child %d (%s) from node %d (%s)", child->id, child->tag != nullptr ? 
				child->tag->c_str() : "<noname>", id, tag != nullptr ? tag->c_str() : "<noname>");
			// check if there isn't already such child marked for deleting
			for (auto it = childrenToRemove.begin(); it != childrenToRemove.end(); ++it) {
				if ((*it).first->GetId() == child->GetId()) return true;
			}

			childrenToRemove.push_back(std::make_pair(child, erase));

		}
		return result;
	}

	bool Node::RemoveFromParent(bool erase) {
		Node* parent = GetParent();

		if (parent != nullptr) {
			parent->RemoveChild(this, erase);
			return true;
		}
		return false;
	}


	Node* Node::FindPredecessor(NodeType type) {
		Node* parent = this->parent;

		while (parent != nullptr && parent->type != type) parent = parent->parent;
		return parent;
	}


	void Node::InsertElementsForAdding(bool applyToChildren, bool init) {

		// insert children
		for (auto it = childrenToAdd.begin(); it != childrenToAdd.end(); ++it) {
			Node* child = (*it);
			children.push_back(child);
			child->parent = this;
			
			// root has no scene
			if (this->type != NodeType::ROOT) {
				child->scene = this->scene;
				scene->AddNode(child);
			}
			else {
				child->GetScene()->AddNode(child);
			}

		}

		// initialize when all children are loaded
		if (applyToChildren) {
			for (auto& child : childrenToAdd) {
				child->InsertElementsForAdding(true, init);
			}
		}

		childrenToAdd.clear();

		// insert behaviors
		for (auto it = behaviorsToAdd.begin(); it != behaviorsToAdd.end(); ++it) {
			Behavior* beh = (*it);

			if (beh->GetMaxCount() != INT_MAX) {
				// check the number of already stored behavior
				int stored = 0;
				for (auto cBeh : behaviors) {
					if (typeid(*cBeh).name() == typeid(*beh).name()) stored++;
				}
				if (stored >= beh->GetMaxCount()) {
					CogLogError("Node", "Attempt to add more behaviors of type %s than allowed; max number is %d", beh->GetClassName().c_str(), beh->GetMaxCount());
					continue;
				}
			}

			beh->owner = this;
			behaviors.push_back(beh);

			// todo: root node can't have its behaviors stored in the scene
			if (this->type != NodeType::ROOT) {
				scene->AddBehavior(beh);
			}

			if (init) {
				// initialize
				beh->Init();
				beh->Start();
			}
		}
		behaviorsToAdd.clear();
	}

	void Node::DeleteElementsForRemoving(bool applyToChildren) {

		// delete behaviors
		for (auto it = behaviorToRemove.begin(); it != behaviorToRemove.end(); ++it) {
			std::pair<Behavior*, bool> item = (*it);
			Behavior* beh = item.first;
			behaviors.remove(beh);
			scene->RemoveBehavior(beh);
			beh->owner = nullptr;
			// item.second holds ERASE indicator
			if (item.second) delete item.first;
		}

		behaviorToRemove.clear();

		// delete children
		for (auto it = childrenToRemove.begin(); it != childrenToRemove.end(); ++it) {
			std::pair<Node*, bool> item = (*it);
			Node* child = item.first;
			children.remove(child);
			scene->RemoveNode(child);
			if (applyToChildren) child->DeleteElementsForRemoving(true);
			// item.second holds ERASE indicator
			if (item.second) delete item.first;
		}

		childrenToRemove.clear();
	}

	void Node::WriteInfo(int logLevel) {
		CogLogTree("INFO_NODE", logLevel, "Node %s (%d)", this->tag != nullptr ? this->tag->c_str() : "<noname>", this->id);

#if DEBUG

		if(attributes.size() > 0) CogLogTree("INFO_NODE", logLevel+1, "Attributes:");

		for (auto it = attributes.begin(); it != attributes.end(); ++it) {
			StringHash key = (*it).first;
			CogLogTree("INFO_NODE", logLevel + 2, StringHash::GetStringValue(key).c_str());
		}
#endif

		CogLogTree("INFO_TRANSFORM", logLevel+1, "Transform: ");
		CogLogTree("INFO_TRANSFORM", logLevel + 2, "LPos: [%f,%f]",transform.localPos.x, transform.localPos.y);
		CogLogTree("INFO_TRANSFORM", logLevel + 2, "LScal: [%f,%f]", transform.scale.x, transform.scale.y);
		CogLogTree("INFO_TRANSFORM", logLevel + 2, "ZIndex: %d", transform.scale.z);
		CogLogTree("INFO_TRANSFORM", logLevel + 2, "Rotation: %f", transform.rotation);
		CogLogTree("INFO_TRANSFORM", logLevel + 2, "AbsPos: [%f,%f]", transform.absPos.x, transform.absPos.y);
		CogLogTree("INFO_TRANSFORM", logLevel + 2, "AbsScal: [%f,%f]", transform.absScale.x, transform.absScale.y);
		CogLogTree("INFO_TRANSFORM", logLevel + 2, "AbsRotation: %f", transform.absRotation);

		CogLogTree("INFO_SHAPE", logLevel+1, "Shape: %s", typeid(*this->shape).name());
		CogLogTree("INFO_SHAPE", logLevel+2, "Size: [%f x %f]", this->shape->GetWidth(), this->shape->GetHeight());
		
		if (states != nullptr) {
			vector<unsigned> allStates = states->GetAllStates();
			if (allStates.size() > 0) {
				CogLogTree("INFO_FLAGS", logLevel+1, "Flags: %d", allStates.size());

				for (unsigned un : allStates) {
					CogLogTree("INFO_FLAGS", logLevel + 2, StringHash::GetStringValue(un).c_str());
				}
			}
		}

		if (behaviors.size() > 0) {
			CogLogTree("INFO_BEHAVIOR", logLevel+1, "Behaviors: %d",behaviors.size());

			for (Behavior* beh : behaviors) {
				CogLogTree("INFO_BEHAVIOR", logLevel + 2, typeid(*beh).name());
			}
		}

		if (children.size() > 0) {
			CogLogTree("INFO_NODE_CHILDREN", logLevel+1, "Children: %d", children.size());
			for (Node* child : children) {
				child->WriteInfo(logLevel + 3);
			}
		}

	}

}// namespace