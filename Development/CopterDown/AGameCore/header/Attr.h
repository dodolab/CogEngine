
#ifndef ATTR_H
#define ATTR_H

#include "Enums.h"
#include "Gnode.h"

class GNode;

class Attr{
protected:
	GNode* _owner;
	const ElemType _elemType;
	const int _key;

public:
	Attr();

	Attr(ElemType type, GNode* owner, int key);

	const GNode* GetOwner() const;

	ElemType GetElemType() const;

	int GetKey() const;
};


template <typename  T>
class Attrx : public Attr{
protected:
	T* _value;
	bool isEmpty;
	void OnAttrChanged(T& old, T& newAt);

public:
	Attrx() : Attr(){

	}

	Attrx(ElemType type, GNode* owner, int key, T& val) : Attr(type, owner, key), _value(val){

	}

	T& GetValue(){
		return *_value;
	}

	void SetValue(T& val){
		this->_value = val;
	}

	bool HasValue() const{
		return !isEmpty;
	}
};

Attr::Attr() : _elemType(ElemType::ALL), _key(0){

}

Attr::Attr(ElemType type, GNode* owner, int key) : _elemType(type), _owner(owner), _key(key){

}

const GNode* Attr::GetOwner() const{
	return _owner;
}

ElemType Attr::GetElemType() const{
	return _elemType;
}

int Attr::GetKey() const{
	return _key;
}

#endif