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
	static MEnvironmentCtrl* _instance;

	// flag for screen size change
	bool _screenSizeChanged = true;
	vector<s3eKey> _pressedKeys;
	spt<EnUserAct> _userActions;
	int _width;
	int _height;

	static int32 PointerButtonEventCallback(s3ePointerEvent* event, void* userData);
	static int32 PointerMotionEventCallback(s3ePointerMotionEvent* event, void* userData);
	static int32 KeyEventCallback(s3eKeyboardEvent* event, void* userData);
	static int32 ScreenSizeChangeCallback(void* systemData, void* userData);


public:

	static MEnvironmentCtrl* Get(){
		if (_instance == nullptr) _instance = new MEnvironmentCtrl();
		return _instance;
	}

	void Initialize();
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
