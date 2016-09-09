
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

	Attr(int key, ElemType type, GNode* owner);

	const GNode* GetOwner() const;

	const ElemType GetElemType() const;

	const int GetKey() const;
};


template <class  T>
class AttrRef : public Attr{
protected:
	T _value;
	
	void OnAttrChanged(T& old, T& newAt){
		auto value = std::make_pair<T, T>(old, newAt);
		Msg msg(this->_elemType, Actions::ATTRIBUTE_CHANGED, SenderType::ATTR, this->GetKey(), &value);
		this->_owner->SendMessageNoResp(msg, resp)
	}

public:

	AttrRef(){

	}

	AttrRef(int key, ElemType type, T& val, GNode* owner) : Attr(type, owner, key), _value(val){
		
	}

	~AttrRef(){ 
		//delete _value;
	}

	T& GetValue(){
		// pointer musn't be null!
		return (_value);
	}

	void SetValue(T& val){
		this->_value = val;
	}

};

template <class T>
class AttrPtr : public Attr{
protected:
	T* _value;

	void OnAttrChanged(T& old, T& newAt){
	  
	}

public:

	AttrPtr(){
	}

	AttrPtr(int key, ElemType type, T* val, GNode* owner) : Attr(type, owner, key), _value(val){
	
	}

	~AttrPtr(){
		delete _value;
	}

	T* GetValue(){
		return (_value);
	}

	void SetValue(T* val){
		delete _value;
		this->_value = val;
	}
};



#endif