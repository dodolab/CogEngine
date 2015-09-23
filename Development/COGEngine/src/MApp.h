#pragma once

#include "ofMain.h"
#include "MFactory.h"

#ifndef TARGET_WINDOWS
#include "ofxAndroid.h"
#endif

/**
* Application wrapper for all platforms
*/
class MApp
#ifdef TARGET_WINDOWS
	:public ofBaseApp
#else
	: public ofxAndroidApp
#endif
{
protected:
	// time elapsed from engine initialization
	uint64 absolute;
	// time elapsed between frames
	uint64 delta;
	// link to default factory
	MFactory* factory;

	spt<ofxXmlSettings> config;

public:

	/**
	* Creates a new application wrapper
	* @param factory default factory
	*/
	MApp(MFactory* factory) : factory(factory){
	}

	/**
	* Creates a new application wrapper
	* @param factory default factory
	* @param config configuration XML
	*/
	MApp(MFactory* factory, spt<ofxXmlSettings> config) : factory(factory), config(config){
	}

	// setup function, called before first draw and update
	void setup();
	// drawing function
	void draw();
	// update function
	void update();

	// handler for key press
	void keyPressed(int key);
	// handler for key release
	void keyReleased(int key);
	// handler for window resize
	void windowResized(int w, int h);


#ifdef TARGET_WINDOWS
	// handler for mouse move
	void mouseMoved(int x, int y);
	// handler for mouse dragging
	void mouseDragged(int x, int y, int button);
	// handler for mouse press
	void mousePressed(int x, int y, int button);
	// handler for mouse release
	void mouseReleased(int x, int y, int button);
	// handler for drag event
	void dragEvent(ofDragInfo dragInfo);
#else
	// handler for android swipe
	void swipe(ofxAndroidSwipeDir swipeDir, int id);
	// handler for android pause
	void pause();
	// handler for android stop
	void stop();
	// handler for android resume
	void resume();
	// handler for reloading textures
	void reloadTextures();

	// handler for android backpress
	bool backPressed();
	// handler for android OK press
	void okPressed();
	// handler for android CANCEL press
	void cancelPressed();

	// handler for android touch down
	void touchDown(int x, int y, int id);
	// handler for android touch move
	void touchMoved(int x, int y, int id);
	// handler for android touch up
	void touchUp(int x, int y, int id);
	// handler for android touch double-tap
	void touchDoubleTap(int x, int y, int id);
	// handler for android touch-cancel
	void touchCancelled(int x, int y, int id);
#endif
};
