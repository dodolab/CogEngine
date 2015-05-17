
#ifndef ATTR_H
#define ATTR_H


#include "EnPair.h"
#include "Msg.h"


class GNode;

/**
* Attr - common class for generic attributes
*
*/
class Attr{
protected:
	// owner node
	GNode* _owner = nullptr;
	// key identifier
	const int _key;

public:

	virtual ~Attr()
	{

	}

	Attr(int key, GNode* owner);

	/**
	* Gets owner node
	*/
	const GNode* GetOwner() const;

	/**
	* Gets key identifier
	*/
	const int GetKey() const;
};


/**
* AttrR - attribute generic wrapper
*
*/
template <class  T>
class AttrR : public Attr{
protected:
	T _value;
	
	/**
	* Event that occurs when an attribute has been changed
	* @param old old value
	* @param newAt new value
	*/
	void OnAttrChanged(T& old, T& newAt){
		/*EnPair<T, T> value = EnPair<T, T>(old, newAt);
		Msg msg(ElemType::ALL, Actions::ATTRIBUTE_CHANGED, SenderType::ATTR, this->GetKey(), &value);
		this->_owner->SendMessageNoResp(msg);*/
	}

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
	AttrR(int key, T val, GNode* owner) : Attr(key, owner), _value(val){
		
	}

	/**
	* Gets reference to the attribute value
	*/
	T& GetValue(){
		return (_value);
	}

	/**
	* Sets attribute value
	*/
	void SetValue(T val){
		T& temp = _value;
		this->_value = val;
		OnAttrChanged(temp, val);
	}

};




#endif