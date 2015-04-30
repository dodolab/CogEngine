#include "Attr.h"

Attr::Attr() : _elemType(ElemType::ALL), _owner(nullptr), _key(0){

}

Attr::Attr(int key, ElemType type, GNode* owner) : _elemType(type), _owner(owner), _key(key){

}

const GNode* Attr::GetOwner() const{
	return _owner;
}

const ElemType Attr::GetElemType() const{
	return _elemType;
}

const int Attr::GetKey() const{
	return _key;
}

