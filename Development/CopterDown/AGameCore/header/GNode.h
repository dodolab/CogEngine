
#ifndef GNODE_H
#define GNODE_H

#include <map>
#include "Attr.h"
#include "ABehavior.h"
#include "Msg.h"
#include "IwGeomFMat2D.h"

using namespace std;

class GNode{
protected:
	map<int, Attr> _attributes;
	CIwArray<ABehavior*> _behaviors;
	GNode* _parent;
	CIwArray<GNode*> _children;

	int _id;
	char* _tag;
	ObjType _type;
	int _subType;
	EnFlags _groups;
	EnFlags _states;
	CIwFMat2D  _transform;

	static int ids;


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

	template<class T> Attrx<T>* FindAtt(int id) const{
		return nullptr;
	}

	template<class T> T* FindAttValue(int id) const{
		return nullptr;
	}

	const CIwArray<ABehavior*> GetBehaviors() const;

	const CIwArray<GNode*> GetChildren() const;

	void AddChild(GNode* child);

	void RemoveChild(GNode* child);

	GNode* GetParent() const;
	void SetParent(GNode* val);

	int GetId() const;
	void SetId(int val);

	char* GetTag() const;
	void SetTag(char* tag);

	ObjType GetType() const;
	void SetType(ObjType val);

	int GetSubType() const;
	void SetSubType(int val);

	GNode* FindParent(ObjType type) const;

	GNode* GetSceneRoot() const;
	
	GNode* GetRoot() const;

	CIwFMat2D GetTransform() const;
	CIwFMat2D SetTransform(CIwFMat2D val);
	void UpdateTransform(CIwFMat2D& parent);

	template<class T> void AddAttribute(ElemType elemType, int key, T& value);

	EnFlags GetGroups() const;
	void SetGroups(EnFlags val);

	EnFlags GetStates() const;
	void SetStates(EnFlags val);
};


#endif
