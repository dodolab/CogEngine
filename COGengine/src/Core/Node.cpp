#include "Node.h"
#include "Behavior.h"
#include "ResourcesMgr.h"
#include "ofxCogEngine.h"
#include "Scene.h"

namespace Cog {

	// first id is always 1
	int Node::idCounter = 1;

	Node::Node(string tag) : type(NODETYPE_OBJECT), networkId(0), id(idCounter++), transform(0, 0) {
		if (!tag.empty()) SetTag(tag);
	}

	Node::Node(string tag, spt<Mesh> mesh) : Node(tag) {
		this->mesh = mesh;
	}

	Node::Node(stenum type, int networkId, string tag) : type(type), networkId(networkId), id(idCounter++), transform(0, 0) {
		if (!tag.empty()) SetTag(tag);
	}

	Node::Node(const Node& copy) : type(copy.type), networkId(copy.networkId), id(idCounter++), transform(0, 0) {
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

		COGLOGDEBUG("Node", "Destructing node %s", tag.c_str());
		// move elements from collection to insert so they can be removed from classic collections
		InsertElementsForAdding(false, false);


		// delete all behaviors
		for (auto& beh : behaviors) {
			if (scene != nullptr) {
				this->scene->RemoveBehavior(beh);
			}
			if (!beh->IsExternal()) {
				delete beh;
			}
		}
		behaviors.clear();


		// delete all children
		for (auto it = children.begin(); it != children.end(); ++it) {
			if (scene != nullptr) {
				scene->RemoveNode(*it);
			}

			if (!(*it)->IsExternal()) {
				delete (*it);
			}
		}
		children.clear();

		for (auto childToAdd : childrenToAdd) {
			if (!childToAdd->IsExternal()) {
				delete childToAdd;
			}
		}

		for (auto childToRemove : childrenToRemove) {
			if (!childToRemove.first->IsExternal()) {
				delete childToRemove.first;
			}
		}

		// delete not shared attributes
		for (auto attr : attributes) {
			if (!attr.second->IsShared()) {
				delete attr.second;
			}
		}

		delete groups;
		delete states;

	}

	void Node::SetImageMesh(string path) {
		this->mesh = spt<Image>(new Image(CogPreload2DImage(path)));
	}

	void Node::UpdateTransform(bool deep) {

		COGMEASURE_BEGIN("NODE_UPDATE_TRANSFORM");

		if (parent != nullptr) {
			this->transform.CalcAbsTransform(parent->transform);
		}
		else this->transform.SetAbsAsLocal();

		this->mesh->UpdateBoundingBox(transform);

		COGMEASURE_END("NODE_UPDATE_TRANSFORM");

		if (deep) {
			for (auto it = children.begin(); it != children.end(); ++it) {
				(*it)->UpdateTransform(true);
			}
		}
	}


	void Node::Update(const uint64 delta, const uint64 absolute) {

		if (runMode == RUNMODE_PAUSED_ALL || runMode == RUNMODE_DISABLED) return;

		if (runMode != RUNMODE_PAUSED_ITSELF) {

			COGMEASURE_BEGIN("NODE_UPDATE_BEHAVIORS");
			// update behaviors
			for (auto it = behaviors.begin(); it != behaviors.end(); ++it) {
				Behavior* beh = *it;
				if (!beh->HasFinished() && (beh->GetComponentState() == ComponentState::ACTIVE_ALL
					|| beh->GetComponentState() == ComponentState::ACTIVE_UPDATES)) {
					beh->Update(delta, absolute);
				}
			}

			COGMEASURE_END("NODE_UPDATE_BEHAVIORS");
		}

		if (runMode != RUNMODE_PAUSED_CHILDREN) {
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

		if (runMode == RUNMODE_INVISIBLE || runMode == RUNMODE_DISABLED) return;

		if (this->IsRenderable()) {
			// push node into renderer component
			CogPushNodeForRendering(this);
		}

		for (auto it = children.begin(); it != children.end(); ++it) {
			(*it)->Draw(delta, absolute);
		}
	}

	bool Node::AddBehavior(Behavior* beh) {
		COGLOGDEBUG("Node", "Adding behavior %s into node %d (%s)", typeid(*beh).name(), id, tag.c_str());
		behaviorsToAdd.push_back(beh);

		return true;
	}

	bool Node::RemoveBehavior(Behavior* beh, bool erase) {
		auto found = find(behaviors.begin(), behaviors.end(), beh);

		bool result = behaviors.end() != found;
		if (result) {
			COGLOGDEBUG("Node", "Removing behavior %s from node %d (%s)", typeid(*beh).name(), id, tag.c_str());
			// behavior found
			// check if there isn't already such behavior marked for deleting
			for (auto it = behaviorToRemove.begin(); it != behaviorToRemove.end(); ++it) {
				if ((*it).first->GetId() == beh->GetId()) return true;
			}

			behaviorToRemove.push_back(std::make_pair(*found, erase));
		}
		return result;
	}

	bool Node::RemoveAttr(StrId key, bool erase) {
		map<StrId, Attr*>::iterator it = attributes.find(key);

		if (it != attributes.end()) {
			Attr* attr = it->second;
			attributes.erase(it);
			SendMessage(ACT_ATTR_CHANGED, new AttributeChangeEvent(key, AttrChangeType::REMOVE));

			if (erase) delete attr;
			return true;
		}
		return false;
	}

	bool Node::HasAttr(StrId key) const {
		return attributes.count(key) != 0;
	}


	void Node::SubmitChanges(bool applyToChildren) {
		InsertElementsForAdding(applyToChildren, true);
		DeleteElementsForRemoving(applyToChildren);
	}

	bool Node::AddChild(Node* child) {

		COGLOGDEBUG("Node", "Adding child %d (%s) into node %d (%s)", child->id, child->tag.c_str(), id, tag.c_str());
		auto existing = std::find(children.begin(), children.end(), child);
		if (existing != children.end()) {
			CogLogError("Attempt to add already existing child: %s into %s", child->tag.c_str(), tag.c_str());
			return false;
		}

		childrenToAdd.push_back(child);

		return true;
	}

	bool Node::RemoveChild(Node* child, bool erase) {
		auto found = find(children.begin(), children.end(), child);

		bool result = children.end() != found;
		if (result) {
			COGLOGDEBUG("Node", "Removing child %d (%s) from node %d (%s)", child->id, child->tag.c_str(), id, tag.c_str());
			// check if there isn't already such child marked for deleting
			for (auto it = childrenToRemove.begin(); it != childrenToRemove.end(); ++it) {
				if ((*it).first->GetId() == child->GetId()) return true;
			}
			childrenToRemove.push_back(std::make_pair(child, erase));

		}
		return result;
	}

	void Node::RemoveAllChildren(bool erase) {
		for (auto child : children) {
			RemoveChild(child, erase);
		}
	}


	bool Node::RemoveFromParent(bool erase) {
		Node* parent = GetParent();

		if (parent != nullptr) {
			parent->RemoveChild(this, erase);
			return true;
		}
		return false;
	}


	Node* Node::FindPredecessor(stenum type) const {
		Node* parent = this->parent;

		while (parent != nullptr && parent->type != type) parent = parent->parent;
		return parent;
	}

	void Node::SetState(unsigned state) {
		GetStates().SetState(state);
		SendMessage(ACT_STATE_CHANGED, new FlagChangeEvent(FlagChangeType::SET, state));
	}

	void Node::ResetState(unsigned state) {
		GetStates().ResetState(state);
		SendMessage(ACT_STATE_CHANGED, new FlagChangeEvent(FlagChangeType::RESET, state));
	}

	void Node::SwitchState(unsigned state1, unsigned state2) {
		GetStates().SwitchState(state1, state2);
		SendMessage(ACT_STATE_CHANGED, new FlagChangeEvent(FlagChangeType::SWITCH, state1, state2));
	}

	void* Node::GetAttrPtr(StrId key) {
		auto it = attributes.find(key);
		if (it != attributes.end()) {
			if (it->second->IsPointer()) {
				// address of a pointer
				return *(void**)(it->second->RawVal());
			}
			else {
				// address of a value
				return it->second->RawVal();
			}
		}
		return nullptr;
	}


	void Node::InsertElementsForAdding(bool applyToChildren, bool init) {

		// insert children
		for (auto it = childrenToAdd.begin(); it != childrenToAdd.end(); ++it) {
			Node* child = (*it);

			if (this->type == NODETYPE_ROOT && child->scene->sceneType != SceneType::DIALOG) {
				children.push_front(child); // scenes always on front
			}
			else {
				children.push_back(child);
			}

			child->parent = this;

			// root has no scene
			if (this->type != NODETYPE_ROOT) {
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

			for (auto jt = behaviors.begin(); jt != behaviors.end(); ++jt) {
				if (typeid(*(*jt)) == typeid(*beh)) {
					behaviors.erase(jt);
					break;
				}
			}

			beh->owner = this;
			behaviors.push_back(beh);

			// root node can't have its behaviors stored in the scene
			if (this->type != NODETYPE_ROOT) {
				scene->AddBehavior(beh);
			}

			if (init) {
				// initialize behavior
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
			if (scene != nullptr) scene->RemoveBehavior(beh);
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
			if (scene != nullptr) scene->RemoveNode(child);
			if (applyToChildren) child->DeleteElementsForRemoving(true);
			// item.second holds ERASE indicator
			if (item.second) delete item.first;
		}

		childrenToRemove.clear();
	}

	void Node::SendMessage(StrId action, RefCountedObjectPtr<MsgPayload> data) {
		if (scene != nullptr) {
			auto msg = Msg(action, MsgObjectType::NODE_ACTUAL, this->id, MsgObjectType::SUBSCRIBERS, this, data);
			scene->SendMessage(msg);
		}
	}

	void Node::WriteInfo(int logLevel) {
		CogLogTree("INFO_NODE", logLevel, "Node %s (%d)", this->tag.c_str(), this->id);

#if _DEBUG

		if (attributes.size() > 0) CogLogTree("INFO_NODE", logLevel + 1, "Attributes:");

		for (auto it = attributes.begin(); it != attributes.end(); ++it) {
			StrId key = (*it).first;
			CogLogTree("INFO_NODE", logLevel + 2, key.GetStringValue().c_str());
		}
#endif

		CogLogTree("INFO_TRANSFORM", logLevel + 1, "Transform: ");
		CogLogTree("INFO_TRANSFORM", logLevel + 2, "LPos: [%f,%f]", transform.localPos.x, transform.localPos.y);
		CogLogTree("INFO_TRANSFORM", logLevel + 2, "LScal: [%f,%f]", transform.scale.x, transform.scale.y);
		CogLogTree("INFO_TRANSFORM", logLevel + 2, "ZIndex: %d", transform.scale.z);
		CogLogTree("INFO_TRANSFORM", logLevel + 2, "Rotation: %f", transform.rotation);
		CogLogTree("INFO_TRANSFORM", logLevel + 2, "AbsPos: [%f,%f]", transform.absPos.x, transform.absPos.y);
		CogLogTree("INFO_TRANSFORM", logLevel + 2, "AbsScal: [%f,%f]", transform.absScale.x, transform.absScale.y);
		CogLogTree("INFO_TRANSFORM", logLevel + 2, "AbsRotation: %f", transform.absRotation);

		CogLogTree("INFO_SHAPE", logLevel + 1, "Shape: %s", typeid(*this->mesh).name());
		CogLogTree("INFO_SHAPE", logLevel + 2, "Size: [%f x %f]", this->mesh->GetWidth(), this->mesh->GetHeight());

		if (states != nullptr) {
			vector<unsigned> allStates = states->GetAllStates();
			if (allStates.size() > 0) {
				CogLogTree("INFO_FLAGS", logLevel + 1, "Flags: %d", allStates.size());

				for (unsigned un : allStates) {
					CogLogTree("INFO_FLAGS", logLevel + 2, StrId(un).GetStringValue().c_str());
				}
			}
		}

		if (behaviors.size() > 0) {
			CogLogTree("INFO_BEHAVIOR", logLevel + 1, "Behaviors: %d", behaviors.size());

			for (Behavior* beh : behaviors) {
				CogLogTree("INFO_BEHAVIOR", logLevel + 2, typeid(*beh).name());
			}
		}

		if (children.size() > 0) {
			CogLogTree("INFO_NODE_CHILDREN", logLevel + 1, "Children: %d", children.size());
			for (Node* child : children) {
				child->WriteInfo(logLevel + 3);
			}
		}

	}

}// namespace