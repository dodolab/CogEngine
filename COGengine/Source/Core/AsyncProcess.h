#pragma once

#include "Definitions.h"
#include "ofxXmlSettings.h"
#include "ofThread.h"

namespace Cog {

	enum class TweenDirection;
	class Scene;

	/**
	* Abstract class for asynchronous processes
	*/
	class Job {

	public:
		virtual void DoJob() = 0;
	};

	/**
	* Job that asynchronously loads a scene from XML
	*/
	class SceneLoader : public Job {

	private:
		Scene* scene;
		TweenDirection tweenDir;
	public:

		SceneLoader(Scene* scene, TweenDirection tweenDir);

		void DoJob();
	};

	/**
	* Thread that runs a job asynchronously
	* Note that some structures couldn't be loaded asynchronously (fonts, for instance)
	*/
	class AsyncProcess : public ofThread {
	private:
		Job* job;
	public:

		AsyncProcess(Job* job) {
			this->job = job;
		}

		void threadedFunction();
	};



} // namespace