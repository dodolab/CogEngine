#include "AsyncProcess.h"
#include "Scene.h"
#include "Stage.h"
#include "ComponentStorage.h"

namespace Cog {

	SceneLoader::SceneLoader(Scene* scene, TweenDirection tweenDir)
		: scene(scene), tweenDir(tweenDir) {

	}

	void SceneLoader::DoJob() {
		
		spt<ofxXml> scenes = spt<ofxXml>(new ofxXml());
		scenes->loadFile(ofToDataPath(PATH_SCENES));

		scenes->popAll();
		scenes->pushTag("resources");

		for (int i = 0; i < scenes->getNumTags("scene"); i++) {
			scenes->pushTag("scene", i);

			// just load scene from XML and switch to it
			string name = scenes->getAttributex("name", "");
			if (name.compare(scene->GetName()) == 0) {
				scene->LoadFromXml(scenes);
				scenes->popTag();
				break;
			}

			scenes->popTag();
		}

		scenes->popAll();

		// switch to scene
		auto stage = GETCOMPONENT(Stage);
		scene->GetSceneNode()->SubmitChanges(true);
		stage->SwitchToScene(scene, tweenDir);
	}

	void AsyncProcess::threadedFunction()
	{
		COGLOGDEBUG("AsyncProcess", "Running threaded job %s", typeid(this->job).name());
		job->DoJob();
		COGLOGDEBUG("AsyncProcess", "Finishing threaded job %s", typeid(this->job).name());
		delete job;
	}
} // namespace