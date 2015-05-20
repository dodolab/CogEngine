#ifndef GAMEENGINE_H
#define GAMEENGINE_H


 class MGameEngine;
extern MGameEngine MEngine;



#include "MEnvironmentCtrl.h"
#include "MGameCtrl.h"
#include "MResourceCtrl.h"
#include "s3eTypes.h"
#include "GNode.h"
#include "IwResGroup.h"



class MGameEngine{
private:

	uint64 absolute = 0;


	void Update(uint64 delta, uint64 absolute);
	void Draw(uint64 delta, uint64 absolute);

public:
	MEnvironmentCtrl* environmentCtrl = nullptr;
	MGameCtrl* gameCtrl = nullptr;
	MResourceCtrl* resourceCtrl = nullptr;


	~MGameEngine(){
		delete environmentCtrl;
		delete gameCtrl;
		delete resourceCtrl;
	}

	void Init();
	void StartLoop();
	void Terminate();
};


#endif
