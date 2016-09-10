#include "ofxCogMain.h"
#include "AnimationLoader.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"
#include "MultiAnim.h"
#include "ofxNetwork.h"


class SwitchBehavior : public Behavior {
private:
	int actualConfig = 0;
	vector<string> configFiles;
public:

	SwitchBehavior(vector<string> configFiles) {
		this->configFiles = configFiles;
	}

	void Init() {
		
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
				ofxXml* xml = new ofxXml();
				xml->loadFile(newConfig.c_str());
				auto xmlPtr = spt<ofxXml>(xml);
				CogEngine::GetInstance().Init(xmlPtr);
				CogEngine::GetInstance().stage->GetRootObject()->AddBehavior(this);
			};

			CogEngine::GetInstance().AddPostUpdateAction(action);
		}
	}
};


class ExampleApp : public CogApp {


	void InitComponents() {

	}

	void InitEngine() {

		// find all config files
		ofDirectory dir = ofDirectory(".");
		auto files = dir.getFiles();
		auto configFiles = vector<string>();

		for (auto& file : files) {
			if (file.getExtension().compare("xml") == 0) {
				configFiles.push_back(file.getFileName());
			}
		}

		// load first config file
		ofxXml* xml = new ofxXml();
		xml->loadFile(configFiles[0]);
		auto xmlPtr = spt<ofxXml>(xml);
		CogEngine::GetInstance().Init(xmlPtr);
		CogEngine::GetInstance().stage->GetRootObject()->AddBehavior(new SwitchBehavior(configFiles));
		return;



		// this example is an alternative for config1.xml
		CogEngine::GetInstance().Init();

		auto resCache = GETCOMPONENT(ResourceCache);

		// <animations>
		auto anims = vector<spt<Anim>>();
		spt<Anim> anim = spt<Anim>(new Anim("square_anim", "sheet_squares.png", "", 4, 2, 0, 3, 1, 0.2, 0, false));
		resCache->StoreAnimation(anim);

		// <spritesheets>
		spt<SpriteSheet> spriteSheet = spt<SpriteSheet>(new SpriteSheet("squares", CogGet2DImage("sheet_squares.png"), 4, 128, 128));
		resCache->StoreSpriteSheet(spriteSheet);
		spriteSheet = spt<SpriteSheet>(new SpriteSheet("bgr", CogGet2DImage("bgr.jpg"), 1, 800, 450));
		resCache->StoreSpriteSheet(spriteSheet);

		// <transforms>
		spt<TransformEnt> trans = spt<TransformEnt>(new TransformEnt("rot_begin", ofVec2f(0), CalcType::PER, 0));
		spt<TransformEnt> trans2 = spt<TransformEnt>(new TransformEnt("rot_end", ofVec2f(0), CalcType::PER, 360));
		spt<TransformEnt> trans3 = spt<TransformEnt>(new TransformEnt("scale_begin", ofVec2f(0), 10, CalcType::PER, ofVec2f(0), ofVec2f(1), CalcType::LOC, 0));
		spt<TransformEnt> trans4 = spt<TransformEnt>(new TransformEnt("scale_end", ofVec2f(0), 10, CalcType::PER, ofVec2f(0), ofVec2f(-1), CalcType::LOC, 0));
		resCache->StoreEntity(trans);
		resCache->StoreEntity(trans2);
		resCache->StoreEntity(trans3);
		resCache->StoreEntity(trans4);

		// <behaviors>
		Setting setting = Setting();
		setting.AddItem("from", "rot_begin");
		setting.AddItem("to", "rot_end");
		setting.AddItem("duration", "4000");
		setting.AddItem("repeat", "false");
		setting.AddItem("blend", "additive");
		spt<BehaviorEnt> beh1 = spt<BehaviorEnt>(new BehaviorEnt("transform1", "TransformAnim", setting));
		Setting setting2 = Setting();
		setting2.AddItem("from", "scale_begin");
		setting2.AddItem("to", "scale_end");
		setting2.AddItem("duration", "4000");
		setting2.AddItem("repeat", "false");
		setting2.AddItem("blend", "additive");
		spt<BehaviorEnt> beh2 = spt<BehaviorEnt>(new BehaviorEnt("transform2", "TransformAnim", setting2));
		Setting setting3 = Setting();
		setting3.AddItem("animation", "square_anim");
		spt<BehaviorEnt> beh3 = spt<BehaviorEnt>(new BehaviorEnt("squareanim", "SheetAnim", setting3));
		resCache->StoreEntity(beh1);
		resCache->StoreEntity(beh2);
		resCache->StoreEntity(beh3);

		// <scenes>
		Scene* main = new Scene("main", false);

		//  <scene_layers>
		LayerEnt layer1 = LayerEnt("bgr", "bgr", 100, 30);
		LayerEnt layer2 = LayerEnt("squares", "squares", 100, 30);
		main->AddLayer(layer1);
		main->AddLayer(layer2);

		// <node>
		NodeBuilder bld = NodeBuilder();
		Node* node1 = new Node("bgr");
		bld.SetSpriteNode(main, node1, "bgr", 0, 0);
		main->GetSceneNode()->AddChild(node1);

		// <node>
		auto spriteSheet2 = resCache->GetSpriteSheet("squares");
		Node* node2 = new Node("square_2");
		bld.SetSpriteNode(main, node2, "squares", 0, 1);

		TransformEnt node2trans = TransformEnt();
		node2trans.pos = ofVec2f(0.15f);
		node2trans.pType = CalcType::PER;
		node2trans.sType = CalcType::LOC;
		node2trans.zIndex = 10;

		TransformMath math = TransformMath();
		math.SetTransform(node2, main->GetSceneNode(), node2trans);

		Setting settingAnim = Setting();
		settingAnim.AddItem("animations", "transform1|transform2");
		MultiAnim* multiAnim = new MultiAnim(settingAnim);

		node2->AddBehavior(multiAnim);
		node2->AddBehavior(bld.CreateBehavior(beh3));
		main->GetSceneNode()->AddChild(node2);

		// add scene into stage
		auto stage = GETCOMPONENT(Stage);
		stage->AddScene(main, true);

		// init logging
		auto logger = GETCOMPONENT(Logger);
		logger->SetLogLevel(LogLevel::LDEBUG);

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



