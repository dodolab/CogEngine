
#ifndef GNODE_H
#define GNODE_H

#include <map>
#include "Attr.h"
#include "ABehavior.h"
#include "Msg.h"

using namespace std;

class Attr;
class ABehavior;

class GNode{
protected:
	map<int, Attr> _attributes;
	vector<ABehavior*> _behaviors;
	GNode* _parent;
	vector<GNode*> _children;

	int _id;
	char* _tag;
	ObjType _type;
	int _subType;
	State _groups;
	State _states;
	CIwFMat2D  _transform;

	static int ids;


public:

	GNode(ObjType type, int subType, char* tag);

	~GNode();

	void SendMessage(Msg msg);

	void Update(int delta, int absolute);

	void Draw(int delta, int absolute);

	void AddBehavior(ABehavior* beh);

	void RemoveBehavior(ABehavior* beh);

	void RemoveAttr(int key);

	template<class T> Attrx<T> FindAtt(int id);

	template<class T> T FindAttValue(int id);

	vector<Attr*> GetAttributes();

	vector<ABehavior*> GetBehaviors();

	vector<GNode*> GetChildren();

	void AddChild(GNode* child);

	void RemoveChild(GNode* child);

	GNode GetParent();
	void SetParent(GNode val);

	int GetId();
	void SetId(int val);

	char* GetTag();
	void SetTag(char* tag);

	ObjType GetType();
	void SetType(ObjType val);

	int GetSubType();
	void SetSubType(int val);

	GNode FindParent(ObjType type);

	GNode GetSceneRoot();
	
	GNode GetRoot();

	CIwFMat2D GetTransform();
	CIwFMat2D SetTransform(CIwFMat2D val);

	template<class T> void AddAttribute(ElemType elemType, int key, T value);

	State GetGroups();
	void SetGroups(State val);

	State GetStates();
	void SetStates(State val);
};

#endif
