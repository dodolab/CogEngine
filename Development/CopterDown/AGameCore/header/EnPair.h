
#ifndef ENPAIR_H
#define ENPAIR_H

template<class T>
class EnPair{
private:
	T _item1;
	T _item2;

public:

	EnPair(T item1, T item2) : _item1(item1), _item2(item2){

	}

	T GetItem1() const {
		return _item1;
	}

	void SetItem1(T val){
		_item1 = val;
	}

	T GetItem2() const{
		return _item2;
	}

	void SetItem2(T val){
		_item2 = val;
	}
};


#endif
