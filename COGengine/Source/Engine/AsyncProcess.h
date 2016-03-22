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

			for (int i = 0; i < config->getNumTags("scene"); i++) {
				config->pushTag("scene", i);

				string name = config->getAttributex("name", "");
				if (name.compare(scene->GetName()) == 0) {
					scene->LoadFromXml(config);
					config->popTag();
					break;
				}

				config->popTag();
			}

			config->popAll();

			// switch to scene
			auto stage = GETCOMPONENT(Stage);
			scene->GetSceneNode()->SubmitChanges(true);
			stage->SwitchToScene(scene, tweenDir);
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
			COGLOGDEBUG("AsyncProcess", "Running threaded job %s", this->job->GetClassName().c_str());
			job->DoJob();
			COGLOGDEBUG("AsyncProcess", "Finishing threaded job %s", this->job->GetClassName().c_str());
			delete job;
			
		}

	};



} // namespace