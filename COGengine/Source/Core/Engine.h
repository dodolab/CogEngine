#pragma once

namespace Cog {
	class Engine;
	extern Engine COGEngine;
}


#include "ResourceCache.h"
#include "NodeStorage.h"
#include "Logger.h"
#include "Utils.h"
#include "Node.h"
#include "Factory.h"
#include "NodeStorage.h"
#include "Tween.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Environment.h"
#include "ComponentStorage.h"

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
		// component storage
		ComponentStorage* componentStorage = nullptr;

		// main components that are simply accessible from this object
		Environment* environment = nullptr;
		ResourceCache* resourceCache = nullptr;
		Logger* logger = nullptr;
		Renderer* renderer = nullptr;
		NodeStorage* nodeStorage = nullptr;
		
		Engine() {
			componentStorage = new ComponentStorage();
		}

		~Engine() {
			delete environment;
			delete resourceCache;
			delete nodeStorage;
			delete logger;
			delete renderer;
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

#define GETCOMPONENT(className) COGEngine.componentStorage->GetComponent<className>(#className)

}// namespace