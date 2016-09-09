#pragma once

#include <map>
#include "GNode.h"
#include "GBehavior.h"
#include <functional>

/**
* Game object repository
*/
class MGameStorage{
private:

	// id counter
	static int callBackIdCounter;
	// callback listeners
	map<int, vector<std::pair<int,MsgCallback>>> callBackListeners;
	// behavior listeners
	map<int, vector<GBehavior*>> behListeners;
	// behavior ids and their registered actions
	map<int, vector<int>> behListenerActions;
	// list of all game objects
	vector<GNode*> allGameObjects;
	// list of all behaviors
	vector<GBehavior*> allBehaviors;

	/**
	* Sends message to behaviors
	* @param msg message to send
	* @param actualNode actual node in bubbling algorithm
	*/
	void SendMessageToBehaviors(GMsg& msg, GNode* actualNode);

	/**
	* Sends message to children of actual node
	* @param msg message to send
	* @param actualNode actual node in bubbling algorithm
	*/
	void SendBubblingMessageToChildren(GMsg& msg, GNode* actualNode);

	/**
	* Sends message that will be bubbled from actualNode
	* @param msg message to send
	* @param actualNode actual node in bubbling algorithm
	*/
	void SendBubblingMessage(GMsg& msg, GNode* actualNode);

	/**
	* Sends direct message - takes all behaviors and callbacks that have
	* registered selected action
	* @param msg message  to send
	*/
	void SendDirectMessage(GMsg& msg);

public:

	/**
	* Registers callback for selected action
	* @param action action to register
	*/
	template<class T>
	int RegisterCallback(int action, void (T::*f)(GMsg &)) {
		return RegisterCallback(action, std::tr1::bind(f, (T*)(this), std::placeholders::_1));
	}

	/**
	* Registers callback for selected action
	* @param action action to register
	* @param callback pointer to method that will be called when selected action is invoked
	*/
	int RegisterCallback(int action, MsgCallback callback){
		if (callBackListeners.find(action) == callBackListeners.end()){
			callBackListeners[action] = vector <std::pair<int, MsgCallback >>();
		}

		vector<std::pair<int,MsgCallback>>& callBacks = callBackListeners.find(action)->second;
		callBacks.push_back(std::make_pair(callBackIdCounter++, callback));

		// return id of this callback
		return callBackIdCounter-1;
	}

	/**
	* Unregisters callback for selected action
	* @return true if callback has been found and erased
	*/
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

	/**
	* Registers behavior listener for selected action
	* @param action action to register
	* @param beh beh that will be called when selected action is invoked
	*/
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

	/**
	* Unregisters behavior listener for selected action
	* @return true if behavior has been found and erased
	*/
	bool UnregisterListener(int action, GBehavior* beh){
		if (behListeners.find(action) != behListeners.end()){
			vector<GBehavior*>& listeners = behListeners[action];

			for (auto it = listeners.begin(); it != listeners.end(); ++it){
				if ((*it)->GetId() == beh->GetId()){
					listeners.erase(it);
					return true;
				}
			}
		}
		return false;
	}

	/**
	* Unregisters all actions that are bound with selected behavior
	* @param beh behavior to unregister
	*/
	void UnregisterListener(GBehavior* beh){
		auto found = behListenerActions.find(beh->GetId());

		if (found != behListenerActions.end()){

			vector<int> actions = found->second;

			// unregister all actions
			for (auto action : actions){
				UnregisterListener(action, beh);
			}

			// remove from the second collection
			behListenerActions.erase(beh->GetId());
		}
	}

	/**
	* Sends common message
	* @param msg message  to send
	* @param actualNode actual node in bubbling algorithm
	*/
	void SendMessage(GMsg& msg, GNode* actualNode);

	/**
	* Sends message to specific behavior
	* @param msg message  to send
	* @param targetId id of target behavior
	*/
	void SendDirectMessageToBehavior(GMsg& msg, int targetId);

	/**
	* Sends message to specific game object
	* @param msg message  to send
	* @param targetId id of target game object
	*/
	void SendDirectMessageToGameObject(GMsg& msg, int targetId);

	/**
	* Returns true, if there is at least one behavior listener for selected action
	*/
	bool IsRegisteredListener(int action) const{
		return behListeners.find(action) != behListeners.end();
	}

	/**
	* Returns true, if behavior is listening for selected action
	*/
	bool IsRegisteredListener(int action, GBehavior* beh){
		if (behListenerActions.find(beh->GetId()) == behListenerActions.end()) return false;

		vector<int>& actions = behListenerActions[beh->GetId()];

		return (std::find(actions.begin(), actions.end(), action) != actions.end());
	}

	/**
	* Returns true, if there is at least one callback listener for selected action
	*/
	bool IsRegisteredCallBack(int key) const{
		return callBackListeners.find(key) != callBackListeners.end();
	}

	/**
	* Finds game object by id
	*/
	GNode* FindGameObjectById(int id) const;

	/**
	* Finds behavior by id
	*/
	GBehavior* FindBehaviorById(int id) const;

	/**
	* Gets number of game objects with specific tag
	*/
	int GetGameObjectsCountByTag(string tag) const;

	/**
	* Finds game object by tag
	*/
	GNode* FindGameObjectByTag(string tag) const;

	/**
	* Finds all game objects by tag
	*/
	vector<GNode*> FindGameObjectsByTag(char* tag) const;

	/**
	* Gets number of game objects with specific subtype
	*/
	int GetGameObjectsCountBySubType(int subtype) const;

	/**
	* Finds game object by subtype
	*/
	GNode* FindGameObjectBySubType(int subtype) const;

	/**
	* Finds all game objects by subtype
	*/
	vector<GNode*> FindGameObjectsBySubType(int subtype) const;

	/**
	* Adds a new game object to the collection
	* @return true, if game object has been added
	*/
	bool AddGameObject(GNode* gameObject){
		auto found = find(allGameObjects.begin(), allGameObjects.end(), gameObject);
		if (found == allGameObjects.end()){
			allGameObjects.push_back(gameObject);
			return true;
		}
		else return false;
	}

	/**
	* Removes game object from collection
	*/
	void RemoveGameObject(GNode* gameObject){
		auto found = find(allGameObjects.begin(), allGameObjects.end(), gameObject);
		if (found != allGameObjects.end()) allGameObjects.erase(found);
	}

	/**
	* Adds a new behavior
	* @return true if behavior has been added
	*/
	bool AddBehavior(GBehavior* beh){
		auto found = find(allBehaviors.begin(), allBehaviors.end(), beh);
		if (found == allBehaviors.end()){
			allBehaviors.push_back(beh);

			return true;
		}
		else return false;
	}

	/**
	* Removes behavior from collection
	*/
	void RemoveBehavior(GBehavior* beh){
		auto found = find(allBehaviors.begin(), allBehaviors.end(), beh);
		if (found != allBehaviors.end()) allBehaviors.erase(found);

		UnregisterListener(beh);
	}

};
