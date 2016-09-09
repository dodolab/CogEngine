#ifndef GAMESTORAGE_H
#define GAMESTORAGE_H

#include <map>
#include "GNode.h"
#include "GBehavior.h"

/**
* MGameStorage - game object manager
*/
class MGameStorage{
private:
	map<int, vector<MsgCallback>> callBackListeners;
	map<int, vector<GBehavior*>> behListeners;

	// list of all game objects
	vector<GNode*> allGameObjects;
	// list of all behaviors
	vector<GBehavior*> allBehaviors;

	void SendMessageToBehaviors(GMsg& msg, GNode* actualNode);

	void SendTraversationMessageToChildren(GMsg& msg, GNode* actualNode);

public:

	void Init(){

	}
	void Terminate(){

	}

	MGameStorage(){

	}

	void RegisterCallback(int action, MsgCallback callback){
		if (callBackListeners.find(action) == callBackListeners.end()){
			callBackListeners[action] = vector < MsgCallback >();
		}

		vector<MsgCallback> callBacks = callBackListeners.find(action)->second;
		callBacks.push_back(callback);
	}

	void UnregisterCallback(int action, MsgCallback callback){
		if (callBackListeners.find(action) != callBackListeners.end()){
			vector<MsgCallback> callBacks = callBackListeners.find(action)->second;
			for (auto it = callBacks.begin(); it != callBacks.end(); ++it){
				MsgCallback cb = (*it);
				if (&cb == &callback){
					callBacks.erase(it);
					return;
				}
			}
		}
	}

	void SendTraversationMessage(GMsg& msg, GNode* actualNode);

	void SendMessage(GMsg& msg);

	bool IsRegisteredListener(int key) const{
		return behListeners.find(key) != behListeners.end();;
	}

	GNode* FindGameObjectById(int id) const{
		/*for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
			if ((*it)->GetId() == id) return (*it);
		}*/
		return nullptr;
	}
	GNode* FindGameObjectByTag(string tag) const{
		/*for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
			if ((*it)->GetTag() == tag) return (*it);
		}*/
		return nullptr;
	}
	vector<GNode*> FindGameObjectsByTag(char* tag) const{
		/*vector<GNode*> output;

		for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
			if ((*it)->GetTag() == tag) output.push_back(*it);
		}
		return output;*/
		return vector < GNode* > ();
	}
	GNode* FindGameObjectBySubType(int subtype) const{
		/*for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
			if ((*it)->GetSubType() == subtype) return (*it);
		}*/
		return nullptr;
	}
	vector<GNode*> FindGameObjectsBySubType(int subtype) const{
		/*vector<GNode*> output;

		for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
			if ((*it)->GetSubType() == subtype) output.push_back(*it);
		}
		return output;*/

		return vector<GNode*>();
	}

	void OnMessage(GMsg msg){

	}
};

#endif
