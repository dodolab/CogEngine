
#ifndef ATTR_H
#define ATTR_H

#include "Enums.h"


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


template <class  T>
class Attrx : public Attr{
protected:
	T* _value;
	bool isEmpty;

	void OnAttrChanged(T& old, T& newAt){
		auto value = std::make_pair<T, T>(old, newAt);
		Msg msg(this->_elemType, Actions::ATTRIBUTE_CHANGED, SenderType::ATTR, this->GetKey(), &value);
		this->_owner->SendMessageNoResp(msg, resp)
	}

public:

	Attrx(){

	}

	Attrx(ElemType type, GNode* owner, int key, T& val) : Attr(type, owner, key), _value(val){
		
	}

	~Attrx(){
		delete _value;
	}

	T& GetValue(){
		// pointer musn't be null!
		return (*_value);
	}

	void SetValue(T& val){
		this->_value = val;
	}

	bool HasValue() const{
		return !isEmpty;
	}
};


#endif