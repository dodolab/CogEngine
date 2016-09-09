#pragma once

namespace Cog {
	class Engine;
	extern Engine COGEngine;
}


#include "Cache.h"
#include "Storage.h"
#include "Logger.h"
#include "Utils.h"
#include "Node.h"
#include "Factory.h"
#include "Storage.h"
#include "Tween.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Environment.h"


namespace Cog {

	class Engine;
	extern Engine COGEngine;

	/**
	* COG engine that holds references to all other components and
	* executes drawing and update loops
	*/
	class Engine {
	private:
		// root node, created by factory
		Node* _root;
		// frame counter
		int frameCounter;
	public:
		Environment* environmentCtrl;
		Cache* resourceCtrl;
		Factory* factory;
		Repository* storage;
		Logger* logger;
		Renderer* renderer;

		Engine() {
			_root = nullptr;
			environmentCtrl = nullptr;
			resourceCtrl = nullptr;
			factory = nullptr;
			storage = nullptr;
			logger = nullptr;
			renderer = nullptr;
			frameCounter = 0;
		}

		~Engine() {
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
		void Init(Factory* factory, spt<ofxXmlSettings> config);

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