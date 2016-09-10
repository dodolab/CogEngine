#pragma once

#include "ofxCogCommon.h"
#include "Settings.h"
#include "LayerEnt.h"

namespace Cog
{
	class Node;
	class MsgListener;
	class Behavior;
	class Msg;
	class StrId;

	enum class SceneType {
		SCENE = 0,
		DIALOG = 1
	};

	/**
	* Scene entity, containing node tree
	*/
	class Scene
	{
	private:
		// unique name
		string name;
		// settings
		Settings settings;
		// node on top
		Node* sceneNode = nullptr;
		// settings for all layers presented in this scene
		vector<LayerEnt> layers;
		// parent scene; only for partial scenes such as dialogs
		Scene* parentScene;
		// message listeners
		map<StrId, vector<MsgListener*>> msgListeners;
		// listeners ids and their registered actions
		map<int, vector<StrId>> msgListenerActions;
		// list of all nodes
		vector<Node*> allNodes;
		// list of all behaviors
		vector<Behavior*> allBehaviors;
		// type of the scene
		SceneType sceneType = SceneType::SCENE;

		// indicator, if lazy loading should be used
		bool lazyLoad = false;
		// indicator, if this scene has been loaded
		bool loaded = false;
		// indicator, if this scene has been initialized (and can be displayed)
		bool initialized = false;

		// viewport offset of the scene
		ofVec2f viewPortOffset = ofVec2f(0,0);

	public:

		~Scene();

		Scene(string name, bool isLazyLoaded);

		Node* GetSceneNode() {
			return sceneNode;
		}

		const string GetName() const {
			return name;
		}

		void SetName(string name) {
			this->name = name;
		}

		bool IsLazyLoad() {
			return lazyLoad;
		}

		void SetLazyLoad(bool lazyLoad) {
			this->lazyLoad = lazyLoad;
		}

		bool Loaded() {
			return loaded;
		}

		bool Initialized() {
			return initialized;
		}

		ofVec2f& GetViewPortOffset() {
			return viewPortOffset;
		}

		Settings& GetSettings() {
			return settings;
		}

		void SetSceneSettings(Settings& settings);

		void AddLayer(LayerEnt layer) {
			layers.push_back(layer);
		}

		vector<LayerEnt>& GetLayers() {
			return layers;
		}

		LayerEnt FindLayerSettings(string name);

		SceneType GetSceneType() {
			return sceneType;
		}

		void SetSceneType(SceneType sceneType) {
			this->sceneType = sceneType;
		}

		Scene* GetParentScene() {
			return parentScene;
		}

		void SetParentScene(Scene* parent) {
			this->parentScene = parent;
		}

		/**
		* Registers behavior listener for selected action
		* @param action action to register
		* @param listener listener that will be called when selected action is invoked
		*/
		void RegisterListener(StrId action, MsgListener* listener);

		/**
		* Unregisters message listener for selected action
		* @return true if listener has been found and erased
		*/
		bool UnregisterListener(StrId action, MsgListener* listener);

		/**
		* Unregisters all actions that are bound with selected listener
		* @param beh listener to unregister
		*/
		void UnregisterListener(MsgListener* beh);

		/**
		* Sends common message
		* @param msg message  to send
		* @param actualNode actual node in bubbling algorithm
		*/
		void SendMessage(Msg& msg, Node* actualNode);

		/**
		* Sends message to specific listener
		* @param msg message  to send
		* @param targetId id of target listener
		*/
		void SendDirectMessageToListener(Msg& msg, int targetId);

		/**
		* Sends message to specific node
		* @param msg message  to send
		* @param targetId id of target node
		*/
		void SendDirectMessageToNode(Msg& msg, int targetId);

		/**
		* Returns true, if there is at least one behavior listener for selected action
		*/
		bool IsRegisteredListener(StrId action) const;

		/**
		* Returns true, if listener is listening for selected action
		*/
		bool IsRegisteredListener(int action, MsgListener* beh);

		/**
		* Finds node by id
		*/
		Node* FindNodeById(int id) const;

		/**
		* Finds behavior by id
		*/
		Behavior* FindBehaviorById(int id) const;

		/**
		* Gets number of nodes with specific tag
		*/
		int GetNodesCountByTag(string tag) const;

		/**
		* Finds node by tag
		*/
		Node* FindNodeByTag(string tag) const;

		/**
		* Finds all nodes by tag
		*/
		vector<Node*> FindNodesByTag(char* tag) const;

		/**
		* Gets number of nodes with specific subtype
		*/
		int GetNodesCountBySubType(int subtype) const;

		/**
		* Finds node by subtype
		*/
		Node* FindNodeBySubType(int subtype) const;

		/**
		* Finds all nodes by subtype
		*/
		vector<Node*> FindNodesBySubType(int subtype) const;

		/**
		* Finds all nodes by group
		*/
		vector<Node*> FindNodesByGroup(StrId group) const;

		/**
		* Initializes the scene
		*/
		void Init();

		/**
		* Disposes the scene
		*/
		void Dispose();

		/**
		* Loads scene from xml
		*/
		void LoadFromXml(spt<ofxXml> xml);

		bool operator==(string name) {
			return this->name.compare(name) == 0;
		}

		bool operator==(const Scene& other) {
			return other.GetName().compare(name) == 0;
		}

		bool operator!=(int id) {
			return this->name.compare(name) != 0;
		}

		bool operator!=(const Scene& other) {
			return other.GetName().compare(name) != 0;
		}

		private:

			/**
			* Sends message to behaviors
			* @param msg message to send
			* @param actualNode actual node in bubbling algorithm
			*/
			void SendMessageToBehaviors(Msg& msg, Node* actualNode);

			/**MM
			* Sends message to children of actual node
			* @param msg message to send
			* @param actualNode actual node in bubbling algorithm
			*/
			void SendTunnelingMessageToChildren(Msg& msg, Node* actualNode);

			/**
			* Sends message that will be bubbled from actualNode
			* @param msg message to send
			* @param actualNode actual node in bubbling algorithm
			*/
			void SendTunnelingMessage(Msg& msg, Node* actualNode);

			/**
			* Sends direct message - takes all behaviors and callbacks that have
			* registered selected action
			* @param msg message  to send
			*/
			void SendDirectMessage(Msg& msg);

			/**
			* Adds a new node to the collection
			* @return true, if node has been added
			*/
			bool AddNode(Node* node);

			/**
			* Removes node from collection
			*/
			void RemoveNode(Node* node);

			/**
			* Adds a new behavior
			* @return true if behavior has been added
			*/
			bool AddBehavior(Behavior* beh);

			/**
			* Removes behavior from collection
			*/
			void RemoveBehavior(Behavior* beh);

			friend class Node;

		public:
			void WriteInfo(int logLevel = 0);
	};

} // namespace
