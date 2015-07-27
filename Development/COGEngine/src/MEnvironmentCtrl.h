#pragma once

#include <vector>
#include "ofxSmartPointer.h"
#include "EnInputAct.h"
#include "EnSound.h"

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
	// collection of played sounds
	vector<EnSound*> playedSounds;

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
	* Adds a new sound
	*/
	void AddSound(EnSound* sound){
		playedSounds.push_back(sound);
	}

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
	* Gets collection of currently played sounds
	*/
	vector<EnSound*>& GetPlayedSounds(){
		return playedSounds;
	}

	/**
	* Gets screen width
	*/
	int GetWidth(){
		//return width;
		// always returns 16:9 (TODO)
		if (width > height) return height / 9.0f*16.0f;
		else return height / 16.0f*9.0f;
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
		return ofVec2f(GetWidth(), GetHeight());
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
