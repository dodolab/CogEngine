#pragma once

#include "ofxCogCommon.h"
#include "Attr.h"
#include "Behavior.h"
#include "Msg.h"
#include "Facade.h"
#include "Shape.h"
#include "Events.h"

namespace Cog {

	class Scene;

	/*! Node running mode */
	enum RunningMode {
		RUNNING,			/*!< active for update and draw */
		PAUSED_ALL,			/*!< active for draw */
		PAUSED_CHILDREN,	/*!< active itself but children are inactive */
		PAUSED_ITSELF,		/*!< inactive but children are active */
		INVISIBLE,			/*!< active for update but inactive for draw */
		DISABLED			/*!< inactive for both, update and draw */
	};

	/*! Node type enumerator */
	enum class NodeType {
		ROOT,				/*!< root object, usually the topmost parent */
		SCENE,				/*!< scene root object, topmost parent in the scene */
		OBJECT,				/*!< common node  */
		HUD,				/*!< human interface object */
		INFO				/*!< info object */
	};

	/**
	* Node with attributes, behaviors, states and transformation matrix
	*
	*/
	class Node {
	protected:
		// identifier incremental counter
		static int idCounter;
		// attributes, mapped by their keys
		map<StringHash, Attr*> attributes;
		// list of childrens
		list<Node*> children;
		// list of behaviors
		list<Behavior*> behaviors;
		// link to the parent
		Node* parent = nullptr;
		// link to the scene
		Scene* scene = nullptr;
		// list of children that will be removed at the end of update method
		list<std::pair<Node*, bool>> childrenToRemove;
		// list of behaviors that will be removed at the end of update method
		list<std::pair<Behavior*, bool>> behaviorToRemove;
		// list of children that will be added at the end of update method
		list<Node*> childrenToAdd;
		// list of behaviors that will be added at the end of update method
		list<Behavior*> behaviorsToAdd;

		// id of this node
		const int id;
		// tag or name
		string* tag = nullptr;
		// object type {ROOT, SCENE, OBJECT, HUD, INFO}
		NodeType type;
		// subtype (or category)
		int subType;
		// groups this objects belongs to
		Flags* groups = nullptr;
		// states of this object
		Flags* states = nullptr;
		// transformation matrix
		Trans  transform;
		// mash object
		spt<Shape> shape = spt<Shape>(new Shape(RenderType::NONE));
		// running mode
		RunningMode runMode = RunningMode::RUNNING;
		bool isExternal = false;

	public:

		/**
		* Creates a new node
		* @param tag tag/name
		*/
		Node(string tag);

		/**
		* Creates a new node
		* @param type object type {ROOT, SCENE, OBJECT, HUD, INFO}
		* @param subType subtype/category number
		* @param tag tag/name
		*/
		Node(NodeType type, int subType, string tag);

		Node(const Node& copy);

		~Node();

		/**
		* Updates absolute transformations according to the parent
		* @param depp if true, children will be updated as well
		*/
		void UpdateTransform(bool deep);

		/**
		* Updates all behaviors in the subtree
		* @param delta delta time from the last loop
		* @param absolute absolute time since the application started
		*/
		void Update(const uint64 delta, const uint64 absolute);

		/**
		* Draws all objects, calling VIEW behaviors
		* @param delta delta time from the last loop
		* @param absolute absolute time since the app begun
		*/
		void Draw(const uint64 delta, const uint64 absolute);

		/**
		* Adds a new behavior
		* @param beh behavior to add
		* @return true, if behavior has been successfully added
		*/
		bool AddBehavior(Behavior* beh);

		/**
		* Removes existing behavior (by its id)
		* @param beh behavior to remove
		* @param erase if true, memory will be released
		* @return true, if behavior has been removed
		*/
		bool RemoveBehavior(Behavior* beh, bool erase);

		/**
		* Removes existing attribute (by its key)
		* @param key key of attribute that will be removed
		* @param erase if true, memory will be released
		* @return true, if attribute has been removed
		*/
		bool RemoveAttr(StringHash key, bool erase);

		/**
		* Returns true, if this node has an attribute with given key
		*/
		bool HasAttr(StringHash key) const;

		/**
		* Updates collections of behaviors and children nodes
		* Adds elements that are supposed to be added and removes
		* elements that are supposed to be removed
		* @param applyToChildren if true, children will be updated as well
		*/
		void SubmitChanges(bool applyToChildren);

		/**
		* Gets copy of list of all behaviors
		*/
		list<Behavior*> GetBehaviorsCopy() const {
			return behaviors;
		}

		/**
		* Gets direct reference to list of all behaviors
		*/
		const list<Behavior*>& GetBehaviors() const {
			return behaviors;
		}

		/**
		* Gets copy of list of all children
		*/
		list<Node*> GetChildrenCopy() const {
			return children;
		}

		/**
		* Gets direct reference to list of all children
		*/
		const list<Node*>& GetChildren() const {
			return children;
		}

		/**
		* Adds a new child
		* @param child child to add
		* @return true, if child has been successfully added
		*/
		bool AddChild(Node* child);

		/**
		* Removes an existing child
		* @param child child to remove
		* @param erase if true, memory will be released
		* @return true, if child has been removed
		*/
		bool RemoveChild(Node* child, bool erase);

		/**
		* Removes this node from its parent (if it exists)
		* @param erase if true, memory will be released
		* @return true, if this object has been removed
		*/
		bool RemoveFromParent(bool erase);

		/**
		* Gets pointer to the parent of this node
		*/
		Node* GetParent() {
			return parent;
		}

		/**
		* Sets parent of this node
		*/
		void SetParent(Node* val) {
			parent = val;
		}

		/**
		* Finds the first predecessor of given object type
		* @param type predecessor type {ROOT, SCENE, OBJECT, HUD, INFO}
		*/
		Node* FindPredecessor(NodeType type);

		/**
		* Gets the nearest parent that is a scene root (if exists)
		*/
		Node* GetSceneRoot() {
			if (type == NodeType::SCENE) return this;
			else return FindPredecessor(NodeType::SCENE);
		}

		/**
		* Gets the root of the whole scene
		*/
		Node* GetRoot() {
			if (type == NodeType::ROOT) return this;
			else return FindPredecessor(NodeType::ROOT);
		}

		/**
		* Gets scene of this object
		*/
		Scene* GetScene() {
			return scene;
		}

		/**
		* Sets object scene
		*/
		void SetScene(Scene* scene) {
			this->scene = scene;
		}

		/**
		* Gets unique identifier of this node
		*/
		int GetId() const {
			return id;
		}

		/**
		* Gets tag/name
		*/
		string GetTag() const {
			if (tag == nullptr) return "";
			else return string(*tag);
		}

		/**
		* Sets tag/name
		*/
		void SetTag(string tag) {
			delete this->tag;
			this->tag = new string(tag);
		}

		/**
		* Gets type/category of this node {ROOT, SCENE, OBJECT, HUD, INFO}
		*/
		NodeType GetType() const {
			return this->type;
		}

		/**
		* Gets subtype/category
		*/
		int GetSubType() const {
			return subType;
		}

		/**
		* Sets subtype/category
		*/
		void SetSubType(int val) {
			this->subType = val;
		}

		/**
		* Gets transformation entity
		*/
		Trans& GetTransform() {
			return transform;
		}

		/**
		* Sets transformation entity
		*/
		void SetTransform(Trans val) {
			this->transform = val;
		}

		/**
		* Gets indicator, if this entity is renderable
		*/
		bool IsRenderable() {
			return shape->GetRenderType() != RenderType::NONE;
		}

		bool IsExternal() {
			return isExternal;
		}

		void SetIsExternal(bool ext) {
			this->isExternal = ext;
		}

		/**
		* Gets shaping object
		*/
		spt<Shape> GetShape() {
			return shape;
		}

		/**
		* Get shape of selected template; must inherit from Shape entity
		*/
		template<class T> T GetShape() {
			auto specialShape = static_cast<T>(shape);
			return specialShape;
		}

		/**
		*  Sets shaping object
		*/
		void SetShape(spt<Shape> shape) {
			this->shape = shape;
		}

		/**
		* Returns true, if the shape is of the selected type
		*/
		bool HasRenderType(RenderType type) {
			return shape->GetRenderType() == type;
		}

		/**
		* Returns true, if groups has been initialized
		* Check this indicator first before asking for groups, because
		* they are lazy initialized
		*/
		const bool HasGroups() const {
			return groups != nullptr;
		}

		/**
		* Gets flag array, representing list of all groups this object belongs to
		*/
		Flags& GetGroups() {
			if (groups == nullptr) groups = new Flags();
			return *groups;
		}

		/**
		* Sets groups this objects belongs to
		*/
		void SetGroups(Flags val) {
			delete groups;
			this->groups = new Flags(val);
		}

		/**
		* Returns true, if this object is in selected group
		*/
		bool IsInGroup(int groupId) {
			return HasGroups() && groups->HasState(groupId);
		}

		/**
		* Sets selected group
		*/
		void SetGroup(int groupId) {
			if (groups == nullptr) groups = new Flags();
			groups->SetState(groupId);
		}

		/**
		* Resets selected group
		*/
		void UnsetGroup(int groupId) {
			if (groups == nullptr) groups = new Flags();
			groups->ResetState(groupId);
		}

		/**
		* Returns true, if states has been initialized
		* Check this indicator first before asking for states, because
		* they are lazy initialized
		*/
		const bool HasStates() const {
			return states != nullptr;
		}

		/**
		* Gets flag array, representing all states this object has set
		*/
		Flags& GetStates() {
			if (states == nullptr) states = new Flags();
			return *states;
		}

		/**
		* Sets states of this node
		*/
		void SetStates(Flags val) {
			delete states;
			this->states = new Flags(val);
		}

		/**
		* Returns true, if this object has selected state
		*/
		bool HasState(unsigned state) {
			if (!HasStates()) return false;
			return states->HasState(state);
		}

		/**
		* Sets new state
		*/
		void SetState(unsigned state) {
			GetStates().SetState(state);

			CogSendDirectMessage(ACT_STATE_CHANGED, 0, new StateChangeEvent(StateChange::SET, state), this, -1);
		}

		/**
		* Resets selected state
		*/
		void ResetState(unsigned state) {
			GetStates().ResetState(state);

			CogSendDirectMessage(ACT_STATE_CHANGED, 0, new StateChangeEvent(StateChange::RESET, state), this, -1);
		}

		/**
		* Switches values of two states
		*/
		void SwitchState(unsigned state1, unsigned state2) {
			GetStates().SwitchState(state1, state2);
			// send message
			CogSendDirectMessage(ACT_STATE_CHANGED, 0, new StateChangeEvent(StateChange::SWITCH, state1, state2), this, -1);
		}

		/**
		* Gets running mode
		*/
		RunningMode GetRunningMode() {
			return this->runMode;
		}

		/**
		* Sets running mode
		*/
		void SetRunningMode(RunningMode runMode) {
			this->runMode = runMode;
		}

		/**
		* Adds a new attribute; or replace already existing attribute
		* @param key key of the attribute
		* @param value reference
		*/
		template<class T> void AddAttr(StringHash key, T value) {
			if (HasAttr(key)) {
				RemoveAttr(key, true);
			}

			attributes[key] = new AttrR<T>(key, value, this);
		}

		/**
		* Gets an attribute by key; call this method only if you are sure that the attribute exists
		* @param key attribute key
		*/
		template<class T> T& GetAttr(StringHash key) {
			auto it = attributes.find(key);

			COGASSERT(it != attributes.end(), "GNODE", "%s: Attribute %d doesn't exists", tag->c_str(), (unsigned)key);
			COGASSERT(typeid(*it->second) == typeid(AttrR<T>), "GNODE", "%s: Attribute %d is of the wrong type!", tag->c_str(), (unsigned)key);

			AttrR<T>* attr = static_cast<AttrR<T>*>(it->second);
			return attr->GetValue();
		}

		/**
		* Changes value of selected attribute or adds a new attribute if it doesn't exist
		* @param key attribute key
		* @param value new value
		*/
		template<class T> void ChangeAttr(StringHash key, T value) {
			auto it = attributes.find(key);
			if (it != attributes.end()) {
				AttrR<T>* attr = static_cast<AttrR<T>*>(it->second);
				attr->SetValue(value);
			}
			else {
				AddAttr(key, value);
			}
		}

		bool operator==(int id) {
			return this->id == id;
		}

		bool operator==(const Node& other) {
			return id == other.id;
		}

		bool operator!=(int id) {
			return this->id != id;
		}

		bool operator!=(const Node& other) {
			return id != other.id;
		}

		// stage can access private members
		friend class Stage;

	protected:
		/*
		* Inserts children and behaviors that should be inserted
		* at the end of update loop
		*/
		void InsertElementsForAdding(bool applyToChildren);

		/*
		* Deletes children and behaviors that should be removed
		* at the end of update loop
		*/
		void DeleteElementsForRemoving(bool applyToChildren);

	public:
		void WriteInfo(int logLevel = 0);
	};

}// namespace