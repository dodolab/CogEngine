
#ifndef ENINPUT_ACT_H
#define ENINPUT_ACT_H

template<class T>
class EnInputAct{
private:
	T _actValue;
	bool _handled;
	bool _ended;
	int _cycleNumber;

public:
	EnInputAct();

	EnInputAct(T& actValue);

	bool GetHandled() const;
	void SetHandled(bool val);

	bool GetEnded() const;
	void SetEnded(bool val);

	int GetCycleNumber() const;
	void SetCycleNumber(int val);
};

// need to be implemented
template<class T>
EnInputAct<T>::EnInputAct(){

}


#endif
