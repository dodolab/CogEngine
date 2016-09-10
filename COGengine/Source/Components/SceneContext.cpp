#include "SceneContext.h"
#include "Scene.h"
#include "CogEngine.h"
#include "AsyncProcess.h"

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

	Scene* SceneContext::FindSceneByName(string name) const {
		for (auto it = scenes.begin(); it != scenes.end(); ++it) {
			if ((*it)->GetName().compare(name) == 0) {
				return (*it);
			}
		}
		return nullptr;
	}

	void SceneContext::SwitchToScene(Scene* scene, TweenDirection tweenDir) {
		auto manager = GETCOMPONENT(SceneManager);

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
			auto manager = GETCOMPONENT(SceneManager);
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

			this->scenes.push_back(sc);

			if (sc->GetName().compare(initialScene) == 0) {
				// set as initial
				AddScene(sc, true);
			}
			else if (!loading.empty() && sc->GetName().compare(loading) == 0) {
				AddScene(sc, false);
				this->loadingScene = sc;
			}
			else {
				AddScene(sc, false);
			}

			xml->popTag();
		}
	}


}// namespace