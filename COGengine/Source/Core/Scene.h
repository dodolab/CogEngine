#pragma once

#include "Settings.h"
#include "LayerEnt.h"
#include "Node.h"

namespace Cog
{
	class BaseComponent;
	class Behavior;
	class Msg;
	class StrId;

	/**
	* Type of the scene
	*/
	enum class SceneType {
		SCENE = 0,		/** common scene */
		DIALOG = 1		/** dialog scene, runs under another scene */
	};

	/**
	* Scene, contains link to the root node of the scene tree
	* All scene nodes are children of the main root, stored in the Stage class
	* Every sent message must go through the scene that looks for subscribers and calls their OnMessage method;
	* Global listeners are copied to the collection of msgListeners every time the scene is switched
	*
	* There can only oby one scene at a time, with the exception of dialog scenes that are always updated
	* together with parent scene
	*/
	class Scene
	{
	private:
		// scene unique name
		string name;
		// static settings, declared when the scene is built
		Settings settings;
		// bundle settings, declared when the scene is switched
		Setting bundle;
		// scene root node
		Node* sceneNode = nullptr;
		// layers presented in this scene
		vector<LayerEnt> layers;
		// parent scene; only for dialogs
		Scene* parentScene;
		// message listeners
		map<StrId, vector<BaseComponent*>> msgListeners;
		// listeners ids and their registered actions
		map<int, vector<StrId>> msgListenerActions;
		// list of all nodes in the tree
		vector<Node*> allNodes;
		// all nodes mapped by their hashed tag
		map<StrId, Node*> allNodes_tag;
		// all nodes mapped by their id
		map<int, Node*> allNodes_id;
		// list of all behaviors
		vector<Behavior*> allBehaviors;
		// type of the scene
		SceneType sceneType = SceneType::SCENE;

		// indicator whether the scene is preloaded
		bool preloaded = false;
		// indicator whether this scene has been loaded
		bool loaded = false;
		// indicator wheter this scene has been initialized (and therefore it can be displayed)
		bool initialized = false;
		// indicator whether this scene has been loaded from xml
		bool loadedFromXml = false;

		// viewport offset of the scene, used while switching
		ofVec2f viewPortOffset = ofVec2f(0, 0);

	public:

		Scene(string name, bool isLazyLoaded);

		~Scene();

		/**
		* Initializes the scene, is called whenever
		* a scene is switched
		*/
		void Init();

		/**
		* Disposes the scene
		*/
		void Dispose();

		/**
		* Clears all resources
		*/
		void Finish();

		/**
		* Reload this scene completely (usable only when loaded from xml)
		*/
		void Reload();

		/**
		* Gets root node of the scene
		*/
		Node* GetSceneNode() {
			return sceneNode;
		}

		/**
		* Gets unique name
		*/
		string GetName() const {
			return name;
		}

		/**
		* Sets scene name, must be unique
		* Note: uniqueness is not verified, its up to you
		*/
		void SetName(string name) {
			this->name = name;
		}

		/**
		* Gets indicator whether the scene is preloaded (only for scenes loaded from XML)
		* If so, everything is loaded when the application starts
		*/
		bool IsPreloaded() const {
			return preloaded;
		}

		/**
		* Sets indicator whether the scene is preloaded
		*/
		void SetPreloaded(bool preloaded) {
			this->preloaded = preloaded;
		}

		/**
		* Gets indicator whether the scene has been loaded
		*/
		bool Loaded() const {
			return loaded;
		}

		/**
		* Gets indicator whether the scene has been initialized
		*/
		bool Initialized() const {
			return initialized;
		}

		/**
		* Gets indicator whether this scene has been loaded from xml
		*/
		bool LoadedFromXml() const {
			return loadedFromXml;
		}

		/**
		* Sets indicator whether this scene has been loaded from xml
		*/
		void SetLoadedFromXml(bool loadedFromXml) {
			this->loadedFromXml = loadedFromXml;
		}

		/**
		* Gets viewport offset, used while switching
		*/
		ofVec2f& GetViewPortOffset() {
			return viewPortOffset;
		}

		/**
		* Gets settings of this scene
		*/
		Settings& GetSceneSettings() {
			return settings;
		}

		/**
		* Sets settings, usually from XML
		*/
		void SetSceneSettings(Settings& settings);

		/**
		* Gets bundle setting for this scene
		*/
		Setting& GetBundleSetting() {
			return bundle;
		}

		/**
		* Sets bundle setting for this scene
		*/
		void SetBundleSetting(Setting& setting) {
			this->bundle = setting;
		}

		/**
		* Gets layer, describing spritesheet used in the scene
		* Note: layers are used only for sprite textures
		*/
		void AddLayer(LayerEnt layer) {
			layers.push_back(layer);
		}

		/**
		* Gets collection of layers
		*/
		vector<LayerEnt>& GetLayers() {
			return layers;
		}

		/**
		* Finds layer settings by name of the layer
		*/
		LayerEnt FindLayerSettings(string name);

		/**
		* Gets scene type
		*/
		SceneType GetSceneType() const {
			return sceneType;
		}

		/**
		* Sets scene type
		*/
		void SetSceneType(SceneType sceneType) {
			this->sceneType = sceneType;
		}

		/**
		* Gets parent of this scene
		* This is for dialogs who only have parent scene
		*/
		Scene* GetParentScene() {
			return parentScene;
		}

		/**
		* Sets parent scene
		*/
		void SetParentScene(Scene* parent) {
			this->parentScene = parent;
		}

		/**
		* Subscribes listener for selected action
		* @param action action to subscribe
		* @param listener listener that will be called when selected action is invoked
		*/
		void RegisterListener(StrId action, BaseComponent* listener);

		/**
		* Unsubscribes listener for selected action
		* @return true if listener has been found and therefore removed
		*/
		bool UnregisterListener(StrId action, BaseComponent* listener);

		/**
		* Unregisters all actions that are subscribed by selected subscriber
		* @param listener listener to unregister
		*/
		void UnregisterListener(BaseComponent* listener);

		/**
		* Sends common message
		* @param msg message  to send
		*/
		void SendMessage(Msg& msg);

		/**
		* Returns true, if there is at least one subscriber for selected action
		*/
		bool IsRegisteredListener(StrId action) const;

		/**
		* Returns true, if listener is listening for selected action
		*/
		bool IsRegisteredListener(int action, BaseComponent* listener);

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
		void FindNodesByTag(char* tag, vector<Node*>& output) const;

		/**
		* Gets number of nodes with given secondary id
		*/
		int GetNodesCountBySecondaryId(int secondaryID) const;

		/**
		* Finds the first node with given secondary id
		*/
		Node* FindNodeBySecondaryId(int secondaryId) const;

		/**
		* Finds all nodes by secondary id
		*/
		void FindNodesBySecondaryId(int secondaryId, vector<Node*>& output) const;

		/**
		* Finds all nodes by group
		*/
		void FindNodesByGroup(StrId group, vector<Node*>& output) const;

		/**
		* Gets behavior of the scene node by its type
		* Note: use this method only if you know what you are doing;
		* in component oriented approach there shouldn't be any direct communication
		* between behaviors.
		*/
		template<class T> T* GetBehavior() {
			sceneNode->GetBehavior<T>();
		}

		/**
		* Loads scene from xml
		*/
		void LoadFromXml(xml_node& node);

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
		* Creates scene node with default parameters
		*/
		void CreateSceneNode();


		/**
		* Sends message to behaviors
		* @param msg message to send
		* @param actualNode actual node in traversal
		*/
		void SendMessageToBehaviors(Msg& msg, Node* actualNode);

		/**
		* Sends message to children of actual node
		* @param msg message to send
		* @param actualNode actual node in traversal
		*/
		void SendTunnelingMessageToChildren(Msg& msg, Node* actualNode);

		/**
		* Sends message that will go down from actualNode
		* @param msg message to send
		* @param actualNode actual node in traversal
		*/
		void SendTunnelingMessage(Msg& msg, Node* actualNode);

		/**
		* Sends direct message to all subscribers
		* @param msg message  to send
		*/
		void SendDirectMessage(Msg& msg);

		/**
		* Adds a new node to the collection
		* @return true, if node was added
		*/
		bool AddNode(Node* node);

		/**
		* Removes node from collection
		*/
		void RemoveNode(Node* node);

		/**
		* Adds a new behavior
		* @return true if behavior was added
		*/
		bool AddBehavior(Behavior* beh);

		/**
		* Removes behavior from collection
		*/
		void RemoveBehavior(Behavior* beh);

		friend class Node;

	public:
		/**
		* Writes info about hierarchy of the whole scene tree into console
		*/
		void WriteInfo(int logLevel = 0);
	};

} // namespace
