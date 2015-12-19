#include "Scene.h"
#include "CogEngine.h"

namespace Cog {

	void Scene::LoadFromXml(spt<ofxXml> xml) {

		auto cache = GETCOMPONENT(ResourceCache);

		SetName(xml->getAttribute(":", "name", ""));

		sceneNode = new Node(ObjType::SCENE, 0, name);

		if (xml->tagExists("scene_settings")) {
			xml->pushTag("scene_settings");

			auto map = cache->LoadSettingsFromXml(xml);
			settings.MergeSettings(map);

			xml->popTag();
		}

		if (xml->tagExists("sceneanim")) {
			// parse animation

		}

		int nodes = xml->getNumTags("node");
		NodeBuilder bld = NodeBuilder();

		for (int i = 0; i < nodes; i++) {
			xml->pushTag("node", i);
			// load nodes
			Node* node = bld.LoadNodeFromXml(xml, sceneNode, settings);
			sceneNode->AddChild(node);

			xml->popTag();
		}

		// submit changes to the root node
		sceneNode->SubmitChanges(true);
	}

}// namespace