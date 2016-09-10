#include "Scene.h"
#include "CogEngine.h"

namespace Cog {

	void Scene::SendMessage(Msg& msg, Node* actualNode) {
		// there is no such callback or behavior that listens to that type of message
		if (!IsRegisteredListener(msg.GetAction())) return;

		BubblingType& trav = msg.GetBubblingType();

		if (trav.scope == Scope::DIRECT_NO_TRAVERSE) {
			// no BubblingType - just iterate over the proper collection of behaviors and callbacks
			SendDirectMessage(msg);

		}
		else SendBubblingMessage(msg, actualNode);

		if (!msg.DataKept()) {
			msg.DeleteData();
		}
	}


	void Scene::SendDirectMessageToListener(Msg& msg, int targetId) {
		Behavior* beh = FindBehaviorById(targetId);

		if (beh != nullptr) {
			beh->OnMessage(msg);
		}

		if (!msg.DataKept()) {
			msg.DeleteData();
		}
	}

	void Scene::SendDirectMessageToNode(Msg& msg, int targetId) {
		Node* node = FindNodeById(targetId);

		if (node != nullptr) {
			SendBubblingMessage(msg, node);
		}

		if (!msg.DataKept()) {
			msg.DeleteData();
		}
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

	void Scene::LoadFromXml(spt<ofxXml> xml) {

		auto cache = GETCOMPONENT(ResourceCache);

		SetName(xml->getAttribute(":", "name", ""));

		sceneNode = new Node(ObjType::SCENE, 0, name);
		sceneNode->SetScene(this);
		sceneNode->SetShape(spt<Shape>(new Rectangle(CogGetScreenWidth(), CogGetScreenHeight())));

		if (xml->pushTagIfExists("transform")) {
			auto math = TransformMath();
			math.LoadTransformFromXml(xml, sceneNode, sceneNode, settings);
			xml->popTag();
		}

		if (xml->pushTagIfExists("scene_settings")) {
			
			auto map = cache->LoadSettingsFromXml(xml);
			settings.MergeSettings(map);

			xml->popTag();
		}

		if (xml->tagExists("sceneanim")) {
			// parse animation

		}

		int nodes = xml->getNumTags("node");
		NodeBuilder bld = NodeBuilder();

		for (int i = 0; i < nodes; i++) {
			xml->pushTag("node", i);
			// load nodes
			Node* node = bld.LoadNodeFromXml(xml, sceneNode, settings);
			sceneNode->AddChild(node);

			xml->popTag();
		}
	}

	void Scene::SendMessageToBehaviors(Msg& msg, Node* actualNode) {
		for (auto it = actualNode->GetBehaviors().begin(); it != actualNode->GetBehaviors().end(); ++it) {
			Behavior* beh = (*it);
			if ((beh->GetListenerState() == ListenerState::ACTIVE_MESSAGES || beh->GetListenerState() == ListenerState::ACTIVE_ALL) &&
				(beh->GetId() != msg.GetBehaviorId())) {
				if (IsRegisteredListener(msg.GetAction(), beh)) {
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

}// namespace