
#ifndef ENINPUT_ACT_H
#define ENINPUT_ACT_H

template<class T>
class EnInputAct{
private:
	T _actValue;
	bool _handled = false;
	bool _ended = false;
	int _cycleNumber = 0;

public:
	EnInputAct(){

	}

	EnInputAct(T& actValue){

	}

	bool GetHandled() const{
		return false;
	}

	void SetHandled(bool val){

	}

	bool GetEnded() const{
		return false;
	}

	void SetEnded(bool val){

	}

	int GetCycleNumber() const{
		return 0;
	}

	void SetCycleNumber(int val){

	}
};


#endif
