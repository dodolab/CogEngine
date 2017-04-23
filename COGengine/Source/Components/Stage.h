#pragma once

#include "Component.h"

namespace Cog {

	class Node;
	class Behavior;
	class Scene;
	class Tween;

	/**
	* Stage, the parent of all scenes and the complete node-tree at all,
	* takes care of scene switching and global listeners registration
	*/
	class Stage : public Component{

	private:

		// message listeners, mapped by the action they listens to
		map<StrId, vector<BaseComponent*>> msgListeners;

		// actual scene
		Scene* actualScene = nullptr;
		// list of all scenes
		vector<Scene*> scenes;
		// root object, holds reference to all scene nodes
		Node* rootObject = nullptr;
		// stack of switched scenes
		stack<Scene*> sceneStack;

	public:

		~Stage();

		void OnInit();

		void OnMessage(Msg& msg);

		/**
		* Gets the root object
		*/
		Node* GetRootObject() const {
			return rootObject;
		}

		/**
		* Gets actual scene
		*/
		Scene* GetActualScene() const {
			return actualScene;
		}

		/**
		* Gets stack of switched scenes
		*/
		stack<Scene*>& GetSceneStack() {
			return sceneStack;
		}

		/**
		* Adds a new scene
		* @param scene scene to add
		* @param setAsActual if true, scene will be set as the actual scene
		*/
		void AddScene(Scene* scene, bool setAsActual);

		/**
		* Copies all global listeners to selected scene
		* Copying global listeners to all scenes simplifies their handling because
		* otherwise the scene had to check both local and global listeners
		*/
		void CopyGlobalListenersToScene(Scene* scene);

		/**
		* Registers global listener for selected action
		*/
		void RegisterGlobalListener(StrId action, BaseComponent* listener);

		/**
		* Unregisters global listener for selected action
		*/
		bool UnregisterGlobalListener(StrId action, BaseComponent* listener);

		/**
		* Finds scene by its name
		*/
		Scene* FindSceneByName(string name) const;
	
		/**
		* Switches to selected scene
		* @param scene scene to switch
		* @param tweenDir tween direction; if none, scene will be switched automatically
		* @param finishOld if true, the old scene will be finished
		*/
		void SwitchToScene(Scene* scene, stenum tweenDir, bool finishOld = false);


		/**
		* Switches actual scene back to previous scene; very similar to 
		* back-press buttton in Android
		* @param finishOld if true, the old scene will be finished
		*/
		bool SwitchBackToScene(stenum tweenDir, bool finishOld = false);

		/**
		* Loads scenes from resources
		*/
		void LoadScenes();

		/**
		* Writes info about all scenes and their nodes into the console
		*/
		void WriteInfo(int logLevel = 0);

		virtual void Update(const uint64 delta, const uint64 absolute) {
		}
	};

}// namespace