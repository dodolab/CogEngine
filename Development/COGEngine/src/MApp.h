#pragma once

#include "ofMain.h"
#include "MGameFactory.h"

#ifndef TARGET_WINDOWS
#include "ofxAndroid.h"
#endif

class MApp
#ifdef TARGET_WINDOWS
	:public ofBaseApp
#else
	: public ofxAndroidApp
#endif
{
protected:
	uint64 absolute;
	uint64 delta;
	MGameFactory* factory;

public:

	MApp(MGameFactory* factory) : factory(factory){

	}

	void setup();
	void draw();
	void update();

	void keyPressed(int key);
	void keyReleased(int key);
	void windowResized(int w, int h);


#ifdef TARGET_WINDOWS
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void dragEvent(ofDragInfo dragInfo);
#else
	void swipe(ofxAndroidSwipeDir swipeDir, int id);
	void pause();
	void stop();
	void resume();
	void reloadTextures();

	bool backPressed();
	void okPressed();
	void cancelPressed();

	void touchDown(int x, int y, int id);
	void touchMoved(int x, int y, int id);
	void touchUp(int x, int y, int id);
	void touchDoubleTap(int x, int y, int id);
	void touchCancelled(int x, int y, int id);
#endif
};
