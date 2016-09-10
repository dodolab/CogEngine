#include "Stage.h"
#include "Scene.h"
#include "Node.h"
#include "Behavior.h"
#include "CogEngine.h"
#include "AsyncProcess.h"
#include "Tween.h"
#include "InputKey.h"

namespace Cog {

	Stage::~Stage() {

		rootObject->InsertElementsForAdding(true);
		rootObject->DeleteElementsForRemoving(true);

		for (Scene* scene : scenes) {
			delete scene;
		}

		delete rootObject;
	}

	void Stage::Init() {
		// create root object with default behaviors, states and attributes
		this->rootObject = new Node(NodeType::ROOT, 0, "root");
		RegisterGlobalListener(ACT_SCENE_SWITCHED, this);
		CogRegisterGlobalListener(ACT_SCREEN_CHANGED, this);
		this->rootObject->AddBehavior(new InputKey(map<int,Act>()));

	}

	void Stage::OnMessage(Msg& msg) {
		if (msg.HasAction(ACT_SCENE_SWITCHED)) {
			Node* scene =  (Node*)msg.GetSourceObject();
			actualScene = scene->GetScene();
		}
		else if (msg.HasAction(ACT_SCREEN_CHANGED)) {
			// scale root node
			if (actualScene != nullptr) {
				auto environment = GETCOMPONENT(Environment);
				// set scale according to the new ratio
				Node* sceneNode = actualScene->GetSceneNode();
				auto changeEvent = msg.GetDataS<ValueChangeEvent<Vec2i>>();
				

				auto virtuals = CogGetVirtualScreenSize();
				auto scSize = CogGetScreenSize();

				float absoluteRatio = (environment->GetScreenAspectRatio() / environment->GetVirtualAspectRatio());

				float heightRatio = scSize.y / (float)virtuals.y;
				float widthRatio = scSize.x / (float)virtuals.x;
				
				this->GetRootObject()->GetTransform().scale = ofVec3f(virtuals.x/((float)environment->GetOriginalWidth()));
			}
		}
	}

	void Stage::AddScene(Scene* scene, bool setAsActual) {

		rootObject->AddChild(scene->GetSceneNode());

		if (setAsActual) {
			this->actualScene = scene;
			scene->Init();
			scene->GetSceneNode()->SetRunningMode(RunningMode::RUNNING);
		}
		else {
			scene->GetSceneNode()->SetRunningMode(RunningMode::DISABLED);
		}

		// copy global listeners
		for (auto it = msgListeners.begin(); it != msgListeners.end(); ++it) {
			StringHash action = (*it).first;
			vector <MsgListener*>& listeners = (*it).second;

			for (auto jt = listeners.begin(); jt != listeners.end(); ++jt) {
				scene->RegisterListener(action, (*jt));
			}
		}

		this->scenes.push_back(scene);

		COGLOGDEBUG("Stage", "Initializing scene %s", scene->GetName().c_str());
		scene->GetSceneNode()->SubmitChanges(true);
	}

	void Stage::RegisterGlobalListener(StringHash action, MsgListener* listener) {
		if (msgListeners.find(action) == msgListeners.end()) {
			msgListeners[action] = vector <MsgListener*>();
		}

		vector<MsgListener*>& listeners = msgListeners[action];
		listeners.push_back(listener);
		
		for (Scene* sc : this->scenes) {
			sc->RegisterListener(action, listener);
		}
	}

	bool Stage::UnregisterGlobalListener(StringHash action, MsgListener* listener) {
		if (msgListeners.find(action) != msgListeners.end()) {
			vector<MsgListener*>& listeners = msgListeners[action];

			for (auto it = listeners.begin(); it != listeners.end(); ++it) {
				if ((*it)->GetId() == listener->GetId()) {
					listeners.erase(it);

					for (Scene* sc : this->scenes) {
						sc->UnregisterListener(listener);
					}

					return true;
				}
			}
		}
		return false;
	}

	Scene* Stage::FindSceneByName(string name) const {
		for (auto it = scenes.begin(); it != scenes.end(); ++it) {
			if ((*it)->GetName().compare(name) == 0) {
				return (*it);
			}
		}
		return nullptr;
	}

	void Stage::SwitchToScene(Scene* scene, TweenDirection tweenDir) {

		if (actualScene == nullptr) {
			// there is no scene running yet -> switch immediately
			this->actualScene = scene;
			scene->Init();
			scene->GetSceneNode()->SetRunningMode(RunningMode::RUNNING);
		}
		else {
			// switch, using switch manager
			auto manager = GETCOMPONENT(SceneSwitchManager);
			
			// if actual scene is dialog, close the dialog first
			if (actualScene->GetSceneType() == SceneType::DIALOG) {
				this->SwitchBackToScene(TweenDirection::NONE);
			}

			Node* from = actualScene->GetSceneNode();
			

			if (scene->IsLazyLoad() && !scene->Loaded()) {
				// scene is lazy loaded -> run asynchronous loading process

				auto async = new AsyncProcess(new SceneLoader(CogEngine::GetInstance().config, scene, tweenDir));

				COGLOGDEBUG("Stage", "Scene is lazy loaded!");

				// lazy load the scene
				if (this->loadingScene != nullptr) {
					COGLOGDEBUG("Stage", "Loading progress scene instead");

					Node* to = loadingScene->GetSceneNode();
					sceneStack.push(actualScene);
					actualScene = loadingScene;

					// switch to loading window
					manager->SwitchToScene(from, to, tweenDir);
				}

				CogRunThread(async);
			}
			else {
				// switch scene using the switch manager
				if (actualScene != nullptr &&
					(loadingScene == nullptr ||
						(actualScene->GetName().compare(loadingScene->GetName()) != 0))) {
					sceneStack.push(actualScene);
				}

				actualScene = scene;

				Node* to = scene->GetSceneNode();
				manager->SwitchToScene(from, to, tweenDir);
			}
		}
	}

	bool Stage::SwitchBackToScene(TweenDirection tweenDir) {
		if (!sceneStack.empty()) {
			auto manager = GETCOMPONENT(SceneSwitchManager);
			Scene* scene = sceneStack.top();

			Node* from = actualScene->GetSceneNode();
			Node* to = scene->GetSceneNode();
			actualScene = scene;

			COGLOGDEBUG("Stage", "Switching to previous scene");

			manager->SwitchToScene(from, to, tweenDir);

			sceneStack.pop();

			return true;
		}
		return false;
	}


	void Stage::LoadScenesFromXml(spt<ofxXml> xml) {

		COGLOGDEBUG("Stage", "Loading scenes from XML");

		string initialScene = xml->getAttributex("initial", "");
		string loading = xml->getAttributex("loading", "");

		int scenesNum = xml->getNumTags("scene");

		for (int i = 0; i < scenesNum; i++) {
			xml->pushTag("scene", i);
			
			string name = xml->getAttributex("name", "");
			bool isLazy = xml->getBoolAttributex("lazy", false);
			
			Scene* sc = new Scene(name, isLazy);

			if (!sc->IsLazyLoad()) {
				// load complete scene only if it isn't lazy loaded
				sc->LoadFromXml(xml);
			}

			if (sc->GetName().compare(initialScene) == 0) {
				// set as initial
				AddScene(sc, true);
			}
			else {
				AddScene(sc, false);
			}
				
			if (!loading.empty() && sc->GetName().compare(loading) == 0) {
				// set as loading scene
				this->loadingScene = sc;
			}

			xml->popTag();
		}

		if (actualScene == nullptr && this->scenes.size() != 0) {
			// if no actual scene specified, use the first one
			auto firstScene = this->scenes[0];
			SwitchToScene(firstScene, TweenDirection::NONE);
		}
	}

	void Stage::WriteInfo(int logLevel) {
		
		if(actualScene != nullptr) CogLogTree("INFO_STAGE", logLevel, "Actual scene: ", actualScene->GetName().c_str());

		if (this->scenes.size() > 0) {
			CogLogTree("INFO_STAGE_SCENES", logLevel, "Scenes: %d", scenes.size());

			for (Scene* sc : scenes) {
				sc->WriteInfo(logLevel + 2);
			}
		}
	}


}// namespace