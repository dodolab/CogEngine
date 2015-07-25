#pragma once

 class MGameEngine;
extern MGameEngine MEngine;

#include "MEnvironmentCtrl.h"
#include "MResourceCtrl.h"
#include "MLogger.h"
#include "MUtils.h"
#include "GNode.h"
#include "MGameFactory.h"
#include "MGameStorage.h"
#include "BeTween.h"
#include "BeSceneManager.h"

/**
* Game engine that holds references to all other components and
* executes drawing and update loops
*/
class MGameEngine{
private:
	// root node, created by factory
	GNode* _root;
	// frame counter
	int frameCounter;
public:
	MEnvironmentCtrl* environmentCtrl;
	MResourceCtrl* resourceCtrl;
	MGameFactory* factory;
	MGameStorage* storage;
	MLogger* logger;

	MGameEngine(){
		_root = nullptr;
		environmentCtrl = nullptr;
		resourceCtrl = nullptr;
		factory = nullptr;
		storage = nullptr;
		logger = nullptr;
		frameCounter = 0;
	}

	~MGameEngine(){
		delete _root;
		delete environmentCtrl;
		delete resourceCtrl;
		delete factory;
		delete storage;
		delete logger;
	}

	/**
	* Initializes engine
	* @param factory default game factory
	*/
	void Init(MGameFactory* factory);
	
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

