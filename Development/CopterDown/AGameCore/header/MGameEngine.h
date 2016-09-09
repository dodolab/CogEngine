#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "MEnvironmentCtrl.h"
#include "MGameCtrl.h"
#include "MResourceCtrl.h"
#include "s3eTypes.h"
#include "GNode.h"
#include "IwResGroup.h"

class MGameEngine{
private:
	GNode* root;
	uint64 absolute;
	CIwResGroup* resourceGroup;

	MEnvironmentCtrl environmentCtrl;
	MGameCtrl gameCtrl;
	MResourceCtrl resourceCtrl;

	void Update(uint64 delta, uint64 absolute);
	void Draw(uint64 delta, uint64 absolute);

public:
	void Init();
	void StartLoop();
	void Terminate();
};




#endif
