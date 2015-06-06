#ifndef GAMEENGINE_H
#define GAMEENGINE_H


 class MGameEngine;
extern MGameEngine MEngine;



#include "MEnvironmentCtrl.h"
#include "MResourceCtrl.h"
#include "s3eTypes.h"
#include "GNode.h"
#include "IwResGroup.h"
#include "MGameFactory.h"
#include "MGameStorage.h"



class MGameEngine{
private:

	uint64 absolute = 0;
	GNode* _root;

	void Update(uint64 delta, uint64 absolute);
	void Draw(uint64 delta, uint64 absolute);

public:
	MEnvironmentCtrl* environmentCtrl = nullptr;
	MResourceCtrl* resourceCtrl = nullptr;
	MGameFactory* factory = nullptr;
	MGameStorage* storage = nullptr;

	~MGameEngine(){
		delete _root;
		delete environmentCtrl;
		delete resourceCtrl;
		delete factory;
		delete storage;
	}

	void Init(MGameFactory* factory, MGameStorage* storage);
	void StartLoop();
	void Terminate();
};


#endif
