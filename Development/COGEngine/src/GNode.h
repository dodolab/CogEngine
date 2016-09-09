#pragma once

#include <map>
#include "GAttr.h"
#include "GBehavior.h"
#include "GMsg.h"
#include "EnBounds.h"
#include "Enums.h"
#include <list>
#include "SmartPointer.h"
#include <stdexcept>
#include <string>
#include "EnTransform.h"
#include "MGameStorage.h"
#include <assert.h>
#include "Utils.h"

using namespace std;

enum RunningMode{
	RUNNING, PAUSED_ALL, PAUSED_CHILDREN, PAUSED_ITSELF
};

/**
* GNode - game object with attributes, behaviors and transformation matrix
*
*/
class GNode{
protected:
	// identifier incremental counter
	static int idCounter;
	// attributes, mapped by their keys
	map<int, GAttr*> _attributes;
	// list of behaviors
	list<GBehavior*> _behaviors;
	// link to the parent
	GNode* _parent;
	// list of childrens
	list<GNode*> _children;

	list<std::pair<GNode*,bool>> _childrenToRemove;
	list<std::pair<GBehavior*,bool>> _behaviorToRemove;

	// id of this node
	const int _id;
	// tag or name
	string* _tag;
	// object type {ROOT, SCENE, OBJECT, HUD, INFO}
	ObjType _type;
	// subtype (or category)
	int _subType;
	// groups this object is in
	EnFlags* _groups ;
	// states this object has set
	EnFlags* _states;
	// transformation matrix (size = 24)
	EnTransform  _transform;

	RunningMode _runMode;

	void Construct(){
		_parent = nullptr;
		_tag = nullptr;
		_groups = nullptr;
		_states = nullptr;
		_tag = nullptr;
		_runMode = RunningMode::RUNNING;
	}
	
public:

	/**
	* Creates a new game object
	* @param type object type {ROOT, SCENE, OBJECT, HUD, INFO}
	* @param subType subtype/category number
	* @param tag tag/name 
	*/
	GNode(ObjType type, int subType, string tag);

	GNode(const GNode& copy);

	~GNode();

	/**
	* Updates transformations to itself and all children
	*/
	void UpdateTransforms();

	/**
	* Updates behavior inner state
	* @param delta delta time from the last loop
	* @param absolute absolute time since the game begun
	*/
	void Update(const uint64 delta, const uint64 absolute);

	/**
	* Draws all objects
	* @param delta delta time from the last loop
	* @param absolute absolute time since the game begun
	*/
	void Draw(const uint64 delta, const uint64 absolute);

	void ClearElementsForRemoving();

	/**
	* Adds a new behavior
	* @param beh behavior to add
	* @return true, if behavior has been added
	*/
	bool AddBehavior(GBehavior* beh);

	/**
	* Removes existing behavior (by its id)
	* @param beh behavior to remove
	* @return true, if behavior has been removed
	*/
	bool RemoveBehavior(GBehavior* beh, bool immediately, bool erase);

	/**
	* Removes existing attribute (by its key)
	* @param key key of attribute that will be removed
	* @return true, if attribute has been removed
	*/
	bool RemoveAttr(int key, bool erase);

	/**
	* Returns true, if this game object has an attribute with given key
	*/
	bool HasAttr(int key) const;

	/**
	* Gets copy of list of all behaviors
	*/
	list<GBehavior*> GetBehaviorsCopy() const;

	/**
	* Gets direct reference to list of all behaviors
	*/
	const list<GBehavior*>& GetBehaviors() const;

	/**
	* Gets copy of list of all children
	*/
	list<GNode*> GetChildrenCopy() const;

	/**
	* Gets direct reference to list of all children
	*/
	const list<GNode*>& GetChildren() const;

	/**
	* Adds a new child
	* @param child childto add
	* @return true, if child has been added
	*/
	bool AddChild(GNode* child);

	/**
	* Removes an existing child
	* @param child child to remove
	* @return true, if child has been removed
	*/
	bool RemoveChild(GNode* child, bool immediately, bool erase);

	/**
	* Gets pointer to the parent of this game node
	*/
	GNode* GetParent();

	/**
	* Sets parent of this game object
	*/
	void SetParent(GNode* val);

	/**
	* Finds the first predecessor of given object type
	* @param type predecessor type {ROOT, SCENE, OBJECT, HUD, INFO}
	*/
	GNode* FindPredecessor(ObjType type);

	/**
	* Gets the nearest parent that is a scene root (if exists)
	*/
	GNode* GetSceneRoot();

	/**
	* Gets the root of the game scene
	*/
	GNode* GetRoot();

	/**
	* Gets unique identifier of this game object
	*/
	int GetId() const;
	
	/**
	* Gets tag/name
	*/
	string GetTag() const;

	/**
	* Sets tag/name
	*/
	void SetTag(string tag);

	/**
	* Gets type/category of this game object {ROOT, SCENE, OBJECT, HUD, INFO}
	*/
	ObjType GetType() const;
	
	/**
	* Gets subtype
	*/
	int GetSubType() const;

	/**
	* Sets subtype 
	*/
	void SetSubType(int val);

	/**
	* Gets transformation matrix
	*/
	EnTransform& GetTransform();

	/**
	* Sets transformation matrix
	*/
	void SetTransform(EnTransform val);

	/**
	* Returns indicator, if groups has been initialized
	* Check this indicator first before asking for groups, because
	* they are lazy initialized
	*/
	const bool HasGroups() const{
		return _groups != nullptr;
	}

	/**
	* Gets flag array, representing list of all groups this object belongs to
	*/
	EnFlags& GetGroups();

	/**
	* Sets groups this objects belongs to
	*/
	void SetGroups(EnFlags val);

	/**
	* Gets indicator, if this object is in selected group
	*/
	bool IsInGroup(int groupId){
		return _groups->HasState(groupId);
	}

	/**
	* Sets selected group
	*/
	void SetGroup(int groupId){
		if (_groups == nullptr) _groups = new EnFlags();
		_groups->SetState(groupId);
	}

	/**
	* Resets selected group
	*/
	void UnsetGroup(int groupId){
		if (_groups == nullptr) _groups = new EnFlags();
		_groups->ResetState(groupId);
	}

	/**
	* Returns indicator, if states has been initialized
	* Check this indicator first before asking for states, because
	* they are lazy initialized
	*/
	const bool HasStates() const{
		return _states != nullptr;
	}

	/**
	* Gets flag array, representing all states this object has set
	*/
	EnFlags& GetStates();

	/**
	* Sets states this object has set
	*/
	void SetStates(EnFlags val);

	/**
	* Gets indicator, if this object has selected state
	*/
	bool HasState(int state){
		if (!HasStates()) return false;
		return _states->HasState(state);
	}

	/**
	* Sets selected state
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


	RunningMode GetRunningMode(){
		return this->_runMode;
	}

	void SetRunningMode(RunningMode runMode){
		this->_runMode = runMode;
	}

	/**
	* Adds a new attribute of value type
	* @param key key of the attribute
	* @param value reference
	* @param managed if true, attribute will be destroyed together with attribute
	*/
	template<class T> void AddAttr(int key, T value){
		if (HasAttr(key)){
			RemoveAttr(key,true);
		}

		_attributes[key] = new GAttrR<T>(key, value, this);
	}
	
	/**
	* Finds an attribute or creates a new one, if it doesn't exist
	* @param key attribute key
	*/
	template<class T> T& GetAttr(int key){
		auto it = _attributes.find(key);

		MASSERT(it != _attributes.end(), string_format("%s: Attribute %d doesn't exists", _tag->c_str(), key));
		MASSERT(typeid(*it->second) == typeid(GAttrR<T>), string_format("%s: Attribute %d is of the wrong type!", _tag->c_str(), key));

		GAttrR<T>* attr = static_cast<GAttrR<T>*>(it->second);
		return attr->GetValue();
	}

	/**
	* Changes value of attribute
	* @param key attribute key
	* @param value new value
	*/
	template<class T> void ChangeAttr(int key, T value){
		auto it = _attributes.find(key);
		if (it != _attributes.end()){
			GAttrR<T>* attr = static_cast<GAttrR<T>*>(it->second);
			attr->SetValue(value);
		}
		else{
			AddAttr(key, value);
		}
	}

	bool operator==(int id){
		return this->_id == id;
	}

	bool operator==(const GNode& other){
		return _id == other._id;
	}

	bool operator!=(int id){
		return this->_id != id;
	}

	bool operator!=(const GNode& other){
		return _id != other._id;
	}

	string GetInfo(bool complete);

	void GetInfo(bool complete, std::ostringstream& ss, int level);

	friend class MGameStorage;
};
