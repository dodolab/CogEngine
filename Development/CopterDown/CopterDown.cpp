#include "s3e.h"
#include "IwDebug.h"
#include "Iw2DSceneGraph.h"
#include "IwGx.h"
#include "Abehavior.h"
#include "GNodePool.h"
#include "BeCollider.h"
#include "BeRender.h"
#include "BeRotateAnim.h"
#include "BeMove.h"
#include "BeTranslateAnim.h"
#include "EnBounds.h"
#include "EnCollision.h"
#include "EnInputAct.h"
#include "EnPair.h"
#include "EnUserAct.h"
#include "unistd.h"

using namespace Iw2DSceneGraphCore;
using namespace Iw2DSceneGraph;

#define MS_PER_SECOND 1000
#define MS_PER_MINUTE (MS_PER_SECOND * 60)
#define MS_PER_HOUR (MS_PER_MINUTE * 60)
#define MS_PER_DAY (MS_PER_HOUR * 24)

#define UPDATE_FREQUENCY 35

// ============================== VARIABLES

// flag for screen size change
bool screenSizeChanged = true;

s3eThread* updateThread;

int width;
int height;

// ============================== LOOPS
// FRAME_TIME is the amount of time that a single frame should last in seconds
#define FRAME_TIME  (30.0f / 1000.0f)


void DrawingLoop(){

	uint64 new_time = s3eTimerGetMs();

	while (!s3eDeviceCheckQuitRequest())
	{
		//Update the input systems
		s3eKeyboardUpdate();
		s3ePointerUpdate();
		

		// Clear the drawing surface
		Iw2DSurfaceClear(0xff000000);

		// Show the drawing surface
		Iw2DSurfaceShow();

		// sleep 0ms (for input events update)
		s3eDeviceYield(0);
		new_time = s3eTimerGetMs();
	}
}

// update loop runs on separate thread
void* UpdateLoop(void* arg){
	while (!s3eDeviceCheckQuitRequest())
	{
		uint32 beforeUpdate = (uint32)s3eTimerGetMs();

		// do logic here

		// sleep max 35 ms
		uint32 afterUpdate = (uint32)s3eTimerGetMs();
		long diff = (UPDATE_FREQUENCY - (afterUpdate - beforeUpdate)) * 1000;
		if (diff > 0) usleep(diff);
	}
	return NULL;
}

// ============================== HANDLERS

// Callback from S3E when the screen size or rotation changes
int32 ScreenSizeChangeCallback(void* systemData, void* userData)
{
	screenSizeChanged = true;
	return 0;
}

int32 KeyEventCallback(s3eKeyboardEvent* event, void* userData){
	return 0;
}

int32 PointerButtonEventCallback(s3ePointerEvent* event, void* userData){
	return 0;
}

// handled even when mouse button is not pressed !!
int32 PointerMotionEventCallback(s3ePointerMotionEvent* event, void* userData){
	return 0;
}

// ============================== INITIALIZATION

void RegisterEvents(){
	s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, ScreenSizeChangeCallback, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, (s3eCallback)KeyEventCallback, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)PointerButtonEventCallback, NULL);
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)PointerMotionEventCallback, NULL);
}

void Init(){

	// initialize 2D graphic system
	Iw2DInit();
	// initialize resources manager
	IwResManagerInit();
	// dont use mipmaps -> better memory usage
	Iw2DSetUseMipMapping(false);
	
	height = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);
	width = height = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
}

bool Check(){
	if (!s3eThreadAvailable())
	{
		s3eDebugErrorPrintf("s3eThread extension not present");
		return false;
	}
	return true;
}

bool StartUpdateThread(){
	updateThread = s3eThreadCreate(UpdateLoop, NULL, NULL);
	if (!updateThread)
	{
		s3eDebugErrorPrintf("s3eThreadCreate failed: %s", s3eThreadGetErrorString());
		return false;
	}
	return true;
}

void Terminate(){
	// terminate all used modules
	IwResManagerTerminate();
	Iw2DTerminate();

}

// ============================== MAIN METHOD

int main()
{
	// initialize
	Init();
	// register input events
	RegisterEvents();
	// check for failures
	if (!Check()) return 1;
	// start update thread
	if (!StartUpdateThread()) return 1;
	// start drawing loop
	DrawingLoop();
	// wait for update thread on quit callback
	s3eThreadJoin(updateThread, NULL);
	// finish
	Terminate();

	return 0;
}
