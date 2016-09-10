#pragma once

#include "ofxCogMain.h"
#include "ResourceCache.h"
#include "Node.h"
#include "Component.h"
#include "Engine.h"
#include "NodeBuilder.h"

namespace Cog
{
	/**
	* Scene entity, containing node tree
	*/
	class Scene
	{
	private:
		string name;
		// settings
		Settings settings;
		// node on top
		Node* sceneNode;

	public:

		Node* GetSceneNode() {
			return sceneNode;
		}

		string GetName() {
			return name;
		}

		void SetName(string name) {
			this->name = name;
		}

		/**
		* Loads scene from xml
		*/
		void LoadFromXml(spt<ofxXml> xml) {

			auto cache = GETCOMPONENT(ResourceCache);

			SetName(xml->getAttribute(":", "name", ""));

			sceneNode = new Node(ObjType::SCENE, 0, name);

			if (xml->tagExists("scene_settings")) {
				xml->pushTag("scene_settings");

				settings.MergeSettings(cache->LoadSettingsFromXml(xml));

				xml->popTag();
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

	};

} // namespace
