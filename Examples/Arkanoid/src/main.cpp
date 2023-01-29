#include "ofxCogMain.h"
#include "jsonxx.h"
#include "pugixml.h"
#include "ArkanoidFactory.h"


class ArkanoidApp : public ofxCogApp {

	void RegisterComponents() {
		REGISTER_COMPONENT(new ArkanoidFactory());
	}


	void InitComponents() {	
		// set native size
		GETCOMPONENT(Environment)->SetNativeSize(640, 400);
	}

	void InitStage(Stage* stage) {
		stage->AddScene(new Scene("default", false), true);
	}
};

#ifndef WIN32
#include <jni.h>
#endif

int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ArkanoidApp());
	return 0;
}

#ifndef WIN32

//========================================================================
extern "C" {
	void Java_cc_openframeworks_OFAndroid_init(JNIEnv*  env, jobject  thiz) {
		main();
	}
}
#endif