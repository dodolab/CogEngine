
#ifndef ENINPUT_ACT_H
#define ENINPUT_ACT_H

/**
* EnInputAct - entity for input activity
*/
template<class T>
class EnInputAct{
public:
	T value;
	// indicates, if input has just started (only for pointer events)
	// if false, it means that pointer is in motion
	bool started;
	// indicates, if input has been handled
	bool handled = false;
	// indicates, if input has ended (e.g. key release)
	bool ended = false;
	// indicates, how many cycle has been this input detected before it ended
	int cycleNumber = 0;


	EnInputAct(){

	}

	EnInputAct(T actValue){
		this->value = actValue;
	}

};


#endif
