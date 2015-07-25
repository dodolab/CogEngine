#pragma once

#include <vector>
#include "ofxSmartPointer.h"
#include "EnInputAct.h"

using namespace std;

/**
* Environment controller for device events (especially inputs)
*/
class MEnvironmentCtrl{
private:

	// flag for screen size change
	bool screenSizeChanged;
	// collection of actually pressed keys
	vector<EnInputAct> pressedKeys;
	// collection of actually pressed buttons
	vector<EnInputAct> pressedPoints;
	// screen width
	int width;
	// screen height
	int height;

public:
	/**
	* Initializes environment controller
	*/
	void Init();
	
	/**
	* Returns true, if screen size has changed
	*/
	bool ScreenSizeChanged(){
		return screenSizeChanged;
	}

	/**
	* Gets collection of currently pressed keys
	*/
	vector<EnInputAct>& GetPressedKeys(){
		return pressedKeys;
	}

	/**
	* Gets collection of currently pressed points
	*/
	vector<EnInputAct>& GetPressedPoints(){
		return pressedPoints;
	}

	/**
	* Gets screen width
	*/
	int GetWidth(){
		return width;
	}

	/**
	* Gets screen height
	*/
	int GetHeight(){
		return height;
	}

	/**
	* Gets width and height in 2D vector
	*/
	ofVec2f GetSize(){
		return ofVec2f(width, height);
	}

	/**
	* Handler for key action
	* @param key id of key
	* @param pressed indicates, if the key is pressed or released
	*/
	void OnKeyAction(int key, bool pressed);

	/**
	* Handler for screen size change
	* @param newWidth new screen width
	* @param newHeight new screen height
	*/
	void OnScreenSizeChanged(int newWidth, int newHeight);

	/**
	* Handler for multitouch press
	* @param x coordinate in x axis
	* @param y coordinate in y axis
	* @param button id of pressed button
	* @param pressed indicates, if the button is pressed or released
	*/
	void OnMultiTouchButton(int x, int y, int button, bool pressed);
	
	/**
	* Handler for multitouch motion
	* @param x coordinate in x axis
	* @param y coordinate in y axis
	* @param button id of button
	*/
	void OnMultiTouchMotion(int x, int y, int button);
	
	/**
	* Handler for single touch press
	* @param x coordinate in x axis
	* @param y coordinate in y axis
	* @param button id of pressed button
	* @param pressed indicates, if the button is pressed or released
	*/
	void OnSingleTouchButton(int x, int y, int button, bool pressed);
	
	/**
	* Handler for single touch motion
	* @param x coordinate in x axis
	* @param y coordinate in y axis
	* @param button id of pressed button
	*/
	void OnSingleTouchMotion(int x, int y, int button);

	/**
	* Removes inputs that already ended from collection
	*/
	void RemoveEndedInputs();
};
