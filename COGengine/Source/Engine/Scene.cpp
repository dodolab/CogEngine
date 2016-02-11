#include "Scene.h"
#include "CogEngine.h"
#include "NodeBuilder.h"

namespace Cog {

	Scene::Scene(string name, bool isLazyLoaded) :name(name), lazyLoad(isLazyLoaded) {
		sceneNode = new Node(ObjType::SCENE, 0, name);
		sceneNode->SetScene(this);
		sceneNode->SetShape(spt<Shape>(new Rectangle((float)CogGetScreenWidth(), (float)CogGetScreenHeight())));
	}

	void Scene::SetSceneSettings(Settings& settings) {

		this->settings = Settings();

		auto cache = GETCOMPONENT(ResourceCache);
		// always merge from default settings
		auto defaultSettings = cache->GetDefaultSettings();
		this->settings.MergeSettings(defaultSettings);
	
		this->settings.MergeSettings(settings);
	}

	LayerEnt Scene::FindLayerSettings(string name) {

		for (LayerEnt& entity : this->layers) {
			if (entity.name.compare(name) == 0) return entity;
		}

		return LayerEnt();
	}

	void Scene::RegisterListener(StringHash action, MsgListener* listener) {

		if (msgListeners.find(action) == msgListeners.end()) {
			msgListeners[action] = vector <MsgListener*>();
		}

		vector<MsgListener*>& listeners = msgListeners[action];
		listeners.push_back(listener);

		if (msgListenerActions.find(listener->GetId()) == msgListenerActions.end()) {
			msgListenerActions[listener->GetId()] = vector<StringHash>();
		}

		msgListenerActions[listener->GetId()].push_back(action);
	}

	bool Scene::UnregisterListener(StringHash action, MsgListener* listener) {
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

	void Scene::UnregisterListener(MsgListener* beh) {
		auto found = msgListenerActions.find(beh->GetId());

		if (found != msgListenerActions.end()) {

			vector<StringHash> actions = found->second;

			// unregister all actions
			for (auto action : actions) {
				UnregisterListener(action, beh);
			}

			// remove from the second collection
			msgListenerActions.erase(beh->GetId());
		}
	}

	void Scene::SendMessage(Msg& msg, Node* actualNode) {

		MLOGDEBUG("Messaging", "Message %s:%s", StringHash::GetStringValue(msg.GetAction()).c_str(), actualNode != nullptr ? actualNode->GetTag().c_str() : "");

		// there is no such callback or behavior that listens to that type of message
		if (!IsRegisteredListener(msg.GetAction())) return;

		BubblingType& trav = msg.GetBubblingType();

		if (trav.scope == Scope::DIRECT_NO_TRAVERSE) {
			// no BubblingType - just iterate over the proper collection of behaviors and callbacks
			SendDirectMessage(msg);

		}
		else if(actualNode != nullptr) SendBubblingMessage(msg, actualNode);

		if (!msg.DataKept()) {
			msg.DeleteData();
		}
	}


	void Scene::SendDirectMessageToListener(Msg& msg, int targetId) {

		MLOGDEBUG("Messaging", "Direct Message to listener %s; target: %d", StringHash::GetStringValue(msg.GetAction()).c_str(), targetId);

		Behavior* beh = FindBehaviorById(targetId);

		if (beh != nullptr) {
			beh->OnMessage(msg);
		}

		if (!msg.DataKept()) {
			msg.DeleteData();
		}
	}

	void Scene::SendDirectMessageToNode(Msg& msg, int targetId) {

		MLOGDEBUG("Messaging", "Direct Message to node %s; target: %d", StringHash::GetStringValue(msg.GetAction()).c_str(), targetId);

		Node* node = FindNodeById(targetId);

		if (node != nullptr) {
			SendBubblingMessage(msg, node);
		}

		if (!msg.DataKept()) {
			msg.DeleteData();
		}
	}

	bool Scene::IsRegisteredListener(StringHash action) const {
		return msgListeners.find(action) != msgListeners.end();
	}

	bool Scene::IsRegisteredListener(int action, MsgListener* beh) {
		if (msgListenerActions.find(beh->GetId()) == msgListenerActions.end()) return false;

		vector<StringHash>& actions = msgListenerActions[beh->GetId()];

		return (std::find(actions.begin(), actions.end(), action) != actions.end());
	}

	Node* Scene::FindNodeById(int id) const {
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetId() == id) return (*it);
		}
		return nullptr;
	}

	Behavior* Scene::FindBehaviorById(int id) const {
		for (auto it = allBehaviors.begin(); it != allBehaviors.end(); ++it) {
			if ((*it)->GetId() == id) return (*it);
		}
		return nullptr;
	}

	int Scene::GetNodesCountByTag(string tag) const {
		int counter = 0;
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetTag().compare(tag) == 0) counter++;
		}
		return counter;
	}

	Node* Scene::FindNodeByTag(string tag) const {
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetTag().compare(tag) == 0) return (*it);
		}
		return nullptr;
	}

	vector<Node*> Scene::FindNodesByTag(char* tag) const {
		vector<Node*> output;

		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetTag().compare(tag) == 0) output.push_back(*it);
		}
		return output;
	}

	int Scene::GetNodesCountBySubType(int subtype) const {
		int counter = 0;

		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetSubType() == subtype) counter++;
		}
		return counter;
	}

	Node* Scene::FindNodeBySubType(int subtype) const {
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetSubType() == subtype) return (*it);
		}
		return nullptr;
	}

	vector<Node*> Scene::FindNodesBySubType(int subtype) const {
		vector<Node*> output;

		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetSubType() == subtype) output.push_back(*it);
		}
		return output;
	}

	vector<Node*> Scene::FindNodesByGroup(StringHash group) const {
		vector<Node*> output;

		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			Node* nd = (*it);

			if (nd->IsInGroup(group)) {
				output.push_back(nd);
			}
		}

		return output;
	}

	
	void Scene::Init() {
		auto renderer = GETCOMPONENT(Renderer);
		auto cache = GETCOMPONENT(ResourceCache);

		// notify renderer that we will use specific layers
		for (auto layer : layers) {
			auto spriteSheet = cache->GetSpriteSheet(layer.spriteSheetName);
			renderer->AddTileLayer(spriteSheet->GetSpriteImage(), layer.name, layer.bufferSize, layer.zIndex);
		}
	}

	void Scene::Dispose() {
		auto renderer = GETCOMPONENT(Renderer);
		auto cache = GETCOMPONENT(ResourceCache);

		// remove layers from renderer
		for (auto layer : layers) {
			auto spriteSheet = cache->GetSpriteSheet(layer.spriteSheetName);
			renderer->RemoveTileLayer(layer.name);
		}
	}

	void Scene::LoadFromXml(spt<ofxXml> xml) {

		MLOGDEBUG("Scene", "Loading scene %s from xml", this->name.c_str());

		// load settings
		if (xml->pushTagIfExists("scene_settings")) {
			Settings set = Settings();
			set.LoadFromXml(xml);
			this->SetSceneSettings(set);

			xml->popTag();
		}
		
		// load layers
		if (xml->pushTagIfExists("scene_layers")) {
			int layersNum = xml->getNumTags("layer");

			for (int i = 0; i < layersNum; i++) {
				xml->pushTag("layer", i);
				LayerEnt layer = LayerEnt();
				layer.LoadFromXml(xml);
				layers.push_back(layer);
				xml->popTag();
			}

			xml->popTag();
		}

		int nodes = xml->getNumTags("node");
		NodeBuilder bld = NodeBuilder();

		// load nodes
		for (int i = 0; i < nodes; i++) {
			xml->pushTag("node", i);
			// load nodes
			Node* node = bld.LoadNodeFromXml(xml, sceneNode, this);
			sceneNode->AddChild(node);
			xml->popTag();
		}

		loaded = true;
	}

	void Scene::SendMessageToBehaviors(Msg& msg, Node* actualNode) {
		for (auto it = actualNode->GetBehaviors().begin(); it != actualNode->GetBehaviors().end(); ++it) {
			Behavior* beh = (*it);
			if ((beh->GetListenerState() == ListenerState::ACTIVE_MESSAGES || beh->GetListenerState() == ListenerState::ACTIVE_ALL) &&
				(beh->GetId() != msg.GetBehaviorId())) {
				if (IsRegisteredListener(msg.GetAction(), beh)) {
					MLOGDEBUG("Messaging", "Sending msg  %s; to behavior %s with id %d", StringHash::GetStringValue(msg.GetAction()).c_str(), beh->GetClassName().c_str(), beh->GetId());
					beh->OnMessage(msg);
				}
			}
		}
	}

	void Scene::SendBubblingMessageToChildren(Msg& msg, Node* actualNode) {
		for (auto it = actualNode->GetChildren().begin(); it != actualNode->GetChildren().end(); ++it) {
			CogSendMessage(msg, (*it));
		}
	}

	void Scene::SendBubblingMessage(Msg& msg, Node* actualNode) {

		BubblingType& trav = msg.GetBubblingType();

		if (trav.scope == Scope::ROOT) {
			trav.scope = Scope::OBJECT;
			// find root and call recursion
			Node* root = actualNode->GetRoot();
			if (root != nullptr) {
				// call this method again from the root
				if (trav.deep && !trav.bubbleDown) SendBubblingMessageToChildren(msg, root);
				SendMessageToBehaviors(msg, root);
				if (trav.deep && trav.bubbleDown) SendBubblingMessageToChildren(msg, root);
			}
			return;
		}
		else if (trav.scope == Scope::SCENE) {
			trav.scope = Scope::OBJECT;
			// find scene and call recursion
			Node* scRoot = actualNode->GetSceneRoot();
			if (scRoot != nullptr) {
				if (trav.deep && !trav.bubbleDown) SendBubblingMessageToChildren(msg, scRoot);
				SendMessageToBehaviors(msg, scRoot);
				if (trav.deep && trav.bubbleDown) SendBubblingMessageToChildren(msg, scRoot);
			}
			return;
		}

		if (trav.scope == Scope::OBJECT) {
			trav.scope = Scope::OBJECT;
			// call children and itself
			if (trav.deep && !trav.bubbleDown) SendBubblingMessageToChildren(msg, actualNode);
			SendMessageToBehaviors(msg, actualNode);
			if (trav.deep && trav.bubbleDown) SendBubblingMessageToChildren(msg, actualNode);
		}
		else if (trav.scope == Scope::CHILDREN) {
			trav.scope = Scope::OBJECT;
			// call children only
			SendBubblingMessageToChildren(msg, actualNode);
		}
	}

	void Scene::SendDirectMessage(Msg& msg) {
		auto listeners = msgListeners.find(msg.GetAction());

		if (listeners != msgListeners.end()) {
			vector<MsgListener*>& lsts = listeners->second;

			for (auto it = lsts.begin(); it != lsts.end(); ++it) {
				if (((*it)->GetListenerState() == ListenerState::ACTIVE_MESSAGES || (*it)->GetListenerState() == ListenerState::ACTIVE_ALL)) {
					(*it)->OnMessage(msg);
				}
			}
		}
	}

	bool Scene::AddNode(Node* node) {
		MLOGDEBUG("Scene", "Adding node %s to scene %s", node->GetTag().c_str(), this->name.c_str());
		auto found = find(allNodes.begin(), allNodes.end(), node);
		if (found == allNodes.end()) {
			allNodes.push_back(node);
			node->SetScene(this);
			return true;
		}
		else return false;
	}

	void Scene::RemoveNode(Node* node) {
		MLOGDEBUG("Scene", "Removing node %s from scene %s", node->GetTag().c_str(), this->name.c_str());
		auto found = find(allNodes.begin(), allNodes.end(), node);
		if (found != allNodes.end()) allNodes.erase(found);
	}

	bool Scene::AddBehavior(Behavior* beh) {
		COGASSERT(beh->GetOwner() != nullptr, "Scene", "Behavior %s hasn't node assigned", beh->GetClassName().c_str());
		MLOGDEBUG("Scene", "Adding behavior %s to node %s", beh->GetClassName().c_str(), beh->GetOwner()->GetTag().c_str());
		auto found = find(allBehaviors.begin(), allBehaviors.end(), beh);
		if (found == allBehaviors.end()) {
			allBehaviors.push_back(beh);

			return true;
		}
		else return false;
	}

	void Scene::RemoveBehavior(Behavior* beh) {
		COGASSERT(beh->GetOwner() != nullptr, "Scene", "Behavior %s hasn't node assigned", beh->GetClassName().c_str());
		MLOGDEBUG("Scene", "Removing behavior %s from node %s", beh->GetClassName().c_str(), beh->GetOwner()->GetTag().c_str());

		auto found = find(allBehaviors.begin(), allBehaviors.end(), beh);
		if (found != allBehaviors.end()) allBehaviors.erase(found);

		UnregisterListener(beh);
	}

	void Scene::WriteInfo(int logLevel) {
		
		CogLogTree("INFO_SCENE", logLevel, "Scene %s info:", this->name.c_str());

#if DEBUG

		if(msgListeners.size() > 0) CogLogTree("INFO_SCENE", logLevel+1, "Message listeners: %d",msgListeners.size());

		for (auto it = msgListeners.begin(); it != msgListeners.end(); ++it) {
			StringHash key = (*it).first;
			int listeners = (*it).second.size();

			if (listeners > 0) {
				string str = StringHash::GetStringValue(key)+":"+ofToString(listeners);
				CogLogTree("INFO_SCENE", logLevel+2, str.c_str());
			}
		}
#endif

		CogLogTree("INFO_SCENE", logLevel+1, "Nodes: %d",allNodes.size());
		CogLogTree("INFO_SCENE", logLevel+1, "Behaviors: %d", allBehaviors.size());

		CogLogTree("INFO_SCENE_NODES", logLevel+1, "Nodes::");

		this->sceneNode->WriteInfo(logLevel + 2);
	}


}// namespace