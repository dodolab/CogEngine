#pragma once

#include <map>
#include "GNode.h"
#include "GBehavior.h"
#include <functional>

/**
* Node repository
*/
class MRepository{
private:
	// behavior listeners
	map<int, vector<GBehavior*>> behListeners;
	// behavior ids and their registered actions
	map<int, vector<int>> behListenerActions;
	// list of all nodes
	vector<GNode*> allNodes;
	// list of all behaviors
	vector<GBehavior*> allBehaviors;

public:


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
	* Sends message to specific node
	* @param msg message  to send
	* @param targetId id of target node
	*/
	void SendDirectMessageToNode(GMsg& msg, int targetId);

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
	* Finds node by id
	*/
	GNode* FindNodeById(int id) const;

	/**
	* Finds behavior by id
	*/
	GBehavior* FindBehaviorById(int id) const;

	/**
	* Gets number of nodes with specific tag
	*/
	int GetNodesCountByTag(string tag) const;

	/**
	* Finds node by tag
	*/
	GNode* FindNodeByTag(string tag) const;

	/**
	* Finds all nodes by tag
	*/
	vector<GNode*> FindNodesByTag(char* tag) const;

	/**
	* Gets number of nodes with specific subtype
	*/
	int GetNodesCountBySubType(int subtype) const;

	/**
	* Finds node by subtype
	*/
	GNode* FindNodeBySubType(int subtype) const;

	/**
	* Finds all nodes by subtype
	*/
	vector<GNode*> FindNodesBySubType(int subtype) const;

	/**
	* Adds a new node to the collection
	* @return true, if node has been added
	*/
	bool AddNode(GNode* node){
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
	void RemoveNode(GNode* node){
		auto found = find(allNodes.begin(), allNodes.end(), node);
		if (found != allNodes.end()) allNodes.erase(found);
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

	private:

		/**
		* Sends message to behaviors
		* @param msg message to send
		* @param actualNode actual node in bubbling algorithm
		*/
		void SendMessageToBehaviors(GMsg& msg, GNode* actualNode);

		/**MM
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

};
