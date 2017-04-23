#include "AsyncProcess.h"
#include "Scene.h"
#include "Stage.h"
#include "ComponentStorage.h"

namespace Cog {

	SceneLoader::SceneLoader(Scene* scene, TweenDirection tweenDir)
		: scene(scene), tweenDir(tweenDir) {

	}

	void SceneLoader::DoJob() {
		
		spt<xml_document> scenes = CogPreloadXMLFile(ofToDataPath(PATH_SCENES));
		auto scenesNode = scenes->children("scene");

		for (auto sceneNode : scenesNode) {
			// just load scene from XML and switch to it
			string name = sceneNode.attribute("name").as_string();
			if (name.compare(scene->GetName()) == 0) {
				scene->LoadFromXml(sceneNode);
				break;
			}
		}

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