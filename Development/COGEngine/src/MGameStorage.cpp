
#include "MGameStorage.h"

// first id is always 1
int MGameStorage::callBackIdCounter = 1;

void MGameStorage::SendMessageToBehaviors(GMsg& msg, GNode* actualNode){
	for (auto it = actualNode->GetBehaviors().begin(); it != actualNode->GetBehaviors().end(); ++it){
		GBehavior* beh = (*it);
		if ((beh->GetBehState() == BehState::ACTIVE_MESSAGES || beh->GetBehState() == BehState::ACTIVE_ALL) &&
			(beh->GetId() != msg.GetBehaviorId()) &&
			(msg.GetCategory() == ElemType::ALL || beh->GetElemType() == msg.GetCategory())){
			if (beh->GetMessageFlags().HasState(msg.GetAction())){
				beh->OnMessage(msg);
			}
		}
	}
}

void MGameStorage::SendTraversationMessageToChildren(GMsg& msg, GNode* actualNode){
	for (auto it = actualNode->GetChildren().begin(); it != actualNode->GetChildren().end(); ++it){
		SendTraversationMessage(msg, (*it));
	}
}

void MGameStorage::SendTraversationMessage(GMsg& msg, GNode* actualNode){

	Traversation& trav = msg.GetTraverse();

	if (trav.scope == ScopeType::ROOT){
		trav.scope = ScopeType::OBJECT;
		// find root and call recursion
		GNode* root = actualNode->GetRoot();
		if (root != nullptr){
			if (trav.deep && !trav.bubbleDown) SendTraversationMessageToChildren(msg, root);
			SendMessageToBehaviors(msg, root);
			if (trav.deep && trav.bubbleDown) SendTraversationMessageToChildren(msg, root);
		}
		else return;
	}
	else if (trav.scope == ScopeType::SCENE){
		trav.scope = ScopeType::OBJECT;
		// find scene and call recursion
		GNode* scRoot = actualNode->GetSceneRoot();
		if (scRoot != nullptr){
			if (trav.deep && !trav.bubbleDown) SendTraversationMessageToChildren(msg, scRoot);
			SendMessageToBehaviors(msg, scRoot);
			if (trav.deep && trav.bubbleDown) SendTraversationMessageToChildren(msg, scRoot);
		}
		else return;
	}

	if (trav.scope == ScopeType::OBJECT){
		trav.scope = ScopeType::OBJECT;
		if (trav.deep && !trav.bubbleDown) SendTraversationMessageToChildren(msg, actualNode);
		SendMessageToBehaviors(msg, actualNode);
		if (trav.deep && trav.bubbleDown) SendTraversationMessageToChildren(msg, actualNode);
	}
	else if (trav.scope == ScopeType::CHILDREN){
		trav.scope = ScopeType::OBJECT;
		SendTraversationMessageToChildren(msg, actualNode);
	}
}

void MGameStorage::SendMessage(GMsg& msg){
	auto callBack = callBackListeners.find(msg.GetAction());

	if (callBack != callBackListeners.end()){
		vector<std::pair<int,MsgCallback>>& callBacks = callBack->second;

		for (auto it = callBacks.begin(); it != callBacks.end(); ++it){
			MsgCallback cb = (*it).second;
			cb(msg);
		}
	}

	auto behCol = behListeners.find(msg.GetAction());

	if (behCol != behListeners.end()){
		vector<GBehavior*>& behaviors = behCol->second;

		for (auto it = behaviors.begin(); it != behaviors.end(); ++it){
			(*it)->OnMessage(msg);
		}
	}
}