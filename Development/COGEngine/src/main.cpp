#include "ofMain.h"
#include "ofApp.h"
#include "MGameEngine.h"
#include "CopterFactory.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1280,720,OF_WINDOW);			// <-------- setup the GL context

		// initialize game engine
	MEngine.Init(new CopterFactory(), new MGameStorage());
	// start game loop
	MEngine.StartLoop();
	// finish
	MEngine.Terminate();


	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
