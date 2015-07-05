#ifndef GAMEENGINE_H
#define GAMEENGINE_H


 class MGameEngine;
extern MGameEngine MEngine;


#include "MEnvironmentCtrl.h"
#include "MResourceCtrl.h"
#include "GNode.h"
#include "MGameFactory.h"
#include "MGameStorage.h"



class MGameEngine{
private:

	GNode* _root;
public:
	MGameEngine(){
		_root = nullptr;
		environmentCtrl = nullptr;
		resourceCtrl = nullptr;
		factory = nullptr;
		storage = nullptr;
	}

	void Update(uint64 delta, uint64 absolute);
	void Draw(uint64 delta, uint64 absolute);

	MEnvironmentCtrl* environmentCtrl;
	MResourceCtrl* resourceCtrl;
	MGameFactory* factory;
	MGameStorage* storage;

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

	// help functions
	ofVec2f GetSize(){
		return ofVec2f(environmentCtrl->GetWidth(), environmentCtrl->GetHeight());
	}

	float TranslateSpeed(float speed){
		// speed = 1 translates over the whole width per 10 seconds
		return speed * 0.001f*environmentCtrl->GetWidth();
	}
};


#endif