#pragma once

#include "CogNode.h"
#include "CogBehavior.h"

/**
* Node repository
*/
class CogRepository{
private:
	// behavior listeners
	map<int, vector<CogBehavior*>> behListeners;
	// behavior ids and their registered actions
	map<int, vector<int>> behListenerActions;
	// list of all nodes
	vector<CogNode*> allNodes;
	// list of all behaviors
	vector<CogBehavior*> allBehaviors;

public:


	/**
	* Registers behavior listener for selected action
	* @param action action to register
	* @param beh beh that will be called when selected action is invoked
	*/
	void RegisterListener(int action, CogBehavior* beh){
		if (behListeners.find(action) == behListeners.end()){
			behListeners[action] = vector <CogBehavior*>();
		}

		vector<CogBehavior*>& listeners = behListeners[action];
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
	bool UnregisterListener(int action, CogBehavior* beh){
		if (behListeners.find(action) != behListeners.end()){
			vector<CogBehavior*>& listeners = behListeners[action];

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
	void UnregisterListener(CogBehavior* beh){
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
	void SendMessage(CogMsg& msg, CogNode* actualNode);

	/**
	* Sends message to specific behavior
	* @param msg message  to send
	* @param targetId id of target behavior
	*/
	void SendDirectMessageToBehavior(CogMsg& msg, int targetId);

	/**
	* Sends message to specific node
	* @param msg message  to send
	* @param targetId id of target node
	*/
	void SendDirectMessageToNode(CogMsg& msg, int targetId);

	/**
	* Returns true, if there is at least one behavior listener for selected action
	*/
	bool IsRegisteredListener(int action) const{
		return behListeners.find(action) != behListeners.end();
	}

	/**
	* Returns true, if behavior is listening for selected action
	*/
	bool IsRegisteredListener(int action, CogBehavior* beh){
		if (behListenerActions.find(beh->GetId()) == behListenerActions.end()) return false;

		vector<int>& actions = behListenerActions[beh->GetId()];

		return (std::find(actions.begin(), actions.end(), action) != actions.end());
	}

	/**
	* Finds node by id
	*/
	CogNode* FindNodeById(int id) const;

	/**
	* Finds behavior by id
	*/
	CogBehavior* FindBehaviorById(int id) const;

	/**
	* Gets number of nodes with specific tag
	*/
	int GetNodesCountByTag(string tag) const;

	/**
	* Finds node by tag
	*/
	CogNode* FindNodeByTag(string tag) const;

	/**
	* Finds all nodes by tag
	*/
	vector<CogNode*> FindNodesByTag(char* tag) const;

	/**
	* Gets number of nodes with specific subtype
	*/
	int GetNodesCountBySubType(int subtype) const;

	/**
	* Finds node by subtype
	*/
	CogNode* FindNodeBySubType(int subtype) const;

	/**
	* Finds all nodes by subtype
	*/
	vector<CogNode*> FindNodesBySubType(int subtype) const;

	/**
	* Adds a new node to the collection
	* @return true, if node has been added
	*/
	bool AddNode(CogNode* node){
		auto found = find(allNodes.begin(), allNodes.end(), node);
		if (found == allNodes.end()){
			allNodes.push_back(node);
			return true;
		}
		else return false;
	}

	/**
	* Removes node from collection
	*/
	void RemoveNode(CogNode* node){
		auto found = find(allNodes.begin(), allNodes.end(), node);
		if (found != allNodes.end()) allNodes.erase(found);
	}

	/**
	* Adds a new behavior
	* @return true if behavior has been added
	*/
	bool AddBehavior(CogBehavior* beh){
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
	void RemoveBehavior(CogBehavior* beh){
		auto found = find(allBehaviors.begin(), allBehaviors.end(), beh);
		if (found != allBehaviors.end()) allBehaviors.erase(found);

		UnregisterListener(beh);
	}

	private:

		/**
		* Sends message to behaviors
		* @param msg message to send
		* @param actualNode actual node in bubbling algorithm
		*/
		void SendMessageToBehaviors(CogMsg& msg, CogNode* actualNode);

		/**MM
		* Sends message to children of actual node
		* @param msg message to send
		* @param actualNode actual node in bubbling algorithm
		*/
		void SendBubblingMessageToChildren(CogMsg& msg, CogNode* actualNode);

		/**
		* Sends message that will be bubbled from actualNode
		* @param msg message to send
		* @param actualNode actual node in bubbling algorithm
		*/
		void SendBubblingMessage(CogMsg& msg, CogNode* actualNode);

		/**
		* Sends direct message - takes all behaviors and callbacks that have
		* registered selected action
		* @param msg message  to send
		*/
		void SendDirectMessage(CogMsg& msg);

};
