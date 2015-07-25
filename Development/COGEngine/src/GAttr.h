#pragma once

#include "EnPair.h"
#include "GMsg.h"


class GNode;

/**
* Common class for generic attributes
*
*/
class GAttr{
protected:
	// owner node
	GNode* owner;
	// key identifier
	const int key;

public:

	GAttr(int key, GNode* owner) : owner(owner), key(key){

	}

	virtual ~GAttr()
	{

	}

	/**
	* Gets owner node
	*/
	const GNode* GetOwner() const{
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
class GAttrR : public GAttr{
protected:
	// generic value
	T value;
	
public:

	~GAttrR()
	{

	}

	/**
	* Creates a new generic attribute
	* @param key attribute key identifier
	* @param val attribute value
	* @param owner owner node
	*/
	GAttrR(int key, T val, GNode* owner) : GAttr(key, owner), value(val){
		
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

