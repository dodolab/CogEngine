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
	// index of actual configuration file
	int actualConfig = 0;
	// path to config files
	vector<string> configFiles;
public:

	SwitchBehavior(vector<string> configFiles, int fileIndex) : actualConfig(fileIndex) {
		this->configFiles = configFiles;
	}

	void OnInit() {

	}

	vector<string> GetConfigFiles() {
		return configFiles;
	}

	int GetActualConfigIndex() {
		return actualConfig;
	}

	virtual void Update(const uint64 delta, const uint64 absolute) {
		for (auto key : CogGetPressedKeys()) {
			if (!key->IsHandled()) {
				string newConfig = "";

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

				if (!newConfig.empty()) {
					// insert action that resets the engine
					auto action = [newConfig, this]() {
						CogLogInfo("Main", "Loading config %s", newConfig.c_str());
						// this will be deallocated, we need to keep path to files:
						vector<string> configFiles = this->GetConfigFiles();
						int actualConfig = this->GetActualConfigIndex();

						ofxCogEngine::GetInstance().Init(newConfig);
						ofxCogEngine::GetInstance().LoadStageFromXml(spt<ofxXml>(new ofxXml(newConfig)));
						ofxCogEngine::GetInstance().stage->GetRootObject()->AddBehavior(new SwitchBehavior(configFiles, actualConfig));
					};

					ofxCogEngine::GetInstance().AddPostUpdateAction(action);
				}
			}
		}
	}
};


class ExampleApp : public ofxCogApp {
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
		ofxCogEngine::GetInstance().Init(configFiles[0]);
		ofxCogEngine::GetInstance().LoadStageFromXml(spt<ofxXml>(new ofxXml(configFiles[0])));
		ofxCogEngine::GetInstance().stage->GetRootObject()->AddBehavior(new SwitchBehavior(configFiles, 0));

	}

	void InitStage(Stage* stage) {
	}
};



int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}


