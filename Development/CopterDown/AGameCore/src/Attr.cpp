#include "Attr.h"


Attr::Attr(int key, ElemType type, GNode* owner, bool isManaged) : _elemType(type), _owner(owner), _key(key), _isManaged(isManaged){

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

const bool Attr::IsManaged() const{
	return _isManaged;
}

