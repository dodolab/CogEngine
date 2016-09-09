
#ifndef GNODEPOOL_H
#define GNODEPOOL_H

#include <map>
#include "GNode.h"
#include "ABehavior.h"
#include <vector>

class GNodePool{
private:
	static GNodePool* _instance;
	map<int, int> behListeners;
	map<int, int> typeCounter;
	vector<GNode*> allGameObjects;
	vector<ABehavior*> allBehaviors;

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
	vector<GNode*> FindGameObjectsByTag(char* tag);
	GNode* FindGameObjectBySubType(int subtype);
	vector<GNode*> FindGameObjectsBySubType(int subtype);

	void OnMessage(Msg msg);
};

#endif
