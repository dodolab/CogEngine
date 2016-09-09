
#ifndef ATTR_H
#define ATTR_H

#include "Enums.h"


class GNode;

/**
* Attr - common class for generic attributes
*
*/
class Attr{
protected:
	GNode* _owner;
	const ElemType _elemType;
	const int _key;
	const bool _isManaged;

public:

	Attr(int key, ElemType type, GNode* owner, bool isManaged);

	const GNode* GetOwner() const;

	const ElemType GetElemType() const;

	const int GetKey() const;
};


/**
* Attrt - attribute generic wrapper
*
*/
template <class  T>
class Attrt : public Attr{
protected:
	T _value;
	
	void OnAttrChanged(T& old, T& newAt){
		auto value = std::make_pair<T, T>(old, newAt);
		Msg msg(this->_elemType, Actions::ATTRIBUTE_CHANGED, SenderType::ATTR, this->GetKey(), &value);
		this->_owner->SendMessageNoResp(msg, resp)
	}

public:


	Attrt(int key, ElemType type, T& val, GNode* owner, bool isManaged) : Attr(type, owner, key, isManaged), _value(val){
		
	}

	~Attrt(){
		if (_isManaged){
			delete &_value;
		}
	}

	T& GetValue(){
		// pointer musn't be null!
		return (_value);
	}

	void SetValue(T& val){
		this->_value = val;
	}

};



#endif