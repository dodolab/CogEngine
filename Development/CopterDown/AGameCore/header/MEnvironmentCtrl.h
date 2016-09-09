#ifndef INPUTCTRL_H
#define INPUTCTRL_H

#include "s3eTypes.h"
#include "s3eKeyboard.h"
#include <vector>
#include "s3ePointer.h"

using namespace std;

class MEnvironmentCtrl{
private:
	
	int32 PointerButtonEventCallback(s3ePointerEvent* event, void* userData);
	int32 PointerMotionEventCallback(s3ePointerMotionEvent* event, void* userData);
	int32 KeyEventCallback(s3eKeyboardEvent* event, void* userData);
	int32 ScreenSizeChangeCallback(void* systemData, void* userData);
	vector<s3eKey> _pressedKeys;

public:
	void Initialize();
	void UpdateInputs();
	void CheckInputs();
};


#endif
