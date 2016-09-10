#include "SceneContext.h"
#include "Scene.h"
#include "CogEngine.h"

namespace Cog {

	void SceneContext::Init() {
		// create root object with default behaviors, states and attributes
		this->rootObject = new Node(ObjType::ROOT, 0, "root");
	}

	void SceneContext::SendMessage(Msg& msg, Node* actualNode) {
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


	void SceneContext::SendDirectMessageToListener(Msg& msg, int targetId) {
		Behavior* beh = FindBehaviorById(targetId);

		if (beh != nullptr) {
			beh->OnMessage(msg);
		}

		if (!msg.DataKept()) {
			msg.DeleteData();
		}
	}

	void SceneContext::SendDirectMessageToNode(Msg& msg, int targetId) {
		Node* node = FindNodeById(targetId);

		if (node != nullptr) {
			SendBubblingMessage(msg, node);
		}

		if (!msg.DataKept()) {
			msg.DeleteData();
		}
	}


	Node* SceneContext::FindNodeById(int id) const {
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetId() == id) return (*it);
		}
		return nullptr;
	}

	Behavior* SceneContext::FindBehaviorById(int id) const {
		for (auto it = allBehaviors.begin(); it != allBehaviors.end(); ++it) {
			if ((*it)->GetId() == id) return (*it);
		}
		return nullptr;
	}

	Scene* SceneContext::FindSceneByName(string name) const {
		for (auto it = scenes.begin(); it != scenes.end(); ++it) {
			if ((*it)->GetName().compare(name) == 0) {
				return (*it);
			}
		}
		return nullptr;
	}

	int SceneContext::GetNodesCountByTag(string tag) const {
		int counter = 0;
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetTag().compare(tag) == 0) counter++;
		}
		return counter;
	}

	Node* SceneContext::FindNodeByTag(string tag) const {
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetTag().compare(tag) == 0) return (*it);
		}
		return nullptr;
	}

	vector<Node*> SceneContext::FindNodesByTag(char* tag) const {
		vector<Node*> output;

		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetTag().compare(tag) == 0) output.push_back(*it);
		}
		return output;
	}

	int SceneContext::GetNodesCountBySubType(int subtype) const {
		int counter = 0;

		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetSubType() == subtype) counter++;
		}
		return counter;
	}

	Node* SceneContext::FindNodeBySubType(int subtype) const {
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetSubType() == subtype) return (*it);
		}
		return nullptr;
	}

	vector<Node*> SceneContext::FindNodesBySubType(int subtype) const {
		vector<Node*> output;

		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetSubType() == subtype) output.push_back(*it);
		}
		return output;
	}

	void SceneContext::SwitchToScene(Scene* scene, TweenDirection tweenDir) {
		auto manager = GETCOMPONENT(SceneManager);

		Node* from = actualScene->GetSceneNode();
		Node* to = scene->GetSceneNode();
		actualScene = scene;
		manager->SwitchToScene(from, to, tweenDir);
	}

	void SceneContext::LoadScenesFromXml(spt<ofxXml> xml) {

		string initialScene = xml->getAttribute(":", "initial", "");

		int scenesNum = xml->getNumTags("scene");

		for (int i = 0; i < scenesNum; i++) {
			xml->pushTag("scene", i);

			Scene* sc = new Scene();
			sc->LoadFromXml(xml);
			this->scenes.push_back(sc);

			if (sc->GetName().compare(initialScene) == 0) {
				// set as initial
				AddScene(sc, true);
			}
			else {
				AddScene(sc, false);
			}

			xml->popTag();
		}
	}


	void SceneContext::SendMessageToBehaviors(Msg& msg, Node* actualNode) {
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

	void SceneContext::SendBubblingMessageToChildren(Msg& msg, Node* actualNode) {
		for (auto it = actualNode->GetChildren().begin(); it != actualNode->GetChildren().end(); ++it) {
			CogSendMessage(msg, (*it));
		}
	}


	void SceneContext::SendBubblingMessage(Msg& msg, Node* actualNode) {

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

	void SceneContext::SendDirectMessage(Msg& msg) {
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