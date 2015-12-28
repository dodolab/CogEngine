#pragma once

#include "ofxCogCommon.h"
#include "Scene.h"
#include "Behavior.h"
#include "Node.h"

namespace Cog {

	/**
	* Abstract class for all jobs
	*/
	class Job {
		OBJECT(Job)
	public:
		virtual void DoJob() = 0;
	};

	/**
	* Job that runs scene loading from XML
	*/
	class SceneLoader : public Job {
		OBJECT(SceneLoader)
	private:
		spt<ofxXml> config;
		Scene* scene;
		TweenDirection tweenDir;
	public:

		SceneLoader(spt<ofxXml> config, Scene* scene, TweenDirection tweenDir) : config(config), scene(scene), tweenDir(tweenDir) {

		}

		void DoJob() {
			config->popAll();
			config->pushTag("app_config");
			config->pushTag("scenes");
			config->pushTag("scene", scene->GetIndex());

			scene->LoadFromXml(config);
			config->popAll();

			// switch to scene
			auto context = GETCOMPONENT(SceneContext);
			scene->GetSceneNode()->SubmitChanges(true);
			context->SwitchToScene(scene, tweenDir);
		}
	};

	/**
	* Asynchronous process
	*/
	class AsyncProcess : public ofThread {
	private:
		Job* job;
	public:

		AsyncProcess(Job* job) {
			this->job = job;
		}

		void threadedFunction()
		{
			MLOGDEBUG("AsyncProcess", "Running threaded job %s", this->job->GetClassName().c_str());
			job->DoJob();
			MLOGDEBUG("AsyncProcess", "Finishing threaded job %s", this->job->GetClassName().c_str());
			delete job;
			
		}

	};



} // namespace