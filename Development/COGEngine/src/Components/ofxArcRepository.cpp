
#include "ofxArcRepository.h"



void ofxArcRepository::SendMessage(ofxAreMsg& msg, ofxAreNode* actualNode){
	// there is no such callback or behavior that listens to that type of message
	if (!IsRegisteredListener(msg.GetAction())) return;

	BubblingType& trav = msg.GetBubblingType();

	if (trav.scope == ScopeType::DIRECT_NO_TRAVERSE){
		// no BubblingType - just iterate over the proper collection of behaviors and callbacks
		SendDirectMessage(msg);

	}else SendBubblingMessage(msg, actualNode);
}


void ofxArcRepository::SendDirectMessageToBehavior(ofxAreMsg& msg, int targetId){
	ofxAreBehavior* beh = FindBehaviorById(targetId);

	if (beh != nullptr){
		beh->OnMessage(msg);
	}
}

void ofxArcRepository::SendDirectMessageToNode(ofxAreMsg& msg, int targetId){
	ofxAreNode* node = FindNodeById(targetId);

	if (node != nullptr){
		SendBubblingMessage(msg, node);
	}
}


ofxAreNode* ofxArcRepository::FindNodeById(int id) const{
	for (auto it = allNodes.begin(); it != allNodes.end(); ++it){
		if ((*it)->GetId() == id) return (*it);
	}
	return nullptr;
}

ofxAreBehavior* ofxArcRepository::FindBehaviorById(int id) const{
	for (auto it = allBehaviors.begin(); it != allBehaviors.end(); ++it){
		if ((*it)->GetId() == id) return (*it);
	}
	return nullptr;
}

int ofxArcRepository::GetNodesCountByTag(string tag) const{
	int counter = 0;
	for (auto it = allNodes.begin(); it != allNodes.end(); ++it){
		if ((*it)->GetTag().compare(tag) == 0) counter++;
	}
	return counter;
}

ofxAreNode* ofxArcRepository::FindNodeByTag(string tag) const{
	for (auto it = allNodes.begin(); it != allNodes.end(); ++it){
		if ((*it)->GetTag().compare(tag) == 0) return (*it);
	}
	return nullptr;
}

vector<ofxAreNode*> ofxArcRepository::FindNodesByTag(char* tag) const{
	vector<ofxAreNode*> output;

	for (auto it = allNodes.begin(); it != allNodes.end(); ++it){
		if ((*it)->GetTag().compare(tag) == 0) output.push_back(*it);
	}
	return output;
}

int ofxArcRepository::GetNodesCountBySubType(int subtype) const{
	int counter = 0;

	for (auto it = allNodes.begin(); it != allNodes.end(); ++it){
		if ((*it)->GetSubType() == subtype) counter++;
	}
	return counter;
}

ofxAreNode* ofxArcRepository::FindNodeBySubType(int subtype) const{
	for (auto it = allNodes.begin(); it != allNodes.end(); ++it){
		if ((*it)->GetSubType() == subtype) return (*it);
	}
	return nullptr;
}

vector<ofxAreNode*> ofxArcRepository::FindNodesBySubType(int subtype) const{
	vector<ofxAreNode*> output;

	for (auto it = allNodes.begin(); it != allNodes.end(); ++it){
		if ((*it)->GetSubType() == subtype) output.push_back(*it);
	}
	return output;
}


void ofxArcRepository::SendMessageToBehaviors(ofxAreMsg& msg, ofxAreNode* actualNode){
	for (auto it = actualNode->GetBehaviors().begin(); it != actualNode->GetBehaviors().end(); ++it){
		ofxAreBehavior* beh = (*it);
		if ((beh->GetBehState() == BehState::ACTIVE_MESSAGES || beh->GetBehState() == BehState::ACTIVE_ALL) &&
			(beh->GetId() != msg.GetBehaviorId())){
			if (IsRegisteredListener(msg.GetAction(), beh)){
				beh->OnMessage(msg);
			}
		}
	}
}

void ofxArcRepository::SendBubblingMessageToChildren(ofxAreMsg& msg, ofxAreNode* actualNode){
	for (auto it = actualNode->GetChildren().begin(); it != actualNode->GetChildren().end(); ++it){
		SendMessage(msg, (*it));
	}
}


void ofxArcRepository::SendBubblingMessage(ofxAreMsg& msg, ofxAreNode* actualNode){

	BubblingType& trav = msg.GetBubblingType();

	if (trav.scope == ScopeType::ROOT){
		trav.scope = ScopeType::OBJECT;
		// find root and call recursion
		ofxAreNode* root = actualNode->GetRoot();
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
		ofxAreNode* scRoot = actualNode->GetSceneRoot();
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

void ofxArcRepository::SendDirectMessage(ofxAreMsg& msg){
	auto behaviors = behListeners.find(msg.GetAction());

	if (behaviors != behListeners.end()){
		vector<ofxAreBehavior*>& behs = behaviors->second;

		for (auto it = behs.begin(); it != behs.end(); ++it){
			if (((*it)->GetBehState() == BehState::ACTIVE_MESSAGES || (*it)->GetBehState() == BehState::ACTIVE_ALL)){
				(*it)->OnMessage(msg);
			}
		}
	}
}