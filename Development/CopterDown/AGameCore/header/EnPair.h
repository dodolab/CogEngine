
#ifndef ENPAIR_H
#define ENPAIR_H

template<class T>
class Pair1{
private:
	T* _item1;
	T* _item2;

public:

	Pair1(T* item1, T* item2);

	T* GetItem1();
	void SetItem1(T* val);

	T* GetItem2();
	void SetItem2(T* val);
};

template<class T, class M>
class Pair2{
private:
	T* _item1;
	M* _item2;

public:

	Pair2(T* item1, M* item2);

	T* GetItem1();
	void SetItem1(T* val);

	M* GetItem2();
	void SetItem2(M* val);
};


#endif
