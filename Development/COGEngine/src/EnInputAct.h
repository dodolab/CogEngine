#pragma once

#include "MEnums.h"

/*! Type of device that invoked the input */
enum InputType{
	KEYBOARD,   /*!< keyboard */
	TOUCH		/*!< pointing device (finger or mouse) */
};

/**
* Entity for input activity
*/
class EnInputAct{
private:
	void Init(){
		handlerId = -1;
		started = true;
		ended = false;
	}

public:
	// input type 
	InputType inputType; 
	// pressed key (keyboard only)
	int key;
	// pressed finger (touch input only)
	int touchId;
	// position (touch input only)
	ofVec3f position;

	// handlerId, -1 if there is none
	int handlerId;
	// indicates, if input has just started
	bool started;
	// indicates, if input has ended (e.g. key release)
	bool ended;

	/**
	* Creates a new input action
	* @param keyboardKey pressed key
	*/
	EnInputAct(int keyboardKey) : inputType(InputType::KEYBOARD), key(keyboardKey){
		Init(); 
	}

	/**
	* Creates a new input action
	* @param touchId pressed button
	* @param pos button position
	*/
	EnInputAct(int touchId, ofVec3f pos) : inputType(InputType::TOUCH), touchId(touchId), position(pos){
		Init();
	}

	EnInputAct() : touchId(-1), key(-1){
		Init();
	}

	/**
	* Returns true, if the action has been already handled (by some behavior)
	*/
	bool IsHandled(){
		return handlerId != -1;
	}

};

