#include "ofxCogMain.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"
#include "ofxTextLabel.h"
#include "NetworkManager.h"
#include "Mesh.h"
#include "NetMessage.h"
#include "Interpolator.h"
#include "AttribAnimator.h"
#include "UpdateMessage.h"
#include "LuaScripting.h"

class ExampleApp : public ofxCogApp {

	void RegisterComponents() {
		REGISTER_COMPONENT(new LuaScripting());
	}


	void InitEngine() {

		ofxCogEngine::GetInstance().Init();
		ofxCogEngine::GetInstance().LoadStage();
	}

	void InitStage(Stage* stage) {
	}
};

int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}


