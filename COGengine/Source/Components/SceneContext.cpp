#include "SceneContext.h"
#include "Node.h"
#include "Behavior.h"
#include "Scene.h"
#include "CogEngine.h"
#include "AsyncProcess.h"
#include "Tween.h"


namespace Cog {

	void SceneContext::Init() {
		// create root object with default behaviors, states and attributes
		this->rootObject = new Node(ObjType::ROOT, 0, "root");
		RegisterGlobalListening(ACT_SCENE_SWITCHED);
	}

	void SceneContext::OnMessage(Msg& msg) {
		if (msg.GetAction() == ACT_SCENE_SWITCHED) {
			Node* scene =  (Node*)msg.GetSourceObject();
			actualScene = scene->GetScene();
		}
	}

	void SceneContext::AddScene(Scene* scene, bool setAsActual) {

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
		MLOGDEBUG("SceneContext", "Initializing scene %s", scene->GetName().c_str());
		scene->GetSceneNode()->SubmitChanges(true);
	}

	void SceneContext::RegisterListener(StringHash action, MsgListener* listener) {
		if (msgListeners.find(action) == msgListeners.end()) {
			msgListeners[action] = vector <MsgListener*>();
		}

		vector<MsgListener*>& listeners = msgListeners[action];
		listeners.push_back(listener);
	}

	bool SceneContext::UnregisterListener(StringHash action, MsgListener* listener) {
		if (msgListeners.find(action) != msgListeners.end()) {
			vector<MsgListener*>& listeners = msgListeners[action];

			for (auto it = listeners.begin(); it != listeners.end(); ++it) {
				if ((*it)->GetId() == listener->GetId()) {
					listeners.erase(it);
					return true;
				}
			}
		}
		return false;
	}

	Scene* SceneContext::FindSceneByName(string name) const {
		for (auto it = scenes.begin(); it != scenes.end(); ++it) {
			if ((*it)->GetName().compare(name) == 0) {
				return (*it);
			}
		}
		return nullptr;
	}

	void SceneContext::SwitchToScene(Scene* scene, TweenDirection tweenDir) {
		auto manager = GETCOMPONENT(SceneSwitchManager);

		Node* from = actualScene->GetSceneNode();

		if (scene->IsLazyLoad() && !scene->Loaded()) {
			auto async = new AsyncProcess(new SceneLoader(COGEngine.config, scene, tweenDir));

			MLOGDEBUG("SceneContext", "Scene is lazy loaded!");

			// lazy load the scene
			if (this->loadingScene != nullptr) {
				MLOGDEBUG("SceneContext", "Loading progress scene instead");

				Node* to = loadingScene->GetSceneNode();
				sceneStack.push(actualScene);
				actualScene = loadingScene;
				
				// switch to loading window
				manager->SwitchToScene(from, to, tweenDir);
				
				// set tween to the map window, but don't switch automatically
				manager->PushSceneSwitch(to, scene->GetSceneNode(), tweenDir, false);
			}

			CogRunThread(async);
			return;
		}

		SetActualScene(scene);

		Node* to = scene->GetSceneNode();
		manager->SwitchToScene(from, to, tweenDir);
	}

	bool SceneContext::SwitchBackToScene(TweenDirection tweenDir) {
		if (!sceneStack.empty()) {
			auto manager = GETCOMPONENT(SceneSwitchManager);
			Scene* scene = sceneStack.top();

			Node* from = actualScene->GetSceneNode();
			Node* to = scene->GetSceneNode();
			actualScene = scene;

			MLOGDEBUG("SceneContext", "Switching to previous scene");

			manager->SwitchToScene(from, to, tweenDir);

			sceneStack.pop();

			return true;
		}
		return false;
	}

	void SceneContext::SetActualScene(Scene* scene) {
		if (loadingScene == nullptr || (actualScene->GetName().compare(loadingScene->GetName()) != 0)) {
				sceneStack.push(actualScene);
		}

		actualScene = scene;
	}

	void SceneContext::LoadScenesFromXml(spt<ofxXml> xml) {

		MLOGDEBUG("SceneContext", "Loading scenes from XML");

		string initialScene = xml->getAttributex("initial", "");
		string loading = xml->getAttributex("loading", "");

		int scenesNum = xml->getNumTags("scene");

		for (int i = 0; i < scenesNum; i++) {
			xml->pushTag("scene", i);
			
			Scene* sc = new Scene();

			sc->LoadInitDataFromXml(xml, i);

			if (!sc->IsLazyLoad()) {
				// load complete scene only if it isn't lazy loaded
				sc->LoadFromXml(xml);
			}

			
			if (sc->GetName().compare(initialScene) == 0) {
				// set as initial
				AddScene(sc, true);
			}
			else if (!loading.empty() && sc->GetName().compare(loading) == 0) {
				// set as loading scene
				AddScene(sc, false);
				this->loadingScene = sc;
			}
			else {
				AddScene(sc, false);
			}

			xml->popTag();
		}

		if (actualScene == nullptr) {
			// if no actual scene specified, use the first one
			auto firstScene = this->scenes[0];

			this->actualScene = firstScene;
			firstScene->Init();
			firstScene->GetSceneNode()->SetRunningMode(RunningMode::RUNNING);
		}
	}


}// namespace