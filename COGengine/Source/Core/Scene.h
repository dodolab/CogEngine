#pragma once

#include "ofxCogMain.h"
#include "ResourceCache.h"
#include "Node.h"
#include "Component.h"
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
		void LoadFromXml(spt<ofxXml> xml);

	};

} // namespace
