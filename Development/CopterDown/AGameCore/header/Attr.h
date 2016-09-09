
#ifndef ATTR_H
#define ATTR_H

#include "Enums.h"
#include "Gnode.h"

class GNode;

class Attr{
protected:
	GNode* _owner;
	ElemType _elemType;
	int _key;

public:

	Attr(ElemType type, GNode* owner, int key);
	GNode* GetOwner();
	ElemType GetElemType();
	int GetKey();
};


template <class T>
class Attrx : public Attr{
protected:
	T _value;

	void OnAttrChanged(T old, T newAt);

public:
	Attrx(ElemType type, GNode owner, int key, T val);

	T GetValue();
	void SetValue(T val);
};

#endif
