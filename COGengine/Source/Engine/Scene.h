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
	class StringHash;

	/**
	* Scene entity, containing node tree
	*/
	class Scene
	{
	private:
		string name;
		// settings
		Settings settings;
		// node on top
		Node* sceneNode = nullptr;
		// settings for all layers presented in this scene
		vector<LayerEnt> layers;

		// message listeners
		map<StringHash, vector<MsgListener*>> msgListeners;
		// listeners ids and their registered actions
		map<int, vector<StringHash>> msgListenerActions;
		// list of all nodes
		vector<Node*> allNodes;
		// list of all behaviors
		vector<Behavior*> allBehaviors;


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

		string GetName() {
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

		/**
		* Registers behavior listener for selected action
		* @param action action to register
		* @param listener listener that will be called when selected action is invoked
		*/
		void RegisterListener(StringHash action, MsgListener* listener);

		/**
		* Unregisters message listener for selected action
		* @return true if listener has been found and erased
		*/
		bool UnregisterListener(StringHash action, MsgListener* listener);

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
		bool IsRegisteredListener(StringHash action) const;

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
		vector<Node*> FindNodesByGroup(StringHash group) const;

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
			void SendBubblingMessageToChildren(Msg& msg, Node* actualNode);

			/**
			* Sends message that will be bubbled from actualNode
			* @param msg message to send
			* @param actualNode actual node in bubbling algorithm
			*/
			void SendBubblingMessage(Msg& msg, Node* actualNode);

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
