
#ifndef GNODE_H
#define GNODE_H

#include <map>
#include "Attr.h"
#include "ABehavior.h"
#include "Msg.h"
#include "IwGeomFMat2D.h"
#include "IwList.h"
#include "EnBounds.h"
#include "IwMap.h"
#include "Enums.h"

using namespace std;

/**
* GNode - game object with attributes, behaviors and transformation matrix
*
*/
class GNode{
protected:
	// identifier incremental counter
	static int idCounter;
	// dummy message that is used for no-response sending
	static Msg _dummyMsg;
	// attributes, mapped by their keys
	map<int, Attr*> _attributes;
	// list of behaviors
	CIwList<ABehavior*> _behaviors;
	// link to the parent
	GNode* _parent;
	// list of childrens
	CIwList<GNode*> _children;

	// id of this node
	const int _id;
	// tag or name
	char* _tag;
	// object type {ROOT, SCENE, OBJECT, HUD, INFO}
	ObjType _type;
	// subtype (or category)
	int _subType;
	// groups this object is in
	EnFlags _groups;
	// states this object has set
	EnFlags _states;
	// transformation matrix
	CIwFMat2D  _transform;
	
public:

	/**
	* Creates a new game object
	* @param type object type {ROOT, SCENE, OBJECT, HUD, INFO}
	* @param subType subtype/category number
	* @param tag tag/name 
	*/
	GNode(ObjType type, int subType, char* tag);

	GNode(const GNode& copy);

	~GNode();

	/**
	* Sends a message to behaviors and other game objects
	* @param msg message that will be sent
    * @param resp response that could be filled (at most once)
	*/
	void SendMessage(Msg& msg, Msg& resp);

	/**
	* Sends a message to behaviors and other game objects, expecting no response
	* @param msg message that will be sent
	*/
	void SendMessageNoResp(Msg& msg);

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

	/**
	* Adds a new behavior
	* @param beh behavior to add
	* @return true, if behavior has been added
	*/
	bool AddBehavior(ABehavior* beh);

	/**
	* Removes existing behavior (by its id)
	* @param beh behavior to remove
	* @return true, if behavior has been removed
	*/
	bool RemoveBehavior(ABehavior* beh);

	/**
	* Removes existing attribute (by its key)
	* @param key key of attribute that will be removed
	* @return true, if attribute has been removed
	*/
	bool RemoveAttr(int key);

	/**
	* Returns true, if this game object has an attribute with given key
	*/
	bool HasAttr(int key) const;

	/**
	* Gets copy of list of all behaviors
	*/
	CIwList<ABehavior*> GetBehaviorsCopy() const;

	/**
	* Gets direct reference to list of all behaviors
	*/
	const CIwList<ABehavior*>& GetBehaviors() const;

	/**
	* Gets copy of list of all children
	*/
	CIwList<GNode*> GetChildrenCopy() const;

	/**
	* Gets direct reference to list of all children
	*/
	const CIwList<GNode*>& GetChildren() const;

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
	bool RemoveChild(GNode* child);

	/**
	* Gets pointer to the parent of this game node
	*/
	GNode* GetParent() const;

	/**
	* Sets parent of this game object
	*/
	void SetParent(GNode* val);

	/**
	* Finds the first predecessor of given object type
	* @param type predecessor type {ROOT, SCENE, OBJECT, HUD, INFO}
	*/
	GNode* FindPredecessor(ObjType type) const;

	/**
	* Gets the nearest parent that is a scene root (if exists)
	*/
	GNode* GetSceneRoot() const;

	/**
	* Gets the root of the game scene
	*/
	GNode* GetRoot() const;

	/**
	* Gets unique identifier of this game object
	*/
	int GetId() const;
	
	/**
	* Gets tag/name
	*/
	char* GetTag() const;

	/**
	* Sets tag/name
	*/
	void SetTag(char* tag);

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
	CIwFMat2D& GetTransform() const;

	/**
	* Sets transformation matrix
	*/
	void SetTransform(CIwFMat2D& val);

	/**
	* Updates transformation matrix
	* @parent updated transformation matrix of the parent
	*/
	void UpdateTransform(CIwFMat2D& parent);

	/**
	* Gets flag array, representing list of all groups this object belongs to
	*/
	EnFlags& GetGroups() const;

	/**
	* Sets groups this objects belongs to
	*/
	void SetGroups(EnFlags& val);

	/**
	* Gets flag array, representing all states this object has set
	*/
	EnFlags& GetStates() const;

	/**
	* Sets states this object has set
	*/
	void SetStates(EnFlags& val);

	/**
	* Add Attribute Value
	* Adds a new attribute, using regular object
	* @param key key of the attribute
	* @param elemType type of attribute {ALL, MODEL, VIEW}
	* @param value attribute value
	*/
	template<class T> void AddAttrV(int key, ElemType elemType, T value){
		if(_attributes.find(key) != _attributes.end()){
			RemoveAttr(key);
		}

		_attributes[key] = new AttrR<T>(key, elemType, value, this);
	}

	/**
	* Add Attribute Reference
	* Adds a new attribute, using reference
	* @param key key of the attribute
	* @param elemType type of attribute {ALL, MODEL, VIEW}
	* @param value reference
	* @param managed if true, attribute will be destroyed together with attribute
	*/
	template<class T> void AddAttrR(int key, ElemType elemType, T& value){
		if (_attributes.find(key) != _attributes.end()){
			RemoveAttr(key);
		}

		_attributes[key] = new AttrR<T>(key, elemType, value, this);
	}

	/**
	* Add Attribute Pointer
	* Adds a new attribute, using pointer
	* @param key key of the attribute
	* @param elemType type of attribute {ALL, MODEL, VIEW}
	* @param value pointer
	*/
	template<class T> void AddAttrP(int key, ElemType elemType, T* value){
		if (_attributes.find(key) != _attributes.end()){
			RemoveAttr(key);
		}

		_attributes[key] = new AttrP<T>(key, elemType, value, this, false);
	}

	/**
	* Add Attribute Pointer Managed
	* Adds a new attribute, using pointer; attribute is managed - will be deleted when removed from game object
	* @param key key of the attribute
	* @param elemType type of attribute {ALL, MODEL, VIEW}
	* @param value pointer
	*/
	template<class T> void AddAttrPM(int key, ElemType elemType, T* value){
		if (_attributes.find(key) != _attributes.end()){
			RemoveAttr(key);
		}

		_attributes[key] = new AttrP<T>(key, elemType, value, this, true);
	}

	/**
	* Finds an reference attribute
	* @param key attribute key
	*/
	template<class T> T const& FindAttrR(int key) const{
		 auto it = _attributes.find(key);
		 AttrR<T>* attr =	static_cast<AttrR<T>*>(it->second);
		 return attr->GetValue();
	}

	/**
	* Finds an pointer attribute
	* @param key attribute key
	*/
	template<class T> T const* FindAttrP(int key) const{
		auto it = _attributes.find(key);
		AttrP<T>* attr = static_cast<AttrP<T>*>(it->second);
		return attr->GetValue();
	}

	/**
	* Changes value of attribute of pointer type
	* @param key attribute key
	* @param value new value
	*/
	template<class T> void ChangeAttr(int key, T* value){
		auto it = _attributes.find(key);
		AttrP<T>* attr = static_cast<AttrP<T>*>(it->second);
		attr->SetValue(value);
	}

	/**
	* Changes value of attribute of reference type
	* @param key attribute key
	* @param value new value
	*/
	template<class T> void ChangeAttr(int key, T const& value){
		auto it = _attributes.find(key);
		AttrR<T>* attr = static_cast<AttrR<T>*>(it->second);
		attr->SetValue(value);
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
};


#endif
