
#ifndef INPUTCTRL_H
#define INPUTCTRL_H



void UpdateInputs();

void CheckInputs();

// Callback from S3E when the screen size or rotation changes
int32 ScreenSizeChangeCallback(void* systemData, void* userData);
vector<s3eKey> pressedKeys;



int32 PointerButtonEventCallback(s3ePointerEvent* event, void* userData);
// handled even when mouse button is not pressed !!
int32 PointerMotionEventCallback(s3ePointerMotionEvent* event, void* userData);
int32 KeyEventCallback(s3eKeyboardEvent* event, void* userData);

// ============================== INITIALIZATION

void RegisterEvents();

#endif
