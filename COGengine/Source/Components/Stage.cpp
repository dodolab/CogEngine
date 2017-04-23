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
			Node* scene = (Node*)msg.GetContextNode();
			auto info = msg.GetDataPtr<CommonEvent<SceneSwitchInfo>>();

			if (info->value.finishedOld) {
				// previous scene has been finished -> remove it from the stack
				if (!sceneStack.empty() 
					&& sceneStack.top()->GetSceneNode()->GetId() != scene->GetId()) { // check if it is a forward switch (not backward)
					sceneStack.pop();
				}
			}

			actualScene = scene->GetScene();
		}
		else if (msg.HasAction(ACT_SCREEN_CHANGED)) {
			// when screen size changes, the root node must be scaled
			if (actualScene != nullptr) {
				auto environment = GETCOMPONENT(Environment);
				// set scale according to the new ratio
				auto changeEvent = msg.GetDataPtr<ValueChangeEvent<Vec2i>>();

				auto virtuals = CogGetVirtualScreenSize();
				auto scSize = CogGetScreenSize();

				float absoluteRatio = (environment->GetScreenAspectRatio() / environment->GetVirtualAspectRatio());
				float heightRatio = scSize.y / (float)virtuals.y;
				float widthRatio = scSize.x / (float)virtuals.x;
				this->GetRootObject()->GetTransform().scale = ofVec3f(virtuals.x / ((float)environment->GetInitialWidth()));
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

	void Stage::SwitchToScene(Scene* scene, TweenDirection tweenDir, bool finishOld) {

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

			// switch scene using the switch manager
			if (actualScene != nullptr) {
				sceneStack.push(actualScene);
			}

			actualScene = scene;
			manager->SwitchToScene(from, actualScene, tweenDir, finishOld);

		}
	}

	bool Stage::SwitchBackToScene(TweenDirection tweenDir, bool finishOld) {
		if (!sceneStack.empty()) {

			auto manager = GETCOMPONENT(SceneSwitchManager);
			Scene* scene = sceneStack.top();

			Scene* from = actualScene;
			Scene* to = scene;

			actualScene = scene;

			COGLOGDEBUG("Stage", "Switching to previous scene");

			manager->SwitchToScene(from, to, tweenDir, finishOld);
			sceneStack.pop();

			return true;
		}
		return false;
	}


	void Stage::LoadScenes() {

		COGLOGDEBUG("Stage", "Loading scenes from XML");

		string initialScene = CogGetGlobalSettings().GetSettingVal("initial_scene");

		if (initialScene.empty()) {
			CogLogInfo("Stage", "Initial scene not specified Using the first one!");
		}

		string scenes = ofToDataPath(PATH_SCENES);

		if (!ofFile(scenes.c_str()).exists()) {
			CogLogError("Stage", "Scenes.xml not found!");
		}
		else {
			spt<xml_document> xml = CogPreloadXMLFile(scenes);

			bool initLoaded = false;

			auto resources = xml->child("resources");

			if (resources) {
				
				for (auto sceneNode : resources.children("scene")) {
					
					string name = sceneNode.attribute("name").as_string("");

					if (name.empty()) CogLogError("Stage", "Scene has no name!");

					bool isPreloaded = sceneNode.attribute("preload").as_bool(false);
					Scene* sc = new Scene(name, isPreloaded);

					bool isInitial = (initialScene.empty() || sc->GetName().compare(initialScene) == 0);


					if (sc->IsPreloaded() || (!initLoaded && isInitial)) {
						// load complete scene only if it isn't lazy loaded
						// the initial scene will always be preloaded
						sc->LoadFromXml(sceneNode);
					}
					else {
						// just set the indicator
						sc->SetLoadedFromXml(true);
					}

					if (!initLoaded && isInitial) {
						// set as initial
						AddScene(sc, true);
						initLoaded = true;
					}
					else {
						AddScene(sc, false);
					}
				}
			}
		}

		if (actualScene == nullptr && this->scenes.size() != 0) {
			// if no actual scene specified, use the first one
			auto firstScene = this->scenes[0];
			SwitchToScene(firstScene, TweenDirection::NONE);
		}
	}

	void Stage::WriteInfo(int logLevel) {

		if (actualScene != nullptr) CogLogTree("INFO_STAGE", logLevel, "Actual scene: ", actualScene->GetName().c_str());

		if (this->scenes.size() > 0) {
			CogLogTree("INFO_STAGE_SCENES", logLevel, "Scenes: %d", scenes.size());

			for (Scene* sc : scenes) {
				sc->WriteInfo(logLevel + 2);
			}
		}
	}


}// namespace