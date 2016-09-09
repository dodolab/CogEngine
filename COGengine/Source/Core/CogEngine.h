#pragma once

namespace Cog {
	class CogEngine;
	extern CogEngine COGEngine;
}


#include "CogCache.h"
#include "CogStorage.h"
#include "CogLogger.h"
#include "CogUtils.h"
#include "CogNode.h"
#include "CogFactory.h"
#include "CogStorage.h"
#include "CogTween.h"
#include "CogSceneManager.h"
#include "CogRenderer.h"
#include "CogEnvironment.h"


namespace Cog {

	class CogEngine;
	extern CogEngine COGEngine;

	/**
	* COG engine that holds references to all other components and
	* executes drawing and update loops
	*/
	class CogEngine {
	private:
		// root node, created by factory
		CogNode* _root;
		// frame counter
		int frameCounter;
	public:
		CogEnvironment* environmentCtrl;
		CogCache* resourceCtrl;
		CogFactory* factory;
		CogRepository* storage;
		CogLogger* logger;
		CogRenderer* renderer;

		CogEngine() {
			_root = nullptr;
			environmentCtrl = nullptr;
			resourceCtrl = nullptr;
			factory = nullptr;
			storage = nullptr;
			logger = nullptr;
			renderer = nullptr;
			frameCounter = 0;
		}

		~CogEngine() {
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
		void Init(CogFactory* factory, spt<ofxXmlSettings> config);

		/**
		* Executes one update cycle; this method is called by App
		* @param delta time Tween frames
		* @param absolute time elapsed after initialization
		*/
		void Update(uint64 delta, uint64 absolute);

		/**
		* Executes one drawing cycle; this method is called by App
		* @param delta time Tween frames
		* @param absolute time elapsed after initialization
		*/
		void Draw(uint64 delta, uint64 absolute);

		/**
		* Gets actual frame number
		*/
		int GetFrameCounter() {
			return frameCounter;
		}
	};

}