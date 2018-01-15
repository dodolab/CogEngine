#pragma once

#include "Definitions.h"
#include "ofThread.h"

namespace Cog {

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
		stenum tweenDir;
	public:

		SceneLoader(Scene* scene, stenum tweenDir);

		void DoJob();
	};

	/**
	* Thread that runs a job asynchronously
	* Note that some structures couldn't be loaded asynchronously (fonts, for instance)
	*/
	class AsyncProcess : public ofThread {
	protected:
		Job* job;
	public:

		AsyncProcess(Job* job) {
			this->job = job;
		}

		void threadedFunction();
	};



} // namespace