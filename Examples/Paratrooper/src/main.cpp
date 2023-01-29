#include "ofxCogMain.h"
#include "jsonxx.h"
#include "pugixml.h"
#include "ParatrooperFactory.h"


class ParatrooperApp : public ofxCogApp {

	void RegisterComponents() {
		REGISTER_COMPONENT(new ParatrooperFactory());
	}


	void InitComponents() {	
		// set native size
		GETCOMPONENT(Environment)->SetNativeSize(800, 450);
	}

	void InitStage(Stage* stage) {
		stage->AddScene(new Scene("default", false), true);
		auto factory = GETCOMPONENT(ParatrooperFactory);
		auto model = factory->LoadGameModel();
		factory->InitializeGame(stage->GetActualScene()->GetSceneNode(), model);
	}
};

#ifndef WIN32
#include <jni.h>
#endif

int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ParatrooperApp());
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