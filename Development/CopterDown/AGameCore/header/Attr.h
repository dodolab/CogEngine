
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
	GNode* _owner = nullptr;
	const int _key;

public:
	virtual ~Attr()
	{

	}

	Attr(int key, GNode* owner);

	const GNode* GetOwner() const;

	const int GetKey() const;
};


/**
* AttrR - reference attribute generic wrapper
*
*/
template <class  T>
class AttrR : public Attr{
protected:
	T _value;
	
	void OnAttrChanged(T& old, T& newAt){
		auto value = std::make_pair<T, T>(old, newAt);
		Msg msg(ElemType::ALL, Actions::ATTRIBUTE_CHANGED, SenderType::ATTR, this->GetKey(), &value);
		this->_owner->SendMessageNoResp(msg, resp)
	}

public:

	~AttrR()
	{

	}

	AttrR(int key, T& val, GNode* owner) : Attr(key, owner), _value(val){
		
	}

	T& GetValue(){
		return (_value);
	}

	void SetValue(T& val){
		this->_value = val;
	}

};




#endif