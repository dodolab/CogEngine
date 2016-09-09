#pragma once

#include <map>
#include "GAttr.h"
#include "GBehavior.h"
#include "GMsg.h"
#include "MEnums.h"
#include <list>
#include "ofxSmartPointer.h"
#include <stdexcept>
#include <string>
#include "EnTransform.h"
#include <assert.h>
#include "MUtils.h"
#include "MFacade.h"
#include "EnShape.h"

using namespace std;

/*! Node running mode */
enum RunningMode{
	RUNNING,			/*!< active for update and draw */
	PAUSED_ALL,			/*!< active for draw */
	PAUSED_CHILDREN,	/*!< active itself but children are inactive */
	PAUSED_ITSELF,		/*!< inactive but children are active */
	INVISIBLE,			/*!< active for update but inactive for draw */
	DISABLED			/*!< inactive for both, update and draw */
};

/**
* Node with attributes, behaviors, states and transformation matrix
*
*/
class GNode{
protected:
	// identifier incremental counter
	static int idCounter;
	// attributes, mapped by their keys
	map<int, GAttr*> attributes;
	// list of childrens
	list<GNode*> children;
	// list of behaviors
	list<GBehavior*> behaviors;
	// link to the parent
	GNode* parent;
	// list of children that will be removed at the end of update method
	list<std::pair<GNode*,bool>> childrenToRemove;
	// list of behaviors that will be removed at the end of update method
	list<std::pair<GBehavior*,bool>> behaviorToRemove;
	// list of children that will be added at the end of update method
	list<GNode*> childrenToAdd;
	// list of behaviors that will be added at the end of update method
	list<GBehavior*> behaviorsToAdd;

	// id of this node
	const int id;
	// tag or name
	string* tag;
	// object type {ROOT, SCENE, OBJECT, HUD, INFO}
	ObjType type;
	// subtype (or category)
	int subType;
	// groups this objects belongs to
	EnFlags* groups ;
	// states of this object
	EnFlags* states;
	// transformation matrix
	EnTransform  transform;
	// mash object
	spt<EnShape> shape;
	// running mode
	RunningMode runMode;

	// initialization method
	void Construct(){
		parent = nullptr;
		tag = nullptr;
		groups = nullptr;
		states = nullptr;
		tag = nullptr;
		runMode = RunningMode::RUNNING;
		shape = spt<EnShape>(new EnShape(RenderType::NONE));
	}

	
public:

	/**
	* Creates a new node
	* @param tag tag/name
	*/
	GNode(string tag);

	/**
	* Creates a new node
	* @param type object type {ROOT, SCENE, OBJECT, HUD, INFO}
	* @param subType subtype/category number
	* @param tag tag/name 
	*/
	GNode(ObjType type, int subType, string tag);

	GNode(const GNode& copy);

	~GNode();

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
	bool AddBehavior(GBehavior* beh);

	/**
	* Removes existing behavior (by its id)
	* @param beh behavior to remove
	* @param erase if true, memory will be released
	* @return true, if behavior has been removed
	*/
	bool RemoveBehavior(GBehavior* beh, bool erase);

	/**
	* Removes existing attribute (by its key)
	* @param key key of attribute that will be removed
	* @param erase if true, memory will be released
	* @return true, if attribute has been removed
	*/
	bool RemoveAttr(int key, bool erase);

	/**
	* Returns true, if this node has an attribute with given key
	*/
	bool HasAttr(int key) const;

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
	list<GBehavior*> GetBehaviorsCopy() const{
		return behaviors;
	}

	/**
	* Gets direct reference to list of all behaviors
	*/
	const list<GBehavior*>& GetBehaviors() const{
		return behaviors;
	}

	/**
	* Gets copy of list of all children
	*/
	list<GNode*> GetChildrenCopy() const{
		return children;
	}

	/**
	* Gets direct reference to list of all children
	*/
	const list<GNode*>& GetChildren() const{
		return children;
	}

	/**
	* Adds a new child
	* @param child child to add
	* @return true, if child has been successfully added
	*/
	bool AddChild(GNode* child);

	/**
	* Removes an existing child
	* @param child child to remove
	* @param erase if true, memory will be released
	* @return true, if child has been removed
	*/
	bool RemoveChild(GNode* child, bool erase);

	/**
	* Removes this node from its parent (if it exists)
	* @param erase if true, memory will be released
	* @return true, if this object has been removed
	*/
	bool RemoveFromParent(bool erase);

	/**
	* Gets pointer to the parent of this node
	*/
	GNode* GetParent(){
		return parent;
	}

	/**
	* Sets parent of this node
	*/
	void SetParent(GNode* val){
		parent = val;
	}

	/**
	* Finds the first predecessor of given object type
	* @param type predecessor type {ROOT, SCENE, OBJECT, HUD, INFO}
	*/
	GNode* FindPredecessor(ObjType type);

	/**
	* Gets the nearest parent that is a scene root (if exists)
	*/
	GNode* GetSceneRoot(){
		if (type == ObjType::SCENE) return this;
		else return FindPredecessor(ObjType::SCENE);
	}

	/**
	* Gets the root of the whole scene
	*/
	GNode* GetRoot(){
		if (type == ObjType::ROOT) return this;
		else return FindPredecessor(ObjType::ROOT);
	}

	/**
	* Gets unique identifier of this node
	*/
	int GetId() const{
		return id;
	}
	
	/**
	* Gets tag/name
	*/
	string GetTag() const{
		if (tag == nullptr) return "";
		else return string(*tag);
	}

	/**
	* Sets tag/name
	*/
	void SetTag(string tag){
		delete this->tag;
		this->tag = new string(tag);
	}

	/**
	* Gets type/category of this node {ROOT, SCENE, OBJECT, HUD, INFO}
	*/
	ObjType GetType() const{
		return this->type;
	}
	
	/**
	* Gets subtype/category
	*/
	int GetSubType() const{
		return subType;
	}

	/**
	* Sets subtype/category
	*/
	void SetSubType(int val){
		this->subType = val;
	}

	/**
	* Gets transformation entity
	*/
	EnTransform& GetTransform(){
		return transform;
	}

	/**
	* Sets transformation entity
	*/
	void SetTransform(EnTransform val){
		this->transform = val;
	}

	/**
	* Gets indicator, if this entity is renderable
	*/
	bool IsRenderable(){
		return shape->GetRenderType() != RenderType::NONE;
	}

	/**
	* Gets shaping object
	*/
	spt<EnShape> GetShape(){
		return shape;
	}

	/**
	* Get shape of selected template; must inherit from EnShape entity
	*/
	template<class T> T GetShape(){
		auto specialShape = static_cast<T>(shape);
		return specialShape;
	}

	/**
	*  Sets shaping object
	*/
	void SetShape(spt<EnShape> shape){
		this->shape = shape;
	}

	/**
	* Returns true, if the shape is of the selected type
	*/
	bool HasRenderType(RenderType type){
		return shape->GetRenderType() == type;
	}

	/**
	* Returns true, if groups has been initialized
	* Check this indicator first before asking for groups, because
	* they are lazy initialized
	*/
	const bool HasGroups() const{
		return groups != nullptr;
	}

	/**
	* Gets flag array, representing list of all groups this object belongs to
	*/
	EnFlags& GetGroups(){
		if (groups == nullptr) groups = new EnFlags();
		return *groups;
	}

	/**
	* Sets groups this objects belongs to
	*/
	void SetGroups(EnFlags val){
		delete groups;
		this->groups = new EnFlags(val);
	}

	/**
	* Returns true, if this object is in selected group
	*/
	bool IsInGroup(int groupId){
		return groups->HasState(groupId);
	}

	/**
	* Sets selected group
	*/
	void SetGroup(int groupId){
		if (groups == nullptr) groups = new EnFlags();
		groups->SetState(groupId);
	}

	/**
	* Resets selected group
	*/
	void UnsetGroup(int groupId){
		if (groups == nullptr) groups = new EnFlags();
		groups->ResetState(groupId);
	}

	/**
	* Returns true, if states has been initialized
	* Check this indicator first before asking for states, because
	* they are lazy initialized
	*/
	const bool HasStates() const{
		return states != nullptr;
	}

	/**
	* Gets flag array, representing all states this object has set
	*/
	EnFlags& GetStates(){
		if (states == nullptr) states = new EnFlags();
		return *states;
	}

	/**
	* Sets states of this node
	*/
	void SetStates(EnFlags val){
		delete states;
		this->states = new EnFlags(val);
	}

	/**
	* Returns true, if this object has selected state
	*/
	bool HasState(int state){
		if (!HasStates()) return false;
		return states->HasState(state);
	}

	/**
	* Sets new state
	*/
	void SetState(int state){
		GetStates().SetState(state);
	}

	/**
	* Resets selected state
	*/
	void ResetState(int state){
		GetStates().ResetState(state);
	}

	/**
	* Switches values of two states
	*/
	void SwitchState(int state1, int state2){
		GetStates().SwitchState(state1, state2);
	}

	/**
	* Gets running mode
	*/
	RunningMode GetRunningMode(){
		return this->runMode;
	}

	/**
	* Sets running mode
	*/
	void SetRunningMode(RunningMode runMode){
		this->runMode = runMode;
	}

	/**
	* Adds a new attribute; or replace already existing attribute
	* @param key key of the attribute
	* @param value reference
	*/
	template<class T> void AddAttr(int key, T value){
		if (HasAttr(key)){
			RemoveAttr(key,true);
		}

		attributes[key] = new GAttrR<T>(key, value, this);
	}
	
	/**
	* Gets an attribute by key; call this method only if you are sure that the attribute exists
	* @param key attribute key
	*/
	template<class T> T& GetAttr(int key){
		auto it = attributes.find(key);

		MASSERT(it != attributes.end(), "GNODE", "%s: Attribute %d doesn't exists", tag->c_str(), key);
		MASSERT(typeid(*it->second) == typeid(GAttrR<T>), "GNODE", "%s: Attribute %d is of the wrong type!", tag->c_str(), key);

		GAttrR<T>* attr = static_cast<GAttrR<T>*>(it->second);
		return attr->GetValue();
	}

	/**
	* Changes value of selected attribute or adds a new attribute if it doesn't exist
	* @param key attribute key
	* @param value new value
	*/
	template<class T> void ChangeAttr(int key, T value){
		auto it = attributes.find(key);
		if (it != attributes.end()){
			GAttrR<T>* attr = static_cast<GAttrR<T>*>(it->second);
			attr->SetValue(value);
		}
		else{
			AddAttr(key, value);
		}
	}

	bool operator==(int id){
		return this->id == id;
	}

	bool operator==(const GNode& other){
		return id == other.id;
	}

	bool operator!=(int id){
		return this->id != id;
	}

	bool operator!=(const GNode& other){
		return id != other.id;
	}

	/**
	* Writes information to the console and returns it as a string
	* @param includeChildren if true, children will be included in info report
	* @param includeAttribute if true, attributes will be included in info report
	*/
	string GetInfo(bool includeChildren, bool includeAttributes);

	/**
	* Writes information to the stringstream 
	* @param includeChildren if true, children will be included in info report
	* @param includeAttribute if true, attributes will be included in info report
	* @param ss stringstream to write into
	* @param level actual space padding
	*/
	void GetInfo(bool includeChildren, bool includeAttributes, std::ostringstream& ss, int level);

	// app storage can access private members
	friend class MRepository;

	protected:
		/*
		* Inserts children and behaviors that should be inserted
		* at the end of update loop
		*/
		void InsertElementsForAdding();

		/*
		* Deletes children and behaviors that should be removed
		* at the end of update loop
		*/
		void DeleteElementsForRemoving();
};
