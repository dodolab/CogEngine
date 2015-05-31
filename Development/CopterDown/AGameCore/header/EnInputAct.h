#include "IwGeomFVec2.h"
#include "s3eKeyboard.h"
#include "s3ePointer.h"

#ifndef ENINPUT_ACT_H
#define ENINPUT_ACT_H

enum InputType{
	KEYBOARD, MOUSE, TOUCH
};

/**
* EnInputAct - entity for input activity
*/
class EnInputAct{
public:
	InputType inputType;
	// pressed key (keyboard only)
	s3eKey key;
	// pressed mouse button (mouse input only)
	s3ePointerButton mouseButton;
	// pressed finger (touch input only)
	int touchId;
	// position (only for mouse and touch events)
	CIwFVec2 position = CIwFVec2::g_Zero;

	// handlerId, -1 if there is none
	int handlerId = -1;
	// indicates, if input has just started (only for pointer events)
	// if false, it means that pointer is in motion
	bool started = true;
	// indicates, if input has ended (e.g. key release)
	bool ended = false;


	EnInputAct(s3eKey keyboardKey) : inputType(InputType::KEYBOARD), key(keyboardKey){

	}

	EnInputAct(s3ePointerButton mouseButt, CIwFVec2 pos) : inputType(InputType::MOUSE), mouseButton(mouseButt), position(pos){

	}

	EnInputAct(int touchID, CIwFVec2 pos) :inputType(InputType::TOUCH), touchId(touchID),  position(pos){

	}

	EnInputAct(){
		
	}

	// indicates, if input has been handled
	bool IsHandled(){
		return handlerId != -1;
	}

};


#endif
