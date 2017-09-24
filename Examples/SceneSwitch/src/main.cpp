#include "ofxCogMain.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"
#include "ofxTextLabel.h"
#include "NetworkManager.h"
#include "Mesh.h"
#include "NetworkCommunicator.h"
#include "NetMessage.h"
#include "Interpolator.h"
#include "AttribAnimator.h"
#include "UpdateMessage.h"
#include "Stage.h"
#include "LuaScripting.h"

/*
 *
 * The code below is implemented in lua script
 *
class SceneSwitcher : public Behavior {
public:
	SceneSwitcher() {
		
	}

	void OnInit() {
		SubscribeForMessages(ACT_BUTTON_CLICKED);
	}

	void OnMessage(Msg& msg) {
		if (msg.HasAction(ACT_BUTTON_CLICKED)) {
			auto stage = GETCOMPONENT(Stage);
			string actualScene = stage->GetActualScene()->GetName();
			string newScene = actualScene.compare("scene1") == 0 ? "scene2" : "scene1";

			if (msg.GetContextNode()->GetTag().compare("previous_but") == 0) {
				stage->SwitchToScene(stage->FindSceneByName(newScene), TweenDirection::RIGHT);
			}
			if (msg.GetContextNode()->GetTag().compare("next_but") == 0) {
				stage->SwitchToScene(stage->FindSceneByName(newScene), TweenDirection::LEFT);
			}
		}
	}

	void Update(uint64 delta, uint64 absolute) {

	}
};*/

class ExampleApp : public ofxCogApp {

	void RegisterComponents() {
		REGISTER_COMPONENT(new LuaScripting());
		//REGISTER_BEHAVIOR(SceneSwitcher);
	}

	void InitEngine() {
		ofxCogEngine::GetInstance().SetFps(100);
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


