#pragma once

#include "ofxAraPair.h"
#include "ofxAreMsg.h"


class ofxAreNode;

/**
* Common class for generic attributes
*
*/
class ofxAreAttr{
protected:
	// owner node
	ofxAreNode* owner;
	// key identifier
	const int key;

public:

	ofxAreAttr(int key, ofxAreNode* owner) : owner(owner), key(key){

	}

	virtual ~ofxAreAttr()
	{

	}

	/**
	* Gets owner node
	*/
	const ofxAreNode* GetOwner() const{
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
class ofxAreAttrR : public ofxAreAttr{
protected:
	// generic value
	T value;
	
public:

	~ofxAreAttrR()
	{

	}

	/**
	* Creates a new generic attribute
	* @param key attribute key identifier
	* @param val attribute value
	* @param owner owner node
	*/
	ofxAreAttrR(int key, T val, ofxAreNode* owner) : ofxAreAttr(key, owner), value(val){
		
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

