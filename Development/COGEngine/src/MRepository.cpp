
#include "MRepository.h"


void MRepository::SendMessageToBehaviors(GMsg& msg, GNode* actualNode){
	for (auto it = actualNode->GetBehaviors().begin(); it != actualNode->GetBehaviors().end(); ++it){
		GBehavior* beh = (*it);
		if ((beh->GetBehState() == BehState::ACTIVE_MESSAGES || beh->GetBehState() == BehState::ACTIVE_ALL) &&
			(beh->GetId() != msg.GetBehaviorId()) &&
			(beh->GetElemType() == msg.GetElemType())){
			if (IsRegisteredListener(msg.GetAction(), beh)){
				beh->OnMessage(msg);
			}
		}
	}
}

void MRepository::SendBubblingMessageToChildren(GMsg& msg, GNode* actualNode){
	for (auto it = actualNode->GetChildren().begin(); it != actualNode->GetChildren().end(); ++it){
		SendMessage(msg, (*it));
	}
}


void MRepository::SendMessage(GMsg& msg, GNode* actualNode){
	// there is no such callback or behavior that listens to that type of message
	if (!IsRegisteredListener(msg.GetAction())) return;

	BubblingType& trav = msg.GetBubblingType();

	if (trav.scope == ScopeType::DIRECT_NO_TRAVERSE){
		// no BubblingType - just iterate over the proper collection of behaviors and callbacks
		SendDirectMessage(msg);

	}else SendBubblingMessage(msg, actualNode);
}

void MRepository::SendBubblingMessage(GMsg& msg, GNode* actualNode){

	BubblingType& trav = msg.GetBubblingType();

	if (trav.scope == ScopeType::ROOT){
		trav.scope = ScopeType::OBJECT;
		// find root and call recursion
		GNode* root = actualNode->GetRoot();
		if (root != nullptr){
			// call this method again from the root
			if (trav.deep && !trav.bubbleDown) SendBubblingMessageToChildren(msg, root);
			SendMessageToBehaviors(msg, root);
			if (trav.deep && trav.bubbleDown) SendBubblingMessageToChildren(msg, root);
		}
		return;
	}
	else if (trav.scope == ScopeType::SCENE){
		trav.scope = ScopeType::OBJECT;
		// find scene and call recursion
		GNode* scRoot = actualNode->GetSceneRoot();
		if (scRoot != nullptr){
			if (trav.deep && !trav.bubbleDown) SendBubblingMessageToChildren(msg, scRoot);
			SendMessageToBehaviors(msg, scRoot);
			if (trav.deep && trav.bubbleDown) SendBubblingMessageToChildren(msg, scRoot);
		}
		return;
	}

	if (trav.scope == ScopeType::OBJECT){
		trav.scope = ScopeType::OBJECT;
		// call children and itself
		if (trav.deep && !trav.bubbleDown) SendBubblingMessageToChildren(msg, actualNode);
		SendMessageToBehaviors(msg, actualNode);
		if (trav.deep && trav.bubbleDown) SendBubblingMessageToChildren(msg, actualNode);
	}
	else if (trav.scope == ScopeType::CHILDREN){
		trav.scope = ScopeType::OBJECT;
		// call children only
		SendBubblingMessageToChildren(msg, actualNode);
	}
}

void MRepository::SendDirectMessage(GMsg& msg){
	auto behaviors = behListeners.find(msg.GetAction());

	if (behaviors != behListeners.end()){
		vector<GBehavior*>& behs = behaviors->second;

		for (auto it = behs.begin(); it != behs.end(); ++it){
			if (((*it)->GetBehState() == BehState::ACTIVE_MESSAGES || (*it)->GetBehState() == BehState::ACTIVE_ALL)){
				(*it)->OnMessage(msg);
			}
		}
	}
}

void MRepository::SendDirectMessageToBehavior(GMsg& msg, int targetId){
	GBehavior* beh = FindBehaviorById(targetId);

	if (beh != nullptr){
		beh->OnMessage(msg);
	}
}

void MRepository::SendDirectMessageToGameObject(GMsg& msg, int targetId){
	GNode* node = FindGameObjectById(targetId);

	if (node != nullptr){
		SendBubblingMessage(msg, node);
	}
}


GNode* MRepository::FindGameObjectById(int id) const{
	for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
		if ((*it)->GetId() == id) return (*it);
	}
	return nullptr;
}

GBehavior* MRepository::FindBehaviorById(int id) const{
	for (auto it = allBehaviors.begin(); it != allBehaviors.end(); ++it){
		if ((*it)->GetId() == id) return (*it);
	}
	return nullptr;
}

int MRepository::GetGameObjectsCountByTag(string tag) const{
	int counter = 0;
	for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
		if ((*it)->GetTag().compare(tag) == 0) counter++;
	}
	return counter;
}

GNode* MRepository::FindGameObjectByTag(string tag) const{
	for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
		if ((*it)->GetTag().compare(tag) == 0) return (*it);
	}
	return nullptr;
}

vector<GNode*> MRepository::FindGameObjectsByTag(char* tag) const{
	vector<GNode*> output;

	for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
		if ((*it)->GetTag().compare(tag) == 0) output.push_back(*it);
	}
	return output;
}

int MRepository::GetGameObjectsCountBySubType(int subtype) const{
	int counter = 0;

	for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
		if ((*it)->GetSubType() == subtype) counter++;
	}
	return counter;
}

GNode* MRepository::FindGameObjectBySubType(int subtype) const{
	for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
		if ((*it)->GetSubType() == subtype) return (*it);
	}
	return nullptr;
}

vector<GNode*> MRepository::FindGameObjectsBySubType(int subtype) const{
	vector<GNode*> output;

	for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
		if ((*it)->GetSubType() == subtype) output.push_back(*it);
	}
	return output;
}