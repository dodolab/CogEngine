#include "MEnvironmentCtrl.h"
#include "Enums.h"
#include "EnInputAct.h"
#include "MGameEngine.h"
#include "SmartPointer.h"
#include <vector>

using namespace std;


void MEnvironmentCtrl::OnKeyAction(int key, bool pressed){

	if (pressed){
		// key down

		// if pressed keys contains this key, remove it
		for (auto pKey : MEngine.environmentCtrl->GetPressedKeys()){
			// todo: shouldn't occur, create assertion
			if (pKey.key == key) return;
		}

		MEngine.environmentCtrl->GetPressedKeys().push_back(EnInputAct(key));
	}
	else{
		// key up
		for (auto it = MEngine.environmentCtrl->GetPressedKeys().begin(); it != MEngine.environmentCtrl->GetPressedKeys().end(); ++it){
			if ((*it).key == key){
				(*it).ended = true;
				break;
			}
		}
	}

	return;
}

// user touches the screen with more fingers
void MEnvironmentCtrl::OnMultiTouchButton(int x, int y){
/*	if (event->m_Pressed){
		for (auto key : MEngine.environmentCtrl->GetPressedPoints()){
			// todo: shouldn't occur, create assertion
			if (key.inputType == InputType::TOUCH && key.touchId == event->m_TouchID) return;
		}

		MEngine.environmentCtrl->GetPressedPoints().push_back(EnInputAct(event->m_TouchID, ofVec3f(event->m_x, event->m_y)));
	}
	else{
		for (auto it = MEngine.environmentCtrl->GetPressedPoints().begin(); it != MEngine.environmentCtrl->GetPressedPoints().end(); ++it){
			if ((*it).inputType == InputType::TOUCH && (*it).touchId == event->m_TouchID){
				// change position as well
				(*it).position = ofVec3f(event->m_x, event->m_y);
				(*it).ended = true;
				return;
			}
		}
	}
	*/
}

// user moves fingers
void MEnvironmentCtrl::OnMultiTouchMotion(int x, int y){
	/*for (auto it = MEngine.environmentCtrl->GetPressedPoints().begin(); it != MEngine.environmentCtrl->GetPressedPoints().end(); ++it){
		// todo: shouldn't occur, create assertion
		if ((*it).inputType == InputType::TOUCH && (*it).touchId == event->m_TouchID){
			(*it).position = ofVec3f(event->m_x, event->m_y);
		}
	}*/
}

// user touches the screen
void MEnvironmentCtrl::OnSingleTouchButton(int x, int y, int button, bool pressed){
	if (pressed){
		for (auto key : MEngine.environmentCtrl->GetPressedPoints()){
			// todo: shouldn't occur, create assertion
			if (key.inputType == InputType::MOUSE) return;
		}

		MEngine.environmentCtrl->GetPressedPoints().push_back(EnInputAct(button, ofVec3f(x,y)));
	}
	else{
		for (auto it = MEngine.environmentCtrl->GetPressedPoints().begin(); it != MEngine.environmentCtrl->GetPressedPoints().end(); ++it){
			if ((*it).inputType == InputType::MOUSE){
				// change position as well
				(*it).position = ofVec3f(x,y);
				(*it).ended = true;
				return;
			}
		}
	}
}

// user moves finger
void MEnvironmentCtrl::OnSingleTouchMotion(int x, int y){
	for (auto it = MEngine.environmentCtrl->GetPressedPoints().begin(); it != MEngine.environmentCtrl->GetPressedPoints().end(); ++it){
		// todo: shouldn't occur, create assertion
		if ((*it).inputType == InputType::MOUSE){
			(*it).position = ofVec3f(x,y);
		}
	}
}


void MEnvironmentCtrl::OnScreenSizeChanged(int newWidth, int newHeight){
	MEngine.environmentCtrl->_screenSizeChanged = true;
	MEngine.environmentCtrl->_height = newHeight;
	MEngine.environmentCtrl->_width = newWidth;
}

void MEnvironmentCtrl::Init(){
	_screenSizeChanged = false;

	_height = ofGetWindowSize().y;
	_width = ofGetWindowSize().x;

	/*s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, &MEnvironmentCtrl::ScreenSizeChangeCallback, NULL);

	// Determine if the device supports multi-touch
	bool isMultiTouch = s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE);

	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, (s3eCallback)&MEnvironmentCtrl::KeyEventCallback, NULL);

	// For multi-touch devices we handle touch and motion events using different callbacks
	if (isMultiTouch)
	{
		s3ePointerRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)&MEnvironmentCtrl::MultiTouchButtonCallback, NULL);
		s3ePointerRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)&MEnvironmentCtrl::MultiTouchMotionCallback, NULL);
	}
	else
	{
		s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)&MEnvironmentCtrl::SingleTouchButtonCallback, NULL);
		s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)&MEnvironmentCtrl::SingleTouchMotionCallback, NULL);
	}*/
}

void MEnvironmentCtrl::Terminate(){

}

void MEnvironmentCtrl::UpdateInputs(){
	//Update the input systems
	//s3eKeyboardUpdate();
	//s3ePointerUpdate();
}

void MEnvironmentCtrl::RemoveEndedInputs(){

	// remove unpressed keys
	for (auto it = GetPressedKeys().begin(); it != GetPressedKeys().end(); ++it){
		if ((*it).ended){
			GetPressedKeys().erase(it);
			break;
		}

		if ((*it).started){
			(*it).started = false;
		}
	}

	// remove ended touches
	for (auto it = GetPressedPoints().begin(); it != GetPressedPoints().end(); ++it){
		if ((*it).ended){
			GetPressedPoints().erase(it);
			break;
		}

		if ((*it).started){
			(*it).started = false;
		}
	}
}
