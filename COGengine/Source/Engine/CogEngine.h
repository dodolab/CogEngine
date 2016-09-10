#pragma once


#include "ResourceCache.h"
#include "Logger.h"
#include "Node.h"
#include "Stage.h"
#include "Tween.h"
#include "SceneSwitchManager.h"
#include "Renderer.h"
#include "Environment.h"
#include "EntityStorage.h"
#include "InputHandler.h"

namespace Cog {

	/**
	* COG engine that holds references to all other components and
	* executes drawing and update loops
	*/
	class CogEngine {
	private:
		// frame counter
		int frameCounter = 0;

		void RegisterComponents();
		vector<function<void()>> actions;

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
		Stage* stage = nullptr;
		InputHandler* inputHandler = nullptr;
		
		CogEngine() {
			entityStorage = new EntityStorage();
		}

		~CogEngine() {
			delete environment;
			delete resourceCache;
			delete stage;
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
		* Initializes engine from XML file
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

		/**
		* Adds a new action that is executed when the update process is finished
		*/
		void AddPostUpdateAction(function<void()> action);

		// ================================= SINGLETON PART ==========================
	public:
		static CogEngine& GetInstance()
		{
			// guaranteed to be destroyed,
			// instantiated on first use
			static CogEngine instance;
			return instance;
		}

		// deleted functions should be public as it results
		// int better error messages due compiling
		CogEngine(CogEngine const&) = delete;
		void operator=(CogEngine const&) = delete;
	};


}// namespace