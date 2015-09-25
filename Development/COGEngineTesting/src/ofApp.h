#pragma once

#include "ofMain.h"
#include "ofxSpriteSheetRenderer.h"
#include "MApp.h"

//a quick and dirty sprite implementation
struct basicSprite {
	ofPoint pos;			// the position on the screen this sprite will be drawn at
	float speed;			// the speed at which this sprite moves down the screen
	float rotation;
	int index;                    //								integer index into the sprite sheet to draw from. sprite sheet is numbered left to right, top to bottom
	int frame;                    // DEFAULT SHOULD BE SET TO 0.	which frame of the animation to draw. current tile is calculated as index+frame..
	int total_frames;             //								the total number of frames in the animation, or 1 if it is not animated

	float w;                      //								how many tiles right of the index are included
	float h; 
};

#pragma once


#ifndef TARGET_WINDOWS
#include "ofxAndroid.h"
#endif

class MTestApp : public MApp
{
protected:
	ofxSpriteSheetRenderer * spriteRenderer;	// our spriteRenderer
	vector<basicSprite *> sprites;				// our vector of sprites

public:

	MTestApp() : MApp(nullptr){

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
