
#ifndef ENINPUT_ACT_H
#define ENINPUT_ACT_H

template<class T>
class EnInputAct{
public:
	T value;
	// indicates, if input has been handled
	bool Handled = false;
	// indicates, if input has ended (e.g. key release)
	bool Ended = false;
	// indicates, how many cycle has been this input detected before it ended
	int CycleNumber = 0;


	EnInputAct(){

	}

	EnInputAct(T actValue){
		this->value = actValue;
	}

};


#endif
