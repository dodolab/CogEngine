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
	// behavior listeners
	map<int, int> behListeners;
	// types counter
	map<int, int> typeCounter;
	// list of all game objects
	list<GNode*> allGameObjects;
	// list of all behaviors
	list<GBehavior*> allBehaviors;

public:

	void Init(){

	}
	void Terminate(){

	}

	MGameStorage(){

	}


	bool IsRegisteredListener(int key) const{
		return behListeners.find(key) != behListeners.end();;
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

	void OnMessage(GMsg msg){

	}
};

#endif
