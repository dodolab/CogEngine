#include "Stage.h"
#include "Scene.h"
#include "Node.h"
#include "Behavior.h"
#include "ofxCogEngine.h"
#include "AsyncProcess.h"
#include "Tween.h"
#include "Environment.h"
#include "ComponentStorage.h"
#include "SceneSwitchManager.h"

namespace Cog {

	Stage::~Stage() {
		// insert all remaining elements so that they
		// will be erased from proper collection
		rootObject->InsertElementsForAdding(true, false);
		rootObject->DeleteElementsForRemoving(true);

		for (Scene* scene : scenes) {
			delete scene;
		}

		delete rootObject;
	}

	void Stage::OnInit() {
		// create root object
		this->rootObject = new Node(NodeType::ROOT, 0, "root");
		RegisterGlobalListener(ACT_SCENE_SWITCHED, this);
		CogRegisterGlobalListener(ACT_SCREEN_CHANGED, this);

	}

	void Stage::OnMessage(Msg& msg) {
		if (msg.HasAction(ACT_SCENE_SWITCHED)) {
			Node* scene =  (Node*)msg.GetContextNode();
			actualScene = scene->GetScene();
		}
		else if (msg.HasAction(ACT_SCREEN_CHANGED)) {
			// when screen size changes, the root node must be scaled
			if (actualScene != nullptr) {
				auto environment = GETCOMPONENT(Environment);
				// set scale according to the new ratio
				auto changeEvent = msg.GetData<ValueChangeEvent<Vec2i>>();
				
				auto virtuals = CogGetVirtualScreenSize();
				auto scSize = CogGetScreenSize();

				float absoluteRatio = (environment->GetScreenAspectRatio() / environment->GetVirtualAspectRatio());
				float heightRatio = scSize.y / (float)virtuals.y;
				float widthRatio = scSize.x / (float)virtuals.x;
				this->GetRootObject()->GetTransform().scale = ofVec3f(virtuals.x/((float)environment->GetInitialWidth()));
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
			// disable all scenes that are not visible in the beginning
			scene->GetSceneNode()->SetRunningMode(RunningMode::DISABLED);
		}

		CopyGlobalListenersToScene(scene);

		this->scenes.push_back(scene);

		COGLOGDEBUG("Stage", "Initializing scene %s", scene->GetName().c_str());
		scene->GetSceneNode()->SubmitChanges(true);
	}

	void Stage::CopyGlobalListenersToScene(Scene* scene) {
		for (auto it = msgListeners.begin(); it != msgListeners.end(); ++it) {
			StrId action = (*it).first;
			vector <BaseComponent*>& listeners = (*it).second;

			for (auto jt = listeners.begin(); jt != listeners.end(); ++jt) {
				scene->RegisterListener(action, (*jt));
			}
		}
	}

	void Stage::RegisterGlobalListener(StrId action, BaseComponent* listener) {
		msgListeners[action].push_back(listener);
		
		for (Scene* sc : this->scenes) {
			sc->RegisterListener(action, listener);
		}
	}

	bool Stage::UnregisterGlobalListener(StrId action, BaseComponent* listener) {
		if (msgListeners.count(action) != 0) {

			vector<BaseComponent*>& listeners = msgListeners[action];

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
			// if there is no running scene yet -> switch immediately
			this->actualScene = scene;
			scene->Init();
			scene->GetSceneNode()->SetRunningMode(RunningMode::RUNNING);
		}
		else {
			// use switch manager
			auto manager = GETCOMPONENT(SceneSwitchManager);
			
			// if actual scene is a dialog, close the dialog first
			if (actualScene->GetSceneType() == SceneType::DIALOG) {
				this->SwitchBackToScene(TweenDirection::NONE);
			}

			// set parent scene for dialog
			if (scene->GetSceneType() == SceneType::DIALOG) {
				scene->SetParentScene(actualScene);
			}

			Scene* from = actualScene;
			
			if (scene->IsLazyLoad() && !scene->Loaded()) 
			{
				// scene is lazy loaded -> run asynchronous loading process
				// note that this makes sense only if the scene is loaded via XML
				auto async = new AsyncProcess(new SceneLoader(ofxCogEngine::GetInstance().config, scene, tweenDir));

				COGLOGDEBUG("Stage", "Scene is lazy loaded!");

				// lazy load the scene
				if (this->loadingScene != nullptr) {
					// load loading scene first
					COGLOGDEBUG("Stage", "Switching to loading scene");

					Scene* to = loadingScene;
					sceneStack.push(actualScene);
					actualScene = loadingScene;

					// switch to loading window
					manager->SwitchToScene(from, to, tweenDir);
				}

				// run asynchronous process
				CogRunProcess(async);
			}
			else {
				// switch scene using the switch manager
				if (actualScene != nullptr && (loadingScene == nullptr ||
						(actualScene->GetName().compare(loadingScene->GetName()) != 0))) {
					sceneStack.push(actualScene);
				}

				actualScene = scene;
				manager->SwitchToScene(from, actualScene, tweenDir);
			}
		}
	}

	bool Stage::SwitchBackToScene(TweenDirection tweenDir) {
		if (!sceneStack.empty()) {

			auto manager = GETCOMPONENT(SceneSwitchManager);
			Scene* scene = sceneStack.top();

			Scene* from = actualScene;
			Scene* to = scene;
			
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

		// initial scene must be specified
		string initialScene = xml->getAttributex("initial", "");
		string loading = xml->getAttributex("loading", "");

		if (initialScene.empty()) CogLogError("Stage", "Initial scene is not specified!");

		int scenesNum = xml->getNumTags("scene");

		for (int i = 0; i < scenesNum; i++) {
			xml->pushTag("scene", i);
			
			string name = xml->getAttributex("name", "");
			
			if (name.empty()) CogLogError("Stage", "Scene has no name!");

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