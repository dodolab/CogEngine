
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
	EnInputAct(T actValue);

	bool GetHandled();
	void SetHandled(bool val);

	bool GetEnded();
	void SetEnded(bool val);

	int GetCycleNumber();
	void SetCycleNumber(int val);
};


#endif
