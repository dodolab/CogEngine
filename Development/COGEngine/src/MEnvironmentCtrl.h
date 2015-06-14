#ifndef INPUTCTRL_H
#define INPUTCTRL_H

#include <vector>
#include "SmartPointer.h"
#include "EnInputAct.h"

using namespace std;

class MEnvironmentCtrl{
private:

	// flag for screen size change
	bool _screenSizeChanged;
	vector<EnInputAct> _pressedKeys;
	vector<EnInputAct> _pressedPoints;
	int _width;
	int _height;

	// gets touch by id or creates a new one
	spt<EnInputAct> GetTouchById(int id);

public:
	void Init();
	void Terminate();
	void UpdateInputs();
	void RemoveEndedInputs();

	bool ScreenSizeChanged(){
		return _screenSizeChanged;
	}

	vector<EnInputAct>& GetPressedKeys(){
		return _pressedKeys;
	}

	vector<EnInputAct>& GetPressedPoints(){
		return _pressedPoints;
	}

	int GetWidth(){
		return _width;
	}

	int GetHeight(){
		return _height;
	}

	void OnKeyAction(int key, bool pressed);
	void OnScreenSizeChanged(int newWidth, int newHeight);

	// user touches the screen with more fingers
	void OnMultiTouchButton(int x, int y);
	// user moves fingers
	void OnMultiTouchMotion(int x, int y);
	// user touches the screen
	void OnSingleTouchButton(int x, int y, int button, bool pressed);
	// user moves finger
	void OnSingleTouchMotion(int x, int y);
};


#endif
