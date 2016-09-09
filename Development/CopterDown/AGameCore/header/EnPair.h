
#ifndef ENPAIR_H
#define ENPAIR_H

/**
* EnPair - generic class for pairs
*/
template<class T>
class EnPair{
private:
	// first item
	T _item1;
	// second item
	T _item2;

public:

	EnPair(T item1, T item2) : _item1(item1), _item2(item2){

	}

	/**
	* Gets the first item
	*/
	T& GetItem1() const {
		return _item1;
	}

	/**
	* Sets the first item
	*/
	void SetItem1(T val){
		_item1 = val;
	}

	/**
	* Gets the second item
	*/
	T& GetItem2() const{
		return _item2;
	}

	/**
	* Sets the second item
	*/
	void SetItem2(T val){
		_item2 = val;
	}
};


#endif
