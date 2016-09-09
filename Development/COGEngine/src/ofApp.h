#pragma once

#include "ofMain.h"
#include "Enums.h"
#include "ofxSpriteSheetRenderer.h"
#include "Utils.h"


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

class ofApp : public ofBaseApp{
	uint64 absolute;
	uint64 delta;

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofxSpriteSheetRenderer * spriteRenderer;	// our spriteRenderer
		vector<basicSprite *> sprites;				// our vector of sprites
};
