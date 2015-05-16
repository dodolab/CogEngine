#include "Attr.h"


Attr::Attr(int key, GNode* owner) : _owner(owner), _key(key){

}

const GNode* Attr::GetOwner() const{
	return _owner;
}

const int Attr::GetKey() const{
	return _key;
}

