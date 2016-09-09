#include "MGameEngine.h"



int main()
{
	
	MEngine.Init();
	// register input events
	// start drawing loop
	MEngine.StartLoop();
	// finish
	MEngine.Terminate();

	return 0;
}
