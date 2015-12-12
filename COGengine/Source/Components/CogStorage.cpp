#include "CogStorage.h"

namespace Cog {

	void CogRepository::SendMessage(CogMsg& msg, CogNode* actualNode) {
		// there is no such callback or behavior that listens to that type of message
		if (!IsRegisteredListener(msg.GetAction())) return;

		CogBubblingType& trav = msg.GetBubblingType();

		if (trav.scope == CogScope::DIRECT_NO_TRAVERSE) {
			// no BubblingType - just iterate over the proper collection of behaviors and callbacks
			SendDirectMessage(msg);

		}
		else SendBubblingMessage(msg, actualNode);
	}


	void CogRepository::SendDirectMessageToBehavior(CogMsg& msg, int targetId) {
		CogBehavior* beh = FindBehaviorById(targetId);

		if (beh != nullptr) {
			beh->OnMessage(msg);
		}
	}

	void CogRepository::SendDirectMessageToNode(CogMsg& msg, int targetId) {
		CogNode* node = FindNodeById(targetId);

		if (node != nullptr) {
			SendBubblingMessage(msg, node);
		}
	}


	CogNode* CogRepository::FindNodeById(int id) const {
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetId() == id) return (*it);
		}
		return nullptr;
	}

	CogBehavior* CogRepository::FindBehaviorById(int id) const {
		for (auto it = allBehaviors.begin(); it != allBehaviors.end(); ++it) {
			if ((*it)->GetId() == id) return (*it);
		}
		return nullptr;
	}

	int CogRepository::GetNodesCountByTag(string tag) const {
		int counter = 0;
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetTag().compare(tag) == 0) counter++;
		}
		return counter;
	}

	CogNode* CogRepository::FindNodeByTag(string tag) const {
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetTag().compare(tag) == 0) return (*it);
		}
		return nullptr;
	}

	vector<CogNode*> CogRepository::FindNodesByTag(char* tag) const {
		vector<CogNode*> output;

		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetTag().compare(tag) == 0) output.push_back(*it);
		}
		return output;
	}

	int CogRepository::GetNodesCountBySubType(int subtype) const {
		int counter = 0;

		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetSubType() == subtype) counter++;
		}
		return counter;
	}

	CogNode* CogRepository::FindNodeBySubType(int subtype) const {
		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetSubType() == subtype) return (*it);
		}
		return nullptr;
	}

	vector<CogNode*> CogRepository::FindNodesBySubType(int subtype) const {
		vector<CogNode*> output;

		for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
			if ((*it)->GetSubType() == subtype) output.push_back(*it);
		}
		return output;
	}


	void CogRepository::SendMessageToBehaviors(CogMsg& msg, CogNode* actualNode) {
		for (auto it = actualNode->GetBehaviors().begin(); it != actualNode->GetBehaviors().end(); ++it) {
			CogBehavior* beh = (*it);
			if ((beh->GetBehState() == CogBehState::ACTIVE_MESSAGES || beh->GetBehState() == CogBehState::ACTIVE_ALL) &&
				(beh->GetId() != msg.GetBehaviorId())) {
				if (IsRegisteredListener(msg.GetAction(), beh)) {
					beh->OnMessage(msg);
				}
			}
		}
	}

	void CogRepository::SendBubblingMessageToChildren(CogMsg& msg, CogNode* actualNode) {
		for (auto it = actualNode->GetChildren().begin(); it != actualNode->GetChildren().end(); ++it) {
			SendMessage(msg, (*it));
		}
	}


	void CogRepository::SendBubblingMessage(CogMsg& msg, CogNode* actualNode) {

		CogBubblingType& trav = msg.GetBubblingType();

		if (trav.scope == CogScope::ROOT) {
			trav.scope = CogScope::OBJECT;
			// find root and call recursion
			CogNode* root = actualNode->GetRoot();
			if (root != nullptr) {
				// call this method again from the root
				if (trav.deep && !trav.bubbleDown) SendBubblingMessageToChildren(msg, root);
				SendMessageToBehaviors(msg, root);
				if (trav.deep && trav.bubbleDown) SendBubblingMessageToChildren(msg, root);
			}
			return;
		}
		else if (trav.scope == CogScope::SCENE) {
			trav.scope = CogScope::OBJECT;
			// find scene and call recursion
			CogNode* scRoot = actualNode->GetSceneRoot();
			if (scRoot != nullptr) {
				if (trav.deep && !trav.bubbleDown) SendBubblingMessageToChildren(msg, scRoot);
				SendMessageToBehaviors(msg, scRoot);
				if (trav.deep && trav.bubbleDown) SendBubblingMessageToChildren(msg, scRoot);
			}
			return;
		}

		if (trav.scope == CogScope::OBJECT) {
			trav.scope = CogScope::OBJECT;
			// call children and itself
			if (trav.deep && !trav.bubbleDown) SendBubblingMessageToChildren(msg, actualNode);
			SendMessageToBehaviors(msg, actualNode);
			if (trav.deep && trav.bubbleDown) SendBubblingMessageToChildren(msg, actualNode);
		}
		else if (trav.scope == CogScope::CHILDREN) {
			trav.scope = CogScope::OBJECT;
			// call children only
			SendBubblingMessageToChildren(msg, actualNode);
		}
	}

	void CogRepository::SendDirectMessage(CogMsg& msg) {
		auto behaviors = behListeners.find(msg.GetAction());

		if (behaviors != behListeners.end()) {
			vector<CogBehavior*>& behs = behaviors->second;

			for (auto it = behs.begin(); it != behs.end(); ++it) {
				if (((*it)->GetBehState() == CogBehState::ACTIVE_MESSAGES || (*it)->GetBehState() == CogBehState::ACTIVE_ALL)) {
					(*it)->OnMessage(msg);
				}
			}
		}
	}

}