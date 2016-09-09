#pragma once

#include <map>
#include "GNode.h"
#include "GBehavior.h"
#include <functional>



/**
* MGameStorage - game object manager
*/
class MGameStorage{
private:
	map<int, vector<std::pair<int,MsgCallback>>> callBackListeners;
	map<int, vector<GBehavior*>> behListeners;
	map<int, vector<int>> behListenerActions;

	static int callBackIdCounter;
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

	template<class T>
	int RegisterCallback(int action, void (T::*f)(GMsg &)) {
		return RegisterCallback(action, std::tr1::bind(f, (T*)(this), std::placeholders::_1));
	}

	int RegisterCallback(int action, MsgCallback callback){
		if (callBackListeners.find(action) == callBackListeners.end()){
			callBackListeners[action] = vector <std::pair<int, MsgCallback >>();
		}

		vector<std::pair<int,MsgCallback>>& callBacks = callBackListeners.find(action)->second;
		callBacks.push_back(std::make_pair(callBackIdCounter++, callback));

		// return id of this callback
		return callBackIdCounter-1;
	}

	bool UnregisterCallback(int action, int id){
		if (callBackListeners.find(action) != callBackListeners.end()){
			vector<std::pair<int,MsgCallback>>& callBacks = callBackListeners.find(action)->second;
			
			for (auto it = callBacks.begin(); it != callBacks.end(); ++it){

				if ((*it).first == id){
					callBacks.erase(it);
					return true;
				}
			}
		}

		return false;
	}

	void RegisterListener(int action, GBehavior* beh){
		if (behListeners.find(action) == behListeners.end()){
			behListeners[action] = vector <GBehavior*>();
		}

		vector<GBehavior*>& listeners = behListeners[action];
		listeners.push_back(beh);

		if (behListenerActions.find(beh->GetId()) == behListenerActions.end()){
			behListenerActions[beh->GetId()] = vector<int>();
		}

		behListenerActions[beh->GetId()].push_back(action);
	}

	void UnregisterListener(int action, GBehavior* beh){
		if (behListeners.find(action) != behListeners.end()){
			vector<GBehavior*>& listeners = behListeners[action];

			for (auto it = listeners.begin(); it != listeners.end(); ++it){
				if ((*it)->GetId() == beh->GetId()){
					listeners.erase(it);
					return;
				}
			}
		}
	}

	void UnregisterListener(GBehavior* beh){
		if (behListenerActions.find(beh->GetId()) != behListenerActions.end()){
			behListenerActions.erase(beh->GetId());
		}
	}

	void SendTraversationMessage(GMsg& msg, GNode* actualNode);

	void SendMessage(GMsg& msg);

	bool IsRegisteredListener(int key) const{
		return behListeners.find(key) != behListeners.end();
	}

	bool IsRegisteredListener(int action, GBehavior* beh){
		if (behListenerActions.find(beh->GetId()) == behListenerActions.end()) return false;

		vector<int>& actions = behListenerActions[beh->GetId()];

		return (std::find(actions.begin(), actions.end(), action) != actions.end());
	}

	bool IsRegisteredCallBack(int key) const{
		return callBackListeners.find(key) != callBackListeners.end();
	}

	GNode* FindGameObjectById(int id) const;

	int GetGameObjectsCountByTag(string tag) const;

	GNode* FindGameObjectByTag(string tag) const;

	vector<GNode*> FindGameObjectsByTag(char* tag) const;

	int GetGameObjectsCountBySubType(int subtype) const;

	GNode* FindGameObjectBySubType(int subtype) const;

	vector<GNode*> FindGameObjectsBySubType(int subtype) const;

	bool AddGameObject(GNode* gameObject){
		auto found = find(allGameObjects.begin(), allGameObjects.end(), gameObject);
		if (found == allGameObjects.end()){
			allGameObjects.push_back(gameObject);
			return true;
		}
		else return false;
	}

	void RemoveGameObject(GNode* gameObject){
		auto found = find(allGameObjects.begin(), allGameObjects.end(), gameObject);
		if (found != allGameObjects.end()) allGameObjects.erase(found);
	}

	bool AddBehavior(GBehavior* beh){
		auto found = find(allBehaviors.begin(), allBehaviors.end(), beh);
		if (found == allBehaviors.end()){
			allBehaviors.push_back(beh);

			return true;
		}
		else return false;
	}

	void RemoveBehavior(GBehavior* beh){
		auto found = find(allBehaviors.begin(), allBehaviors.end(), beh);
		if (found != allBehaviors.end()) allBehaviors.erase(found);

		UnregisterListener(beh);
	}

	void OnMessage(GMsg msg){

	}
};
