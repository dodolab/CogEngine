#pragma once

#include "Definitions.h"
#include <vector>
#include "ofxXmlSettings.h"

namespace Cog {

  //         __       _____            ______             _            
  //        / _|     / ____|          |  ____|           (_)           
  //   ___ | |___  _| |     ___   __ _| |__   _ __   __ _ _ _ __   ___ 
  //  / _ \|  _\ \/ / |    / _ \ / _` |  __| | '_ \ / _` | | '_ \ / _ \
  // | (_) | |  >  <| |___| (_) | (_| | |____| | | | (_| | | | | |  __/
  //  \___/|_| /_/\_\\_____\___/ \__, |______|_| |_|\__, |_|_| |_|\___|
  //                              __/ |              __/ |             
  //                             |___/              |___/              
	class ComponentStorage;
	class Environment;
	class Logger;
	class Resources;
	class Renderer;
	class InputHandler;
	class Stage;

	/**
	* COG engine that holds references to important components and
	* executes drawing and update loops
	*/
	class ofxCogEngine {
	private:
		// frame counter
		int frameCounter = 0;
		int fps = 0;
		// actions that should be executed after update loop
		vector<function<void()>> actions;
		uint64 lastAbsoluteTime = 0;

		/**
		* Registers important components
		*/
		void RegisterComponents();
	
	public:
		// global configuration file
		spt<ofxXml> config;

		// component storage
		ComponentStorage* compStorage;

		// important components that are simply accessible from this object
		Environment* environment = nullptr;
		Resources* resources = nullptr;
		Logger* logger = nullptr;
		Renderer* renderer = nullptr;
		Stage* stage = nullptr;
		InputHandler* inputHandler = nullptr;
		
		ofxCogEngine();

		~ofxCogEngine() {
			Clear();
		}

		/**
		* Initializes engine
		*/
		void Init();

		/**
		* Initializes engine from XML file
		* @param xmlPath path to configuration file
		*/
		void Init(string xmlPath);

		/**
		* Initializes engine from XML file
		* @param config configuration xml
		*/
		void Init(spt<ofxXml> config);

		/**
		* Loads stage from xml
		*/
		void LoadStageFromXml(spt<ofxXml> config);

		/**
		* Executes one update cycle; this method is called by ofxCogApp
		* @param delta time between frames
		* @param absolute time elapsed since initialization
		*/
		void Update(uint64 delta, uint64 absolute);

		/**
		* Executes one drawing cycle; this method is called by ofxCogApp
		* @param delta time between frames
		* @param absolute time elapsed since initialization
		*/
		void Draw(uint64 delta, uint64 absolute);

		/**
		* Gets actual frame number
		*/
		int GetFrameCounter() const {
			return frameCounter;
		}

		/**
		* Resets frame counter
		*/
		void ResetFrameCounter() {
			this->frameCounter = 0;
		}

		/**
		* Gets time elapsed since the initialization
		*/
		uint64 GetAbsoluteTime() const {
			return this->lastAbsoluteTime;
		}

		/**
		* Gets actual framerate
		*/
		int GetFps();

		/**
		* Sets the framerate
		*/
		void SetFps(int fps);

		/**
		* Adds a new action that is executed after the update process is finished
		*/
		void AddPostUpdateAction(function<void()> action);

	private:
		// clears resources
		void Clear();

		/**
		* Checks if assets of this library are presented
		*/
		void CheckCOGAssets();

		/**
		* Loads assets of this library
		*/
		void LoadCOGAssets();

		// ================================= SINGLETON PART ==========================
	public:
		static ofxCogEngine& GetInstance()
		{
			// guaranteed to be destroyed,
			// instantiated on first use
			static ofxCogEngine instance;
			return instance;
		}

		// deleted functions should be public as it results
		// int better error messages due compiling
		ofxCogEngine(ofxCogEngine const&) = delete;
		void operator=(ofxCogEngine const&) = delete;
	};


}// namespace