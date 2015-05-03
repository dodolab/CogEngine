
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

	const bool IsManaged() const;
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
		Msg msg(this->_elemType, Actions::ATTRIBUTE_CHANGED, SenderType::ATTR, this->GetKey(), &value);
		this->_owner->SendMessageNoResp(msg, resp)
	}

public:


	AttrR(int key, ElemType type, T& val, GNode* owner) : Attr(key, type, owner, false), _value(val){
		
	}

	~AttrR(){
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

/**
* AttrP - pointer attribute generic wrapper
*
*/
template <class  T>
class AttrP : public Attr{
protected:
	T* _value;

	void OnAttrChanged(T& old, T& newAt){
		auto value = std::make_pair<T, T>(old, newAt);
		Msg msg(this->_elemType, Actions::ATTRIBUTE_CHANGED, SenderType::ATTR, this->GetKey(), &value);
		this->_owner->SendMessageNoResp(msg, resp)
	}

public:


	AttrP(int key, ElemType type, T* val, GNode* owner, bool isManaged) : Attr(key, type, owner, isManaged), _value(val){

	}

	~AttrP(){
		if (_isManaged){
			delete &_value;
		}
	}

	T* GetValue(){
		// pointer musn't be null!
		return (_value);
	}

	void SetValue(T* val){
		this->_value = val;
	}

};



#endif