#include "MEnvironmentCtrl.h"
#include "Enums.h"
#include "EnInputAct.h"
#include "s3eSurface.h"
#include "s3eTypes.h"
#include "s3ePointer.h"
#include "s3eKeyboard.h"
#include "MGameEngine.h"
#include "SmartPointer.h"
#include <vector>

using namespace std;


int32 MEnvironmentCtrl::KeyEventCallback(s3eKeyboardEvent* event, void* userData){

	if (event->m_Pressed){
		// key down

		// if pressed keys contains this key, remove it
		for (auto key : MEngine.environmentCtrl->GetPressedKeys()){
			// todo: shouldn't occur, create assertion
			if (key.key == event->m_Key) return 0;
		}

		MEngine.environmentCtrl->GetPressedKeys().push_back(EnInputAct(event->m_Key));
	}
	else{
		// key up
		for (auto it = MEngine.environmentCtrl->GetPressedKeys().begin(); it != MEngine.environmentCtrl->GetPressedKeys().end(); ++it){
			if ((*it).key == event->m_Key){
				(*it).ended = true;
				break;
			}
		}
	}

	return 0;
}

// user touches the screen with more fingers
void MEnvironmentCtrl::MultiTouchButtonCallback(s3ePointerTouchEvent* event){
	if (event->m_Pressed){
		for (auto key : MEngine.environmentCtrl->GetPressedPoints()){
			// todo: shouldn't occur, create assertion
			if (key.inputType == InputType::TOUCH && key.touchId == event->m_TouchID) return;
		}

		MEngine.environmentCtrl->GetPressedPoints().push_back(EnInputAct(event->m_TouchID, CIwFVec2(event->m_x, event->m_y)));
	}
	else{
		for (auto it = MEngine.environmentCtrl->GetPressedPoints().begin(); it != MEngine.environmentCtrl->GetPressedPoints().end(); ++it){
			if ((*it).inputType == InputType::TOUCH && (*it).touchId == event->m_TouchID){
				// change position as well
				(*it).position = CIwFVec2(event->m_x, event->m_y);
				(*it).ended = true;
				return;
			}
		}
	}

}

// user moves fingers
void MEnvironmentCtrl::MultiTouchMotionCallback(s3ePointerTouchMotionEvent* event){
	for (auto it = MEngine.environmentCtrl->GetPressedPoints().begin(); it != MEngine.environmentCtrl->GetPressedPoints().end(); ++it){
		// todo: shouldn't occur, create assertion
		if ((*it).inputType == InputType::TOUCH && (*it).touchId == event->m_TouchID){
			(*it).position = CIwFVec2(event->m_x, event->m_y);
		}
	}
}

// user touches the screen
void MEnvironmentCtrl::SingleTouchButtonCallback(s3ePointerEvent* event){
	if (event->m_Pressed){
		for (auto key : MEngine.environmentCtrl->GetPressedPoints()){
			// todo: shouldn't occur, create assertion
			if (key.inputType == InputType::MOUSE) return;
		}

		MEngine.environmentCtrl->GetPressedPoints().push_back(EnInputAct(event->m_Button, CIwFVec2(event->m_x, event->m_y)));
	}
	else{
		for (auto it = MEngine.environmentCtrl->GetPressedPoints().begin(); it != MEngine.environmentCtrl->GetPressedPoints().end(); ++it){
			if ((*it).inputType == InputType::MOUSE){
				// change position as well
				(*it).position = CIwFVec2(event->m_x, event->m_y);
				(*it).ended = true;
				return;
			}
		}
	}
}

// user moves finger
void MEnvironmentCtrl::SingleTouchMotionCallback(s3ePointerMotionEvent* event){
	for (auto it = MEngine.environmentCtrl->GetPressedPoints().begin(); it != MEngine.environmentCtrl->GetPressedPoints().end(); ++it){
		// todo: shouldn't occur, create assertion
		if ((*it).inputType == InputType::MOUSE){
			(*it).position = CIwFVec2(event->m_x, event->m_y);
		}
	}
}


int32 MEnvironmentCtrl::ScreenSizeChangeCallback(void* systemData, void* userData){
	MEngine.environmentCtrl->_screenSizeChanged = true;
	MEngine.environmentCtrl->_height = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);
	MEngine.environmentCtrl->_width = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
	return 0;
}

void MEnvironmentCtrl::Init(){
	_height = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);
	_width = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);

	s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, &MEnvironmentCtrl::ScreenSizeChangeCallback, NULL);

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
	}
}

void MEnvironmentCtrl::UpdateInputs(){
	//Update the input systems
	s3eKeyboardUpdate();
	s3ePointerUpdate();
}

void MEnvironmentCtrl::RemoveEndedInputs(){

	// remove unpressed keys
	for (auto it = GetPressedKeys().begin(); it != GetPressedKeys().end(); ++it){
		if ((*it).ended){
			GetPressedKeys().erase(it);
			break;
		}
	}

	// remove ended touches
	for (auto it = GetPressedPoints().begin(); it != GetPressedPoints().end(); ++it){
		if ((*it).ended){
			GetPressedPoints().erase(it);
			break;
		}
	}
}
