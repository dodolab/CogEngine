#ifndef ATTR_H
#define ATTR_H


#include "EnPair.h"
#include "GMsg.h"


class GNode;

/**
* Attr - common class for generic attributes
*
*/
class GAttr{
protected:
	// owner node
	GNode* _owner = nullptr;
	// key identifier
	const int _key;

public:

	virtual ~GAttr()
	{

	}

	GAttr(int key, GNode* owner);

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
class GAttrR : public GAttr{
protected:
	T _value;
	
	/**
	* Event that occurs when an attribute has been changed
	* @param old old value
	* @param newAt new value
	*/
	void OnAttrChanged(T& old, T& newAt){
		/*EnPair<T, T> value = EnPair<T, T>(old, newAt);
		GMsg msg(ElemType::ALL, Actions::ATTRIBUTE_CHANGED, SenderType::ATTR, this->GetKey(), &value);
		this->_owner->SendMessageNoResp(msg);*/
	}

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
	GAttrR(int key, T val, GNode* owner) : GAttr(key, owner), _value(val){
		
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