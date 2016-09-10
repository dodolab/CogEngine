#include "CogEngine.h"
#include "Node.h"
#include "Factory.h"
#include "ResourceCache.h"
#include "ofSoundPlayer.h"
#include "Animator.h"
#include "Button.h"
#include "Collider.h"
#include "DelayAction.h"
#include "DelayRemove.h"
#include "HitEvent.h"
#include "InputKey.h"
#include "Move.h"
#include "MultiAnim.h"
#include "RotateAnim.h"
#include "SceneManager.h"
#include "TransformAnim.h"
#include "TranslateAnim.h"
#include "Tween.h"


namespace Cog {

	Engine COGEngine;

	
	void Engine::Init() {
		RegisterComponents();

		vector<Component*> components = entityStorage->GetAllComponents();

		for (auto it = components.begin(); it != components.end(); ++it) {
			(*it)->Init();
		}
	}

	void Engine::Init(spt<ofxXmlSettings> config) {
		RegisterComponents();
		vector<Component*> components = entityStorage->GetAllComponents();

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
		nodeStorage = new NodeContext();
		renderer = new Renderer();
		
		REGISTER_COMPONENT(environment);
		REGISTER_COMPONENT(resourceCache);
		REGISTER_COMPONENT(logger);
		REGISTER_COMPONENT(nodeStorage);
		REGISTER_COMPONENT(renderer);

		REGISTER_BEHAVIOR(Animator);
		REGISTER_BEHAVIOR(Button);
		REGISTER_BEHAVIOR(Collider);
		REGISTER_BEHAVIOR(DelayAction);
		REGISTER_BEHAVIOR(DelayRemove);
		REGISTER_BEHAVIOR(HitEvent);
		REGISTER_BEHAVIOR(InputKey);
		REGISTER_BEHAVIOR(Move);
		REGISTER_BEHAVIOR(MultiAnim);
		REGISTER_BEHAVIOR(RotateAnim);
		REGISTER_BEHAVIOR(SceneManager);
		REGISTER_BEHAVIOR(TransformAnim);
		REGISTER_BEHAVIOR(TranslateAnim);
		REGISTER_BEHAVIOR(SlideTween);

	}

}// namespace