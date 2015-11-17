#pragma once

#include "ofxCogMain.h"
#include "CogInputAct.h"
#include "CogSoundFile.h"


/**
* Environment controller for device events (especially inputs)
*/
class CogEnvironment{
private:

	// flag for screen size change
	bool screenSizeChanged;
	// collection of actually pressed keys
	vector<CogInputAct> pressedKeys;
	// collection of actually pressed buttons
	vector<CogInputAct> pressedPoints;
	// collection of played sounds
	vector<spt<CogSound>> playedSounds;

	// scaled screen width
	int width;
	// scaled screen height
	int height;
	// real screen width
	int realWidth;
	// real screen height
	int realHeight;
	// virtual aspect ratio
	float aspectRatio;
	// collection of running threads
	vector<ofThread*> runningThreads;
public:


	/**
	* Initializes environment controller
	*/
	void Init();

	/**
	* Adds a new sound
	*/
	void AddSound(spt<CogSound> sound){
		playedSounds.push_back(sound);
	}

	/**
	* Plays sound and adds it to the collection
	*/
	void PlaySound(spt<CogSound> sound){
		sound->Play();
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
	vector<CogInputAct>& GetPressedKeys(){
		return pressedKeys;
	}

	/**
	* Gets collection of currently pressed points
	*/
	vector<CogInputAct>& GetPressedPoints(){
		return pressedPoints;
	}

	/**
	* Gets collection of currently played sounds
	*/
	vector<spt<CogSound>>& GetPlayedSounds(){
		return playedSounds;
	}

	/**
	* Gets ratio of virtual size / real size
	*/
	float GetRatioScale(){
		return (aspectRatio) / (((float)realWidth) / realHeight);
	}

	/**
	* Gets real device width
	*/
	int GetRealWidth(){
		return realWidth;
	}

	/**
	* Gets real device height
	*/
	int GetRealHeight(){
		return realHeight;
	}

	/**
	* Gets real aspect ratio, calculated from real devide width and height
	*/
	float GetRealAspectRatio(){
		return ((float)realWidth / realHeight);
	}

	/**
	* Gets virtual aspect ratio
	*/
	float GetAspectRatio(){
		return aspectRatio;
	}

	/**
	* Sets aspect ratio
	*/
	void SetAspectRatio(float ratio){
		this->aspectRatio = ratio;

		ReinitAspectRatio();
	}

	/**
	* Gets screen virtual width
	*/
	int GetWidth(){
		return width;
	}

	/**
	* Gets screen virtual height
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
	* Removes processes that already ended from collection
	*/
	void RemoveEndedProcesses();

	/**
	* Runs a new thread
	* @param thread thread to run
	*/
	void RunThread(ofThread* thread);

protected:

	/**
	* Reinitializes virtual width and height
	* according to the actual aspect ratio
	*/
	void ReinitAspectRatio(){
		if (abs(GetAspectRatio() - GetRealAspectRatio()) > 0.1f){

			if (realWidth <= realHeight){
				width = GetRatioScale()*realWidth;
				height = realHeight;
			}
			else {
				width = realWidth;
				height = GetRatioScale()*realHeight;
			}
		}
	}
};
