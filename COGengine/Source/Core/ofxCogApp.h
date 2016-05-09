#pragma once

#include "ofBaseApp.h"
#include "Definitions.h"
#include "ofxXmlSettings.h"


#ifdef ANDROID
#include "ofxAndroid.h"
#endif

namespace Cog {

	class Stage;

	/**
	* Application ofApp wrapper for both Android and Windows platforms
	*/
	class ofxCogApp
#ifdef WIN32
		:public ofBaseApp
#elif ANDROID
		: public ofxAndroidApp
#endif
	{
	protected:
		// time elapsed since engine initialization
		uint64 absolute;
		// time elapsed since last frame
		uint64 delta;
		// frames per second
		uint64 fps;
		// path to config file
		string configFile;
		// xml configuration
		spt<ofxXml> xmlConfig;
		// indicator whether the engine was initialized
		bool engineInitialized = false;

	public:

		/**
		* Creates a new application wrapper
		*/
		ofxCogApp() : configFile(""), fps(DEFAULT_APP_SPEED){
		}

		/**
		* Creates a new application wrapper with configuration file
		*/
		ofxCogApp(string configFile) : configFile(configFile), fps(DEFAULT_APP_SPEED) {
			ofxXml* xml = new ofxXml();
			xml->loadFile(configFile);
			this->xmlConfig = spt<ofxXml>(xml);
		}

		/**
		* Gets actual frames per second
		*/
		uint64 GetFps() const {
			return fps;
		}

		/**
		* Virtual method that can be overridden to register custom components
		*/
		virtual void RegisterComponents() {

		}

		/**
		* Virtual method that can be overridden to initialize custom components
		*/
		virtual void InitComponents() {

		}

		/**
		* Virtual method that can be overridden to initialize engine with custom configuration
		*/
		virtual void InitEngine();

		/**
		* Virtual method that can be overridden to initialize stage component
		*/
		virtual void InitStage(Stage* stage);

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
		// handler for mouse wheel
		void mouseScrolled(int x, int y, float scrollX, float scrollY);
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
		// handler for texture reloading
		void reloadTextures();

		// default behavior for backpress button, can be overridden
		virtual bool OnBackPress() {
			return false;
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

	};

}// namespace