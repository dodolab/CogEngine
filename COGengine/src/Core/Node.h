#pragma once

#include "Attr.h"
#include "Behavior.h"
#include "Msg.h"
#include "Facade.h"
#include "Mesh.h"
#include "MsgPayloads.h"
#include "Flags.h"

namespace Cog {

	class Scene;

	/**
	* Scene object; the most important entity in the whole ofxCogEngine
	*
	* Each object has set of attributes, behaviors and states. By default it has a
	* rendering mesh (but there could be non-renderable nodes as well) and 
	* a transformation.
	*
	* The main part of component-oriented approach is to provide modularity and this class goes in this way.
	* The Node can become a game object, trigger, but also an isolated entity with own scene stored as an attribute
	* 
	*/
	class Node {
	protected:
		// identifier incremental counter
		static int idCounter;

		// id of this node
		const int id;
		// tag or name
		string tag = "";
		// object type {ROOT, SCENE, OBJECT}
		stenum type;
		// network identifier
		int networkId;
		// groups this objects belongs to
		Flags* groups = nullptr;
		// states of this object
		Flags* states = nullptr;
		// transformation
		Trans  transform;
		// mesh object
		spt<Mesh> mesh = spt<Mesh>(new Mesh(MESH_NONE));
		// running mode
		stenum runMode = RUNMODE_RUNNING;
		// indicator, if this node is allocated via a custom allocator and therefore it shouldn't be deleted
		bool isExternal = false;

		// attributes, mapped by their keys
		map<StrId, Attr*> attributes;
		// list of childrens
		list<Node*> children;
		// list of behaviors
		list<Behavior*> behaviors;
		// link to the parent
		Node* parent = nullptr;
		// link to the scene
		Scene* scene = nullptr;
		// list of children that will be removed at the end of update loop
		list<std::pair<Node*, bool>> childrenToRemove;
		// list of behaviors that will be removed at the end of update loop
		list<std::pair<Behavior*, bool>> behaviorToRemove;
		// list of children that will be inserted at the end of update loop
		list<Node*> childrenToAdd;
		// list of behaviors that will be added at the end of update loop
		list<Behavior*> behaviorsToAdd;
	public:

		/**
		* Creates a new node
		* @param tag tag/name
		*/
		Node(string tag);

		/**
		 * Creates a new node
		 * @param tag tag/name
		 * @param mesh mesh of this node
		 */
		Node(string tag, spt<Mesh> mesh);

		/**
		* Creates a new node
		* @param type object type {ROOT, SCENE, OBJECT}
		* @param secondaryId secondary id
		* @param tag tag/name
		*/
		Node(stenum type, int secondaryId, string tag);

		Node(const Node& copy);

		~Node();

		/**
		* Gets unique identifier
		*/
		int GetId() const {
			return id;
		}

		/**
		* Gets tag/name
		*/
		string GetTag() const {
			return tag;
		}

		/**
		* Sets tag/name
		*/
		void SetTag(string tag) {
			this->tag = tag;
		}

		/**
		* Returns true, if the node has given tag
		*/
		bool HasTag(string tag) {
			return this->tag.compare(tag) == 0;
		}

		/**
		* Gets type {ROOT, SCENE, OBJECT}
		*/
		stenum GetType() const {
			return this->type;
		}

		/**
		* Gets network id
		*/
		int GetNewtorkId() const {
			return networkId;
		}

		/**
		* Sets secondary id
		*/
		void SetNetworkId(int val) {
			this->networkId = val;
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
		bool IsInGroup(unsigned groupId) {
			return HasGroups() && groups->HasState(groupId);
		}

		/**
		* Sets a group for this node
		*/
		void SetGroup(unsigned groupId) {
			if (groups == nullptr) groups = new Flags();
			groups->SetState(groupId);
		}

		/**
		* Resets selected group
		*/
		void UnsetGroup(unsigned groupId) {
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
		* Returns true, if this object has the selected state
		*/
		bool HasState(unsigned state) {
			if (!HasStates()) return false;
			return states->HasState(state);
		}

		/**
		* Sets a new state
		*/
		void SetState(unsigned state);

		/**
		* Resets the selected state
		*/
		void ResetState(unsigned state);

		/**
		* Switches values of two states
		*/
		void SwitchState(unsigned state1, unsigned state2);

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
		* Gets mesh object
		*/
		spt<Mesh> GetMesh() const {
			return mesh;
		}

		/**
		* Gets mesh object
		*/
		Mesh* GetMeshPtr() const {
			return mesh.get();
		}

		/**
		* Get mesh of selected type; must inherit from Mesh entity
		*/
		template<class T> spt<T> GetMesh() const {
			auto spMesh = static_pointer_cast<T>(mesh);
			return spMesh;
		}

		/**
		*  Sets mesh
		*/
		void SetMesh(spt<Mesh> mesh) {
			this->mesh = mesh;
		}

		/**
		* Sets image imesh directly from path
		*/
		void SetImageMesh(string path);

		/**
		* Returns true, if the shape is of the selected type
		*/
		bool HasMeshType(stenum type) {
			return mesh->GetMeshType() == type;
		}

		/**
		* Gets indicator whether this entity is renderable
		*/
		bool IsRenderable() {
			return mesh->GetMeshType() != MESH_NONE;
		}

		/**
		* Gets running mode
		*/
		stenum GetRunningMode() {
			return this->runMode;
		}

		/**
		* Sets running mode
		*/
		void SetRunningMode(stenum runMode) {
			this->runMode = runMode;
		}

		/**
		* Gets indicator whether this entity is external
		*/
		bool IsExternal() {
			return isExternal;
		}

		/**
		* Sets indicator whether this entity is external
		*/
		void SetIsExternal(bool ext) {
			this->isExternal = ext;
		}


		/**
		* Updates absolute transformations according to the parent
		* @param deep if true, children will be updated as well
		*/
		void UpdateTransform(bool deep);

		/**
		* Updates all behaviors in the subtree
		* @param delta delta time since the last loop
		* @param absolute absolute time since the engine initialization
		*/
		void Update(const uint64 delta, const uint64 absolute);

		/**
		* Draws all objects
		* @param delta delta time since the last loop
		* @param absolute absolute time since the engine initialization
		*/
		void Draw(const uint64 delta, const uint64 absolute);

		/**
		* Adds a new behavior
		* @param beh behavior to add
		* @return true, if behavior has been successfully added
		*/
		bool AddBehavior(Behavior* beh);

		/**
		* Removes an existing behavior
		* @param beh behavior to remove
		* @param erase if true, behavior will be destroyed
		* @return true, if behavior has added to the collection for removal
		*/
		bool RemoveBehavior(Behavior* beh, bool erase);

		/**
		* Removes existing attribute directly
		* @param key key of attribute that will be removed
		* @param erase if true, attribute will be destroyed
		* @return true, if attribute has been removed
		*/
		bool RemoveAttr(StrId key, bool erase);

		/**
		* Returns true, if this node has an attribute with given key
		*/
		bool HasAttr(StrId key) const;

		/**
		* Updates collections of behaviors and children nodes
		* Adds/removes elements that are supposed to be added/removed
		* @param applyToChildren if true, children will be updated as well
		*/
		void SubmitChanges(bool applyToChildren);


		/**
		* Gets direct reference to list of all behaviors
		*/
		const list<Behavior*>& GetBehaviors() const {
			return behaviors;
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
		* @return true, if child has been added into the collection for adding
		*/
		bool AddChild(Node* child);

		/**
		* Removes an existing child
		* @param child child to remove
		* @param erase if true, child will be destroyed
		* @return true, if child has been added to the collection for removal
		*/
		bool RemoveChild(Node* child, bool erase);

		void RemoveAllChildren(bool erase);

		/**
		* Removes this node from its parent (if it exists)
		* @param erase if true, node will be destroyed
		* @return true, if this object has been added to the parent's collection for removal
		*/
		bool RemoveFromParent(bool erase);

		/**
		* Gets pointer to the parent of this node
		*/
		Node* GetParent() const {
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
		* @param type predecessor type {ROOT, SCENE, OBJECT}
		*/
		Node* FindPredecessor(stenum type) const;

		/**
		* Gets the nearest parent which is a scene root (if exists)
		*/
		Node* GetSceneRoot() const {
			if (type == NODETYPE_SCENE) return const_cast<Node*>(this);
			else return FindPredecessor(NODETYPE_SCENE);
		}

		/**
		* Gets the root of the whole stage
		*/
		Node* GetRoot() const {
			if (type == NODETYPE_ROOT) return const_cast<Node*>(this);
			else return FindPredecessor(NODETYPE_ROOT);
		}

		/**
		* Gets scene of this object
		*/
		Scene* GetScene() const {
			return scene;
		}

		/**
		* Sets object scene
		*/
		void SetScene(Scene* scene) {
			this->scene = scene;
		}

		/**
		* Gets behavior by its type
		* Note: use this method only if you know what you are doing; 
		* in component oriented approach there shouldn't be any direct communication
		* between behaviors. 
		*/
		template<class T> T* GetBehavior() {
			for (auto beh : this->behaviors) {
				if (typeid(*beh) == typeid(T)) {
					return static_cast<T*>(beh);
				}
			}

			// search for not added behaviors
			for (auto beh : this->behaviorsToAdd) {
				if (typeid(*beh) == typeid(T)) {
					return static_cast<T*>(beh);
				}
			}

			return nullptr;
		}

		/**
		* Adds a new attribute or replaces already existing attribute
		* @param key key of the attribute
		* @param value reference
		*/
		template<class T> void AddAttr(StrId key, T value, bool isShared = false) {
			if (HasAttr(key)) {
				RemoveAttr(key, true);
			}

			attributes[key] = new AttrR<T>(key, value, this, isShared);
			SendMessage(ACT_ATTR_CHANGED, new AttributeChangeEvent(key, AttrChangeType::ADD));
		}

		/**
		* Adds a new attribute if doesn't exists
		* @param key key of the attribute
		* @param value reference
		*/
		template<class T> bool AddAttrIfNotExists(StrId key, T value, bool isShared = false) {
			if (HasAttr(key)) {
				return false;
			}

			attributes[key] = new AttrR<T>(key, value, this, isShared);
			SendMessage(ACT_ATTR_CHANGED, new AttributeChangeEvent(key, AttrChangeType::ADD));
			return true;
		}

		/**
		* Gets an attribute by key; call this method only if you are sure that the attribute exists
		* @param key attribute key
		*/
		template<class T> T& GetAttr(StrId key) {
			auto it = attributes.find(key);

			COGASSERT(it != attributes.end(), "Node", "%s: Attribute %s doesn't exists", tag.c_str(), key.GetStringValue().c_str());
			COGASSERT(typeid(*it->second) == typeid(AttrR<T>), "Node", "%s: Attribute %s is of the wrong type!", tag.c_str(), key.GetStringValue().c_str());

			AttrR<T>* attr = static_cast<AttrR<T>*>(it->second);
			return attr->GetValue();
		}

		/**
		* Changes value of selected attribute or adds a new attribute if it doesn't exist
		* @param key attribute key
		* @param value new value
		*/
		template<class T> void ChangeAttr(StrId key, T value) {
			auto it = attributes.find(key);
			if (it != attributes.end()) {
				AttrR<T>* attr = static_cast<AttrR<T>*>(it->second);
				attr->SetValue(value);
				SendMessage(ACT_ATTR_CHANGED, new AttributeChangeEvent(key, AttrChangeType::MODIFY));
			}
			else {
				AddAttr(key, value);
				SendMessage(ACT_ATTR_CHANGED, new AttributeChangeEvent(key, AttrChangeType::ADD));
			}
		}

	
		/**
		* Gets a pointer attribute by its key
		*/
		void* GetAttrPtr(StrId key);

		/**
		* Gets a pointer attribute using a static template parameter (used for Lua mapping)
		*/
		template <char const *str>
		void* GetAttrPtrStatic() {
			return GetAttrPtr(StrId(str));
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
		void InsertElementsForAdding(bool applyToChildren, bool init);

		/*
		* Deletes children and behaviors that should be removed
		* at the end of update loop
		*/
		void DeleteElementsForRemoving(bool applyToChildren);

		/**
		* Sends message to the scene
		* @param action message action type
		* @param data payload
		*/
		void SendMessage(StrId action, RefCountedObjectPtr<MsgPayload> data);


	public:
		/**
		* Writes info about hierarchy of this node into the console
		*/
		void WriteInfo(int logLevel = 0);
	};

}// namespace