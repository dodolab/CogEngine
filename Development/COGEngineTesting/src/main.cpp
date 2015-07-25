#include "ofMain.h"
#include "ofApp.h"


#ifdef TARGET_WINDOWS
int main(){
	//ofAppGLFWWindow window;
	//ofSetupOpenGL(&window, 225,400,OF_WINDOW);
	ofSetupOpenGL(450, 800, OF_WINDOW);
	//window.setDoubleBuffering(true);
	//window.setNumSamples(4);

	//window.setGlutDisplayString("rgba double samples=4 depth");
	//window.setWindowTitle("COGEngine");
	ofRunApp(new MTestApp());
	return 0;
}

#else
#include <jni.h>

int main(){
	ofSetupOpenGL(720, 1280, OF_WINDOW);			// <-------- setup the GL context
	ofRunApp(new MTestApp());
	return 0;
}


//========================================================================
extern "C"{
	void Java_cc_openframeworks_OFAndroid_init(JNIEnv*  env, jobject  thiz){
		main();
	}
}
#endif

