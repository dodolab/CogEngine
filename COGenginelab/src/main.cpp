#include "ofxCogMain.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"
#include "MultiAnim.h"
#include "ofxNetwork.h"

/**
* Simple behavior that reloads configuration file when user presses PAGE UP/DOWN button
*/
class SwitchBehavior : public Behavior {
private:
	// index of actual configuration file
	int actualConfig = 0;
	// path to config files
	vector<string> configFiles;
public:

	SwitchBehavior(vector<string> configFiles, int fileIndex) : actualConfig(fileIndex) {
		this->configFiles = configFiles;
	}

	void Init() {

	}

	vector<string> GetConfigFiles() {
		return configFiles;
	}

	int GetActualConfigIndex() {
		return actualConfig;
	}

	virtual void Update(const uint64 delta, const uint64 absolute) {
		string newConfig = "";

		// for touch
		for (InputAct* touch : CogGetPressedPoints()) {
			if (!touch->IsHandled()) {
				touch->handlerNodeId = 1;
				actualConfig = (actualConfig + 1) % configFiles.size();
				newConfig = configFiles[actualConfig];
			}
		}

		for (auto key : CogGetPressedKeys()) {
			if (!key->IsHandled()) {
				

				if (key->key == (int)(OF_KEY_END)) {
					// restarts actual config file
					newConfig = configFiles[actualConfig];
				}
				else if (key->key == OF_KEY_PAGE_UP) {
					// goes to previous config file
					actualConfig = (actualConfig == 0 ? (configFiles.size() - 1) : (actualConfig - 1)) % configFiles.size();
					newConfig = configFiles[actualConfig];
				}
				else if (key->key == OF_KEY_PAGE_DOWN) {
					// goes to next config file
					actualConfig = (actualConfig + 1) % configFiles.size();
					newConfig = configFiles[actualConfig];
				}
			}
		}

		if (!newConfig.empty()) {
			// insert action that resets the engine
			auto action = [newConfig, this]() {
				CogLogInfo("Main", "Loading config %s", newConfig.c_str());
				// this will be deallocated, we need to keep path to files:
				vector<string> configFiles = this->GetConfigFiles();
				int actualConfig = this->GetActualConfigIndex();

				CogEngine::GetInstance().Init(newConfig);
				CogEngine::GetInstance().LoadStageFromXml(spt<ofxXml>(new ofxXml(newConfig)));
				CogEngine::GetInstance().stage->GetRootObject()->AddBehavior(new SwitchBehavior(configFiles, actualConfig));
			};

			CogEngine::GetInstance().AddPostUpdateAction(action);
		}
	}
};

class ExampleApp : public CogApp {
public:

	void InitComponents() {

	}

	void InitEngine() {
		// find all configuration files
		ofDirectory dir = ofDirectory(".");
		auto files = dir.getFiles();
		auto configFiles = vector<string>();

		for (auto& file : files) {
			if (ofToLower(file.getExtension()).compare("xml") == 0) {
				configFiles.push_back(file.getFileName());
			}
		}

		if (configFiles.size() == 0) {
			throw ConfigErrorException("No configuration file found!");
		}

		// load first config file
		CogEngine::GetInstance().Init(configFiles[0]);
		CogEngine::GetInstance().LoadStageFromXml(spt<ofxXml>(new ofxXml(configFiles[0])));
		CogEngine::GetInstance().stage->GetRootObject()->AddBehavior(new SwitchBehavior(configFiles, 0));
		return;



	}

	void InitStage(Stage* stage) {
	}
};


#ifdef WIN32

int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}

#else
#include <jni.h>

int main() {

	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}


//========================================================================
extern "C" {
	void Java_cc_openframeworks_OFAndroid_init(JNIEnv*  env, jobject  thiz) {
		main();
	}
}
#endif



