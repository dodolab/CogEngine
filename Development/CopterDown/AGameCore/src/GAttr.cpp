#include "GAttr.h"


GAttr::GAttr(int key, GNode* owner) : _owner(owner), _key(key){

}

const GNode* GAttr::GetOwner() const{
	return _owner;
}

const int GAttr::GetKey() const{
	return _key;
}

