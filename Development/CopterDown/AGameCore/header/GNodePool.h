
#ifndef GNODEPOOL_H
#define GNODEPOOL_H

#include <map>
#include "GNode.h"
#include "ABehavior.h"

/**
* GNodePool - game object manager
*/
class GNodePool{
private:
	// singleton instance
	static GNodePool* _instance;
	// behavior listeners
	map<int, int> behListeners;
	// types counter
	map<int, int> typeCounter;
	// list of all game objects
	list<GNode*> allGameObjects;
	// list of all behaviors
	list<ABehavior*> allBehaviors;

public:
	GNodePool(){

	}

	static GNodePool* Get(){
		if (_instance == nullptr){
			_instance = new GNodePool();
		}
		return _instance;
	}

	bool IsRegisteredListener(int listenerType) const{
		return true;
	}

	GNode* FindGameObjectById(int id) const{
		for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
			if ((*it)->GetId() == id) return (*it);
		}
		return nullptr;
	}
	GNode* FindGameObjectByTag(string tag) const{
		for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
			if ((*it)->GetTag() == tag) return (*it);
		}
		return nullptr;
	}
	vector<GNode*> FindGameObjectsByTag(char* tag) const{
		vector<GNode*> output;

		for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
			if ((*it)->GetTag() == tag) output.push_back(*it);
		}
		return output;
	}
	GNode* FindGameObjectBySubType(int subtype) const{
		for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
			if ((*it)->GetSubType() == subtype) return (*it);
		}
		return nullptr;
	}
	vector<GNode*> FindGameObjectsBySubType(int subtype) const{
		vector<GNode*> output;

		for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it){
			if ((*it)->GetSubType() == subtype) output.push_back(*it);
		}
		return output;
	}

	void OnMessage(Msg msg){

	}
};

GNodePool* GNodePool::_instance = nullptr;

#endif
