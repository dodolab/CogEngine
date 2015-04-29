#include "Attr.h"

Attr::Attr() : _elemType(ElemType::ALL), _owner(nullptr), _key(0){

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

