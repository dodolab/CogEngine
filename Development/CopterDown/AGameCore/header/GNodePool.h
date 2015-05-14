
#ifndef GNODEPOOL_H
#define GNODEPOOL_H

#include <map>
#include "GNode.h"
#include "ABehavior.h"


class GNodePool{
private:
	static GNodePool* _instance;
	map<int, int> behListeners;
	map<int, int> typeCounter;
	vector<GNode*> allGameObjects;
	vector<ABehavior*> allBehaviors;

public:
	GNodePool();

	static GNodePool* Get(){
		return nullptr;
	}

	int GetGameObjCountBySubType(int type) const{
		return 0;
	}

	void AllocateNode(GNode* node){

	}

	void FreeNode(GNode* node){

	}

	void AllocateBehavior(ABehavior* beh){

	}

	void FreeBehavior(ABehavior* beh){

	}

	bool IsRegisteredListener(int listenerType) const{
		return true;
	}
	GNode* FindGameObjectById(int id) const{
		return nullptr;
	}
	GNode* FindGameObjectByTag(char* tag) const{
		return nullptr;
	}
	vector<GNode*> FindGameObjectsByTag(char* tag) const{
		return vector<GNode*>();
	}
	GNode* FindGameObjectBySubType(int subtype) const{
		return nullptr;
	}
	vector<GNode*> FindGameObjectsBySubType(int subtype) const{
		return vector<GNode*>();
	}

	void OnMessage(Msg msg){

	}
};

#endif
