#pragma once

 class ofxAreEngine;
extern ofxAreEngine COGEngine;

#include "ofxArcEnvironmentCtrl.h"
#include "ofxArcStorage.h"
#include "ofxAreLogger.h"
#include "ofxAreUtils.h"
#include "ofxAreNode.h"
#include "ofxArcFactory.h"
#include "ofxArcRepository.h"
#include "ofxArbTween.h"
#include "ofxArbSceneManager.h"
#include "ofxArcRenderer.h"

/**
* COG engine that holds references to all other components and
* executes drawing and update loops
*/
class ofxAreEngine{
private:
	// root node, created by factory
	ofxAreNode* _root;
	// frame counter
	int frameCounter;
public:
	MofxArcEnvironmentCtrl* environmentCtrl;
	ofxArcStorage* resourceCtrl;
	ofxArcFactory* factory;
	ofxArcRepository* storage;
	ofxAreLogger* logger;
	ofxArcRenderer* renderer;

	ofxAreEngine(){
		_root = nullptr;
		environmentCtrl = nullptr;
		resourceCtrl = nullptr;
		factory = nullptr;
		storage = nullptr;
		logger = nullptr;
		renderer = nullptr;
		frameCounter = 0;
	}

	~ofxAreEngine(){
		//delete _root;
		delete environmentCtrl;
		delete resourceCtrl;
		delete factory;
		delete storage;
		delete logger;
		delete renderer;
	}

	/**
	* Initializes engine
	* @param factory default COG factory
	* @param config configuration xml
	*/
	void Init(ofxArcFactory* factory, spt<ofxXmlSettings> config);
	
	/**
	* Executes one update cycle; this method is called by ofxAreApp
	* @param delta time ofxArbTween frames
	* @param absolute time elapsed after initialization
	*/
	void Update(uint64 delta, uint64 absolute);

	/**
	* Executes one drawing cycle; this method is called by ofxAreApp
	* @param delta time ofxArbTween frames
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

