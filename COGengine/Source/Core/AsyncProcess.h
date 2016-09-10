#pragma once

#include "ofxCogMain.h"
#include "Scene.h"
#include "Behavior.h"
#include "Node.h"

namespace Cog {

	class Job {
	public:
		virtual void DoJob() = 0;
	};

	class SceneLoader : public Job {
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
			MLOGDEBUG("AsyncProcess", "Running threaded job");
			job->DoJob();
			delete job;
			MLOGDEBUG("SceneManager", "Finishing threaded job");
		}

	};



} // namespace