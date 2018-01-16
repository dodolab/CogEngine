#include "ofxCogMain.h"
#include "AnimLoader.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"
#include "MultiAnim.h"
#include "ofxNetwork.h"
#include "SpriteSheet.h"
#include "AttrAnimEnt.h"

/**
* Simple behavior that reloads configuration file when user presses PAGE UP/DOWN button
*/
class SwitchBehavior : public Behavior {
private:

	int actualSceneIndex = 0;
	int sceneNum = 0;
public:

	void OnInit() {
		sceneNum = GETCOMPONENT(Stage)->GetScenes().size();
	}

	virtual void Update(const uint64 delta, const uint64 absolute) {
		for (auto key : CogGetPressedKeys()) {
			if (!key->IsProcessed()) {
				string newConfig = "";

				if (key->key == (int)(OF_KEY_END)) {
					// restarts actual scene
					GETCOMPONENT(Stage)->GetActualScene()->Reload();
				}
				else if (key->key == OF_KEY_PAGE_UP) {
					// goes to previous scene
					if(actualSceneIndex == 0) {
						actualSceneIndex = sceneNum - 1;
					} else {
						actualSceneIndex = (actualSceneIndex - 1) % sceneNum;
					}
					auto scene = GETCOMPONENT(Stage)->GetScenes()[actualSceneIndex];
					GETCOMPONENT(Stage)->SwitchToScene(scene, TWEEN_RIGHT, false);
				}
				else if (key->key == OF_KEY_PAGE_DOWN) {
					// go to next scene
					actualSceneIndex = (actualSceneIndex + 1) % sceneNum;
					auto scene = GETCOMPONENT(Stage)->GetScenes()[actualSceneIndex];
					GETCOMPONENT(Stage)->SwitchToScene(scene, TWEEN_LEFT, false);
				}

				key->SetIsProcessed(true);
			}
		}
	}
};

class ExampleApp : public ofxCogApp {
public:


	void InitComponents() {

	}

	void InitEngine() {

		// load first config file
		ofxCogEngine::GetInstance().Init();
		ofxCogEngine::GetInstance().LoadStage();
		ofxCogEngine::GetInstance().stage->GetRootObject()->AddBehavior(new SwitchBehavior());
		
	}

	void InitStage(Stage* stage) {
	}
};



int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}


