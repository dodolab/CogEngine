#include "MEnvironmentCtrl.h"
#include "Enums.h"
#include "EnInputAct.h"
#include "s3eSurface.h"
#include "s3eTypes.h"
#include "s3ePointer.h"
#include "s3eKeyboard.h"

MEnvironmentCtrl* MEnvironmentCtrl::instance = new MEnvironmentCtrl();

int32 MEnvironmentCtrl::PointerButtonEventCallback(s3ePointerEvent* event, void* userData){
	return 0;
}

int32 MEnvironmentCtrl::PointerMotionEventCallback(s3ePointerMotionEvent* event, void* userData){
	return 0;
}


Act GetAction(s3eKey key){
	if (key == s3eKeyLeft) return Act::LEFT;
	if (key == s3eKeyRight) return Act::RIGHT;
	if (key == s3eKeySpace) return Act::FIRE;
	if (key == s3eKeyTab) return Act::SWITCH;
	return Act::FIRE;
}



int32 MEnvironmentCtrl::KeyEventCallback(s3eKeyboardEvent* event, void* userData){
	if (event->m_Pressed){
		// key down


		// if pressed keys contains this key, do nothing
		for (auto key : instance->pressedKeys){
			if (key == event->m_Key) return 0;
		}

		instance->pressedKeys.push_back(event->m_Key);
		Act inAct = GetAction(event->m_Key);

		bool alreadyContains = false;
		for (EnInputAct<Act> act : instance->userActions->GetKeyActions()){
			if (act.value == inAct){
				alreadyContains = true;
				break;
			}
		}

		if (!alreadyContains) instance->userActions->GetKeyActions().push_back(EnInputAct<Act>(inAct));
	}
	else{
		// key up
		for (auto it = instance->pressedKeys.begin(); it != instance->pressedKeys.end(); ++it){
			if ((*it) == event->m_Key){
				instance->pressedKeys.erase(it);
				break;
			}
		}

		Act inAct = GetAction(event->m_Key);

		for (EnInputAct<Act> act : instance->userActions->GetKeyActions()){
			if (act.value == inAct){
				act.ended = true;
				break;
			}
		}
	}

	return 0;
}

int32 MEnvironmentCtrl::ScreenSizeChangeCallback(void* systemData, void* userData){
	instance->screenSizeChanged = true;
	return 0;
}

void MEnvironmentCtrl::Initialize(){
	height = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);
	width = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
	userActions = spt<EnUserAct>(new EnUserAct());

	s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, &MEnvironmentCtrl::ScreenSizeChangeCallback, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, (s3eCallback) &MEnvironmentCtrl::KeyEventCallback, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)&MEnvironmentCtrl::PointerButtonEventCallback, NULL);
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)&MEnvironmentCtrl::PointerMotionEventCallback, NULL);
}

void MEnvironmentCtrl::UpdateInputs(){

}

void MEnvironmentCtrl::CheckInputs(){
	auto actions = userActions->GetKeyActions();
	auto points = userActions->GetPointActions();

	for (auto it = actions.begin(); it != actions.end(); /*nothing*/){
		EnInputAct<Act>& act = (*it);
		if (act.ended || act.handled){
			actions.erase(it);
		}
		else{
			act.cycleNumber++;
			++it;
		}
	}

	for (auto it = points.begin(); it != points.end(); /*nothing*/){
		EnInputAct<CIwFVec2> act = (*it);
		if (act.ended || act.handled){
			points.erase(it);
		}
		else{
			act.cycleNumber++;
			++it;
		}
	}
}