#pragma once

 class MEngine;
extern MEngine COGEngine;

#include "MEnvironmentCtrl.h"
#include "MStorage.h"
#include "MLogger.h"
#include "MUtils.h"
#include "GNode.h"
#include "MFactory.h"
#include "MRepository.h"
#include "BeTween.h"
#include "BeSceneManager.h"

/**
* COG engine that holds references to all other components and
* executes drawing and update loops
*/
class MEngine{
private:
	// root node, created by factory
	GNode* _root;
	// frame counter
	int frameCounter;
public:
	MEnvironmentCtrl* environmentCtrl;
	MStorage* resourceCtrl;
	MFactory* factory;
	MRepository* storage;
	MLogger* logger;

	MEngine(){
		_root = nullptr;
		environmentCtrl = nullptr;
		resourceCtrl = nullptr;
		factory = nullptr;
		storage = nullptr;
		logger = nullptr;
		frameCounter = 0;
	}

	~MEngine(){
		//delete _root;
		delete environmentCtrl;
		delete resourceCtrl;
		delete factory;
		delete storage;
		delete logger;
	}

	/**
	* Initializes engine
	* @param factory default COG factory
	*/
	void Init(MFactory* factory);
	
	/**
	* Executes one update cycle; this method is called by MApp
	* @param delta time between frames
	* @param absolute time elapsed after initialization
	*/
	void Update(uint64 delta, uint64 absolute);

	/**
	* Executes one drawing cycle; this method is called by MApp
	* @param delta time between frames
	* @param absolute time elapsed after initialization
	*/
	void Draw(uint64 delta, uint64 absolute);

	/**
	* Gets actual frame number
	*/
	int GetFrameCounter(){
		return frameCounter;
	}
};

