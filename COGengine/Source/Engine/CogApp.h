#pragma once

#include "ofxCogCommon.h"

#ifdef ANDROID
#include "ofxAndroid.h"
#endif

namespace Cog {

	/**
	* Application wrapper for all platforms
	*/
	class CogApp
#ifdef WIN32
		:public ofBaseApp
#elif ANDROID
		: public ofxAndroidApp
#endif
	{
	protected:
		// time elapsed from engine initialization
		uint64 absolute;
		// time elapsed between frames
		uint64 delta;
		
	public:

		/**
		* Creates a new application wrapper
		*/
		CogApp() {
		}

		/**
		* Virtual method that can be overriden to initialize custom components
		*/
		virtual void InitComponents();

		/**
		* Virtual method that can be overriden to initialize engine with custom configuration
		*/
		virtual void InitEngine();

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


	#ifdef WIN32
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

}// namespace