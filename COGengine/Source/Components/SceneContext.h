#pragma once

#include "Node.h"
#include "Behavior.h"
#include "Scene.h"
#include "Component.h"
#include "Tween.h"

namespace Cog {

	/**
	* Node context
	*/
	class SceneContext : public Component{
	
		OBJECT(SceneContext)
	
	private:
		// message listeners
		map<StringHash, vector<MsgListener*>> msgListeners;

		// actual scene
		Scene* actualScene = nullptr;

		Scene* loadingScene = nullptr;
		// list of all scenes
		vector<Scene*> scenes = vector<Scene*>();

		// root object, should be created only once
		Node* rootObject = nullptr;

		stack<Scene*> sceneStack = stack<Scene*>();

	public:

		void Init();

		void OnMessage(Msg& msg);

		/**
		* Gets the root object
		*/
		Node* GetRootObject() {
			return rootObject;
		}

		/**
		* Gets actual scene
		*/
		Scene* GetActualScene() {
			return actualScene;
		}

		/**
		* Adds a new scene
		* @param setAsActual if true, scene will be set as the actual scene
		*/
		void AddScene(Scene* scene, bool setAsActual) {

			rootObject->AddChild(scene->GetSceneNode());

			if (setAsActual) {
				this->actualScene = scene;
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

			MLOGDEBUG("SceneContext", "Initializing scene %s", scene->GetName().c_str());
			scene->GetSceneNode()->SubmitChanges(true);
		}

		/**
		* Registers behavior listener for selected action
		* @param action action to register
		* @param listener listener that will be called when selected action is invoked
		*/
		void RegisterListener(StringHash action, MsgListener* listener) {
			if (msgListeners.find(action) == msgListeners.end()) {
				msgListeners[action] = vector <MsgListener*>();
			}

			vector<MsgListener*>& listeners = msgListeners[action];
			listeners.push_back(listener);
		}

		/**
		* Unregisters message listener for selected action
		* @return true if listener has been found and erased
		*/
		bool UnregisterListener(StringHash action, MsgListener* listener) {
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

		/**
		* Finds scene by its name
		*/
		Scene* FindSceneByName(string name) const;
	
		void SwitchToScene(Scene* scene, TweenDirection tweenDir);

		bool SwitchBackToScene(TweenDirection tweenDir);

		/**
		* Loads scene from xml
		*/
		void LoadScenesFromXml(spt<ofxXml> xml);

	};

}// namespace