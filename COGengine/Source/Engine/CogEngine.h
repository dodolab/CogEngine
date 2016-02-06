#pragma once

namespace Cog {
	class Engine;
	extern Engine COGEngine;
}


#include "ResourceCache.h"
#include "Logger.h"
#include "Node.h"
#include "SceneContext.h"
#include "Tween.h"
#include "SceneSwitchManager.h"
#include "Renderer.h"
#include "Environment.h"
#include "EntityStorage.h"
#include "InputHandler.h"

namespace Cog {

	class Engine;
	extern Engine COGEngine;

	/**
	* COG engine that holds references to all other components and
	* executes drawing and update loops
	*/
	class Engine {
	private:
		// frame counter
		int frameCounter = 0;

		void RegisterComponents();

	public:
		// global configuration file
		spt<ofxXml> config;

		// component storage
		EntityStorage* entityStorage = nullptr;

		// main components that are simply accessible from this object
		Environment* environment = nullptr;
		ResourceCache* resourceCache = nullptr;
		Logger* logger = nullptr;
		Renderer* renderer = nullptr;
		SceneContext* sceneContext = nullptr;
		InputHandler* inputHandler = nullptr;
		
		Engine() {
			entityStorage = new EntityStorage();
		}

		~Engine() {
			delete environment;
			delete resourceCache;
			delete sceneContext;
			delete logger;
			delete renderer;
			delete inputHandler;
			delete entityStorage;
		}

		/**
		* Initializes engine
		*/
		void Init();


		/**
		* Initializes engine
		* @param config configuration xml
		*/
		void Init(spt<ofxXmlSettings> config);

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


}// namespace