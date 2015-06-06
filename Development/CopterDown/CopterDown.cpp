#include "MGameEngine.h"



int main()
{

	// initialize 2D graphic system
	Iw2DInit();
	// dont use mipmaps -> better memory usage
	Iw2DSetUseMipMapping(false);

	// initialize game engine
	MEngine.Init(new MGameFactory(), new MGameStorage());
	// start game loop
	MEngine.StartLoop();
	// finish
	MEngine.Terminate();

	Iw2DTerminate();

	return 0;
}
