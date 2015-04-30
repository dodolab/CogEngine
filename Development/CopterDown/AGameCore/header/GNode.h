
#ifndef GNODE_H
#define GNODE_H

#include <map>
#include "Attr.h"
#include "ABehavior.h"
#include "Msg.h"
#include "IwGeomFMat2D.h"
#include "EnBounds.h"

using namespace std;

class GNode{
protected:
	static Msg _dummyMsg;
	static int idCounter;
	
	map<int, Attr> _attributes;
	CIwArray<ABehavior*> _behaviors;
	GNode* _parent;
	CIwArray<GNode*> _children;

	const int _id;
	char* _tag;
	ObjType _type;
	int _subType;
	EnFlags _groups;
	EnFlags _states;
	CIwFMat2D  _transform;
	


public:

	GNode(ObjType type, int subType, char* tag);

	GNode(const GNode& copy);

	~GNode();

	void SendMessage(Msg& msg, Msg& resp) const;

	void SendMessageNoResp(Msg& msg) const;

	void Update(const uint64 delta, const uint64 absolute);

	void Draw(const uint64 delta, const uint64 absolute);

	void AddBehavior(ABehavior* beh);

	void RemoveBehavior(ABehavior* beh);

	void RemoveAttr(int key);

	bool HasAttr(int key) const;

	template<class T> void AddAttrRef(int key, ElemType elemType, T& value){

	}

	template<class T> void AddAttrPtr(int key, ElemType elemtype, T* value){

	}

	template<class T> void CopyAttr(int id, T& output) const{

	}
	
	template<class T> T* FindAttrPtr(int id) const{
		return nullptr;
	}

	template<class T> T& FindAttrRef(int id) const{
		return *(new T());
	}

	template<class T> T& FindOrCreateAttrRef(int id){
		return *(new T());
	}

	template<class T> T* FindOrCreateAttrPtr(int id){
		return nullptr;
	}

	template<class T> void ChangeAttrPtr(int id, T* value){

	}

	template<class T> void ChangeAttrRef(int id, T& value){

	}

	CIwArray<ABehavior*> GetBehaviorsCopy() const;

	const CIwArray<ABehavior*> GetBehaviors() const;

	CIwArray<GNode*> GetChildrenCopy() const;

	const CIwArray<GNode*> GetChildren() const;

	void AddChild(GNode* child);

	void RemoveChild(GNode* child);

	GNode* GetParent() const;
	void SetParent(GNode* val);

	GNode* FindPredecessor(ObjType type) const;

	GNode* GetSceneRoot() const;

	GNode* GetRoot() const;

	int GetId() const;
	
	char* GetTag() const;
	void SetTag(char* tag);

	ObjType GetType() const;
	
	int GetSubType() const;
	void SetSubType(int val);

	CIwFMat2D GetTransform() const;
	CIwFMat2D SetTransform(CIwFMat2D val);
	void UpdateTransform(CIwFMat2D& parent);

	EnFlags GetGroups() const;
	void SetGroups(EnFlags val);

	EnFlags GetStates() const;
	void SetStates(EnFlags val);
};


#endif
