#include "Engine.h"
#include "TranslateAnim.h"
#include "Node.h"
#include "RotateAnim.h"
#include "Factory.h"
#include "ResourceCache.h"
#include "ofSoundPlayer.h"

namespace Cog {

	Engine COGEngine;

	
	void Engine::Init() {
		RegisterComponents();

		vector<Component*> components = componentStorage->GetAllComponents();

		for (auto it = components.begin(); it != components.end(); ++it) {
			(*it)->Init();
		}
	}

	void Engine::Init(spt<ofxXmlSettings> config) {
		RegisterComponents();
		vector<Component*> components = componentStorage->GetAllComponents();

		for (auto it = components.begin(); it != components.end(); ++it) {
			(*it)->Init(config);
		}
	}


	void Engine::Update(uint64 delta, uint64 absolute) {
		frameCounter++;

		// update transforms
		nodeStorage->GetRootObject()->UpdateTransform(true);
		// update scene
		nodeStorage->GetRootObject()->Update(delta, absolute);
		// remove ended inputs
		environment->RemoveEndedProcesses();

		// flush each 100th frame
		if (frameCounter % 100 == 0) {
			logger->Flush();
		}

		ofSoundUpdate();
	}

	void Engine::Draw(uint64 delta, uint64 absolute) {
		renderer->ClearCounters();
		// clear the drawing surface
		ofBackground(50, 50, 50);
		// setup ortographic camera
		ofSetupScreenOrtho(environment->GetRealWidth(), environment->GetRealHeight(), -1000, 1000);
		
		// draw the root node
		nodeStorage->GetRootObject()->Draw(delta, absolute);

		renderer->Render();
	}

	void Engine::RegisterComponents() {
		environment = new Environment();
		resourceCache = new ResourceCache();
		logger = new Logger();
		nodeStorage = new NodeStorage();
		renderer = new Renderer();
		
		componentStorage->RegisterComponent(environment);
		componentStorage->RegisterComponent(resourceCache);
		componentStorage->RegisterComponent(logger);
		componentStorage->RegisterComponent(nodeStorage);
		componentStorage->RegisterComponent(renderer);
	}

}// namespace