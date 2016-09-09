#include "ofxArcEnvironmentCtrl.h"
#include "ofxAraInputAct.h"
#include "ofxAreEngine.h"
#include "ofxAreUtils.h"


void MofxArcEnvironmentCtrl::Init(){
	screenSizeChanged = false;
	realWidth = width = ofGetWindowSize().x;
	realHeight = height = ofGetWindowSize().y;
	aspectRatio = ((float)realWidth) / realHeight;
	ReinitAspectRatio();
}

void MofxArcEnvironmentCtrl::OnScreenSizeChanged(int newWidth, int newHeight){
	screenSizeChanged = true;
	realHeight = newHeight;
	realWidth = newWidth;
	ReinitAspectRatio();
}


void MofxArcEnvironmentCtrl::OnKeyAction(int key, bool pressed){

	if (pressed){
		// key down

		// if pressed keys contains this key, remove it
		for (auto pKey : pressedKeys){
			// key is already in the collection of pressed keys
			if (pKey.key == key) return;
		}

		pressedKeys.push_back(ofxAraInputAct(key));
	}
	else{
		// key up
		for (auto it = pressedKeys.begin(); it != pressedKeys.end(); ++it){
			if ((*it).key == key){
				(*it).ended = true;
				break;
			}
		}
	}
}

void MofxArcEnvironmentCtrl::OnMultiTouchButton(int x, int y, int button, bool pressed){
	// user touches the screen with more fingers

	// todo: scale to format
	if (width != GetWidth()) x /= (float)(width / ((float)GetWidth()));

	if (pressed){
		pressedPoints.push_back(ofxAraInputAct(button, ofVec3f(x, y)));
	}
	else{
		for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it){
			if ((*it).inputType == InputType::TOUCH && (*it).touchId == button){
				// change position
				(*it).position = ofVec3f(x, y);
				(*it).ended = true;
				return;
			}
		}
	}
}

void MofxArcEnvironmentCtrl::OnMultiTouchMotion(int x, int y, int button){
	// user moves fingers
	// todo: scale to format
	if (width != GetWidth()) x /= (float)(width / ((float)GetWidth()));

	for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it){
		if ((*it).touchId == button && (*it).inputType == InputType::TOUCH){
			(*it).position = ofVec3f(x, y);
		}
	}
}

void MofxArcEnvironmentCtrl::OnSingleTouchButton(int x, int y, int button, bool pressed){
	// user touches the screen

	// todo: scale to format
	if (width != GetWidth()) x /= (float)(width / ((float)GetWidth()));

	if (pressed){
		pressedPoints.push_back(ofxAraInputAct(button, ofVec3f(x, y)));
	}
	else{
		for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it){
			if ((*it).inputType == InputType::TOUCH){
				// change position
				(*it).position = ofVec3f(x,y);
				(*it).ended = true;
				return;
			}
		}
	}
}

void MofxArcEnvironmentCtrl::OnSingleTouchMotion(int x, int y, int button){
	// todo: scale to format
	if (width != GetWidth()) x /= (float)(width / ((float)GetWidth()));

	// user moves finger
	for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it){
		if ((*it).touchId == button && (*it).inputType == InputType::TOUCH){
			(*it).position = ofVec3f(x,y);
		}
	}
}

void MofxArcEnvironmentCtrl::RemoveEndedProcesses(){
	screenSizeChanged = false;

	// remove released keys
	for (auto it = pressedKeys.begin(); it != pressedKeys.end();){
		if ((*it).ended){
			it = pressedKeys.erase(it);
			continue;
		}else if ((*it).started){
			(*it).started = false;
		}
		// increment only if item hasn't been erased
		++it;
	}

	// remove released touches
	for (auto it = pressedPoints.begin(); it != pressedPoints.end();){
		if ((*it).ended){
			it = pressedPoints.erase(it);
			continue;
		} else if ((*it).started){
			(*it).started = false;
		}
		// increment only if item hasn't been erased
		++it;
	}
	
	// remove ended sounds
	for (auto it = playedSounds.begin(); it != playedSounds.end();){
		if ((*it)->Ended()){
			it = playedSounds.erase(it);
			
			continue;
		}
		// increment only if item hasn't been erased
		++it;
	}

	// remove ended threads
	for (auto it = runningThreads.begin(); it != runningThreads.end();){
		if (!(*it)->isThreadRunning()){
			delete (*it);
			it = runningThreads.erase(it);
			continue;
		}
		// increment only if item hasn't been erased
		++it;
	}

}

void MofxArcEnvironmentCtrl::RunThread(ofThread* thread){
	runningThreads.push_back(thread);
	thread->startThread();
}
