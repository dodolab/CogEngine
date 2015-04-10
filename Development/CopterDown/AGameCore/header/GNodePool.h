
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

	static GNodePool* Get();

	void AddGameObject(GNode* node);
	int GetGameObjCountBySubType(int type);
	void RemoveGameObject(GNode* node);
	void AddBehavior(ABehavior* beh);
	void RemoveBehavior(ABehavior* beh);
	bool IsRegisteredListener(int listenerType);
	GNode* FindGameObjectById(int id);
	GNode* FindGameObjectByTag(char* tag);
	CIwArray<GNode*> FindGameObjectsByTag(char* tag);
	GNode* FindGameObjectBySubType(int subtype);
	CIwArray<GNode*> FindGameObjectsBySubType(int subtype);

	void OnMessage(Msg msg);
};

#endif
