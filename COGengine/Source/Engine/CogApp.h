#pragma once

#include "ofxCogCommon.h"

#ifdef ANDROID
#include "ofxAndroid.h"
#endif

#define APP_SPEED 60


namespace Cog {

	class Stage;

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
		// frames per second
		uint64 fps;
		string configFile;
		// path to splash screen
		string splashScreen;
		spt<ofxXml> xmlConfig;
		bool engineInitialized = false;

	public:

		/**
		* Creates a new application wrapper
		*/
		CogApp() : configFile(""), fps(APP_SPEED), splashScreen(""){
		}

		CogApp(string configFile) : configFile(configFile), fps(APP_SPEED), splashScreen("") {
			ofxXml* xml = new ofxXml();
			xml->loadFile(configFile);
			this->xmlConfig = spt<ofxXml>(xml);
		}

		uint64 GetFps() {
			return fps;
		}

		void SetSplashScreenPath(string splashScreen) {
			this->splashScreen = splashScreen;
		}

		string GetSplashScreenPath() {
			return splashScreen;
		}

		/**
		* Virtual method that can be overriden to register custom components
		*/
		virtual void RegisterComponents() {

		}

		/**
		* Virtual method that can be overriden to initialize custom components
		*/
		virtual void InitComponents() {

		}

		/**
		* Virtual method that can be overriden to initialize engine with custom configuration
		*/
		virtual void InitEngine();

		virtual void InitStage(Stage* stage) {
			// nothing to do here
		}

		// setup function, called before first draw and update
		void setup();
		// setup function, called during the first draw and update
		void setupEngine();

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

		virtual bool OnBackPress() {
			return true;
		}

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

	private:
		void DrawSplashScreen();
	};

}// namespace