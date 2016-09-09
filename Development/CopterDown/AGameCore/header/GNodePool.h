
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
	CIwArray<GNode*> allGameObjects;
	CIwArray<ABehavior*> allBehaviors;

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
	CIwArray<GNode*> FindGameObjectsByTag(char* tag) const{
		return CIwArray<GNode*>();
	}
	GNode* FindGameObjectBySubType(int subtype) const{
		return nullptr;
	}
	CIwArray<GNode*> FindGameObjectsBySubType(int subtype) const{
		return CIwArray<GNode*>();
	}

	void OnMessage(Msg msg){

	}
};

#endif
