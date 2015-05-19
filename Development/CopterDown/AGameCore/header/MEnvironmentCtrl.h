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
	static MEnvironmentCtrl* instance;

	// flag for screen size change
	bool screenSizeChanged = true;
	static int32 PointerButtonEventCallback(s3ePointerEvent* event, void* userData);
	static int32 PointerMotionEventCallback(s3ePointerMotionEvent* event, void* userData);
	static int32 KeyEventCallback(s3eKeyboardEvent* event, void* userData);
	static int32 ScreenSizeChangeCallback(void* systemData, void* userData);


public:

	vector<s3eKey> pressedKeys;
	spt<EnUserAct> userActions;
	int width;
	int height;

	void Initialize();
	void UpdateInputs();
	void CheckInputs();
};


#endif
