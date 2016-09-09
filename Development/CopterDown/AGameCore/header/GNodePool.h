
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
	int GetGameObjCountBySubType(int type) const;
	void RemoveGameObject(GNode* node);
	void AddBehavior(ABehavior* beh);
	void RemoveBehavior(ABehavior* beh);
	bool IsRegisteredListener(int listenerType) const;
	GNode* FindGameObjectById(int id) const;
	GNode* FindGameObjectByTag(char* tag) const;
	CIwArray<GNode*> FindGameObjectsByTag(char* tag) const;
	GNode* FindGameObjectBySubType(int subtype) const;
	CIwArray<GNode*> FindGameObjectsBySubType(int subtype) const;

	void OnMessage(Msg msg);
};

#endif
