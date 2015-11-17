#pragma once

#include "CogPair.h"
#include "CogMsg.h"


class CogNode;

/**
* Common class for generic attributes
*
*/
class Attr{
protected:
	// owner node
	CogNode* owner;
	// key identifier
	const int key;

public:

	Attr(int key, CogNode* owner) : owner(owner), key(key){

	}

	virtual ~Attr()
	{

	}

	/**
	* Gets owner node
	*/
	const CogNode* GetOwner() const{
		return owner;
	}

	/**
	* Gets key identifier
	*/
	const int GetKey() const{
		return key;
	}
};


/**
* Attribute generic wrapper
*
*/
template <class  T>
class AttrR : public Attr{
protected:
	// generic value
	T value;
	
public:

	~AttrR()
	{

	}

	/**
	* Creates a new generic attribute
	* @param key attribute key identifier
	* @param val attribute value
	* @param owner owner node
	*/
	AttrR(int key, T val, CogNode* owner) : Attr(key, owner), value(val){
		
	}

	/**
	* Gets reference to the attribute value
	*/
	T& GetValue(){
		return (value);
	}

	/**
	* Sets attribute value
	*/
	void SetValue(T val){
		T& temp = value;
		this->value = val;
	}

};

