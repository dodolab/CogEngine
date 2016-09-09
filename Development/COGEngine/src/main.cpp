#include "ofMain.h"
#include "MGameEngine.h"
#include "CopterFactory.h"
//========================================================================

#ifdef TARGET_WINDOWS

#include "ofApp.h"

int main( ){
	ofSetupOpenGL(1280,720,OF_WINDOW);
	ofRunApp(new ofApp());
	return 0;
}


#endif


#ifdef TARGET_ANDROID
#include "ofAndroidApp.h"
#include <jni.h>

int main(){
	ofSetupOpenGL(1280,720, OF_WINDOW);			// <-------- setup the GL context
	ofRunApp( new ofAndroidApp() );
	return 0;
}


//========================================================================
extern "C"{
	void Java_cc_openframeworks_OFAndroid_init( JNIEnv*  env, jobject  thiz ){
		main();
	}
}
#endif

