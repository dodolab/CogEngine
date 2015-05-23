#ifndef INPUTCTRL_H
#define INPUTCTRL_H

#include "s3eTypes.h"
#include "s3eKeyboard.h"
#include <vector>
#include "s3ePointer.h"
#include "SmartPointer.h"
#include "EnUserAct.h"

using namespace std;

class MEnvironmentCtrl{
private:

	// flag for screen size change
	bool _screenSizeChanged = true;
	vector<s3eKey> _pressedKeys;
	spt<EnUserAct> _userActions;
	int _width;
	int _height;

	static int32 KeyEventCallback(s3eKeyboardEvent* event, void* userData);
	static int32 ScreenSizeChangeCallback(void* systemData, void* userData);

	// user touches the screen with more fingers
	static void MultiTouchButtonCallback(s3ePointerTouchEvent* event);
	// user moves fingers
	static void MultiTouchMotionCallback(s3ePointerTouchMotionEvent* event);
	// user touches the screen
	static void SingleTouchButtonCallback(s3ePointerEvent* event);
	// user moves finger
	static void SingleTouchMotionCallback(s3ePointerMotionEvent* event);

	// gets touch by id or creates a new one
	spt<EnInputAct<CIwFVec2>> GetTouchById(int id);

public:
	void Init();
	void UpdateInputs();
	void CheckInputs();

	bool ScreenSizeChanged(){
		return _screenSizeChanged;
	}

	vector<s3eKey>& GetPressedKeys(){
		return _pressedKeys;
	}

	spt<EnUserAct> GetUserActions(){
		return _userActions;
	}

	int GetWidth(){
		return _width;
	}

	int GetHeight(){
		return _height;
	}
};


#endif
