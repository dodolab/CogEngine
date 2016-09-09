#include "s3e.h"
#include "IwDebug.h"
#include "unistd.h"
#include <iostream>
#include "MGameEngine.h"


MGameEngine engine;

// ============================== VARIABLES




// ============================== LOOPS





int main()
{
	

	// initialize	
	engine.Init();
	// register input events
	// start drawing loop
	engine.StartLoop();
	// finish
	engine.Terminate();

	return 0;
}
