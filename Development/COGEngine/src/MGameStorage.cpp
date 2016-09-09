
#include "MGameStorage.h"

// first id is always 1
int MGameStorage::callBackIdCounter = 1;

void MGameStorage::SendMessageToBehaviors(GMsg& msg, GNode* actualNode){
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

void MGameStorage::SendBubblingMessageToChildren(GMsg& msg, GNode* actualNode){
	for (auto it = actualNode->GetChildren().begin(); it != actualNode->GetChildren().end(); ++it){
		SendMessage(msg, (*it));
	}
}


void MGameStorage::SendMessage(GMsg& msg, GNode* actualNode){
	// there is no such callback or behavior that listens to that type of message
	if (!IsRegisteredCallBack(msg.GetAction()) && !IsRegisteredListener(msg.GetAction())) return;

	BubblingType& trav = msg.GetBubblingType();

	if (trav.scope == ScopeType::DIRECT_NO_TRAVERSE){
		// no BubblingType - just iterate over the proper collection of behaviors and callbacks
		SendDirectMessage(msg);

	}else SendBubblingMessage(msg, actualNode);
}

void MGameStorage::SendBubblingMessage(GMsg& msg, GNode* actualNode){

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

void MGameStorage::SendDirectMessage(GMsg& msg){
	auto behaviors = behListeners.find(msg.GetAction());

	if (behaviors != behListeners.end()){
		vector<GBehavior*>& behs = behaviors->second;

		for (auto it = behs.begin(); it != behs.end(); ++it){
			if (((*it)->GetBehState() == BehState::ACTIVE_MESSAGES || (*it)->GetBehState() == BehState::ACTIVE_ALL)){
				(*it)->OnMessage(msg);
			}
		}
	}

	auto callBacks = callBackListeners.find(msg.GetAction());

	if (callBacks != callBackListeners.end()){
		vector<std::pair<int, MsgCallback>>& cbck = callBacks->second;

		for (auto it = cbck.begin(); it != cbck.end(); ++it){
			std::pair<int, MsgCallback>& pair = (*it);
			pair.second(msg);
		}
	}
}

void MGameStorage::SendDirectMessageToBehavior(GMsg& msg, int targetId){
	GBehavior* beh = FindBehaviorById(targetId);

	if (beh != nullptr){
		beh->OnMessage(msg);
	}
}

void MGameStorage::SendDirectMessageToGameObject(GMsg& msg, int targetId){
	GNode* node = FindGameObjectById(targetId);

	if (node != nullptr){
		SendBubblingMessage(msg, node);
	}
}


GNode* MGameStorage::FindGameObjectById(int id) const{
	for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
		if ((*it)->GetId() == id) return (*it);
	}
	return nullptr;
}

GBehavior* MGameStorage::FindBehaviorById(int id) const{
	for (auto it = allBehaviors.begin(); it != allBehaviors.end(); ++it){
		if ((*it)->GetId() == id) return (*it);
	}
	return nullptr;
}

int MGameStorage::GetGameObjectsCountByTag(string tag) const{
	int counter = 0;
	for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
		if ((*it)->GetTag().compare(tag) == 0) counter++;
	}
	return counter;
}

GNode* MGameStorage::FindGameObjectByTag(string tag) const{
	for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
		if ((*it)->GetTag().compare(tag) == 0) return (*it);
	}
	return nullptr;
}

vector<GNode*> MGameStorage::FindGameObjectsByTag(char* tag) const{
	vector<GNode*> output;

	for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
		if ((*it)->GetTag().compare(tag) == 0) output.push_back(*it);
	}
	return output;
}

int MGameStorage::GetGameObjectsCountBySubType(int subtype) const{
	int counter = 0;

	for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
		if ((*it)->GetSubType() == subtype) counter++;
	}
	return counter;
}

GNode* MGameStorage::FindGameObjectBySubType(int subtype) const{
	for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
		if ((*it)->GetSubType() == subtype) return (*it);
	}
	return nullptr;
}

vector<GNode*> MGameStorage::FindGameObjectsBySubType(int subtype) const{
	vector<GNode*> output;

	for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
		if ((*it)->GetSubType() == subtype) output.push_back(*it);
	}
	return output;
}