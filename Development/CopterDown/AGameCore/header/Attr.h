
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
	Attrx(ElemType type, GNode* owner, int key, T val) : Attr(type, owner, key), _value(val){

	}

	T GetValue(){
		return _value;
	}

	void SetValue(T val){
		this->_value = val;
	}
};



Attr::Attr(ElemType type, GNode* owner, int key) : _elemType(type), _owner(owner), _key(key){

}

GNode* Attr::GetOwner(){
	return _owner;
}

ElemType Attr::GetElemType(){
	return _elemType;
}

int Attr::GetKey(){
	return _key;
}

#endif