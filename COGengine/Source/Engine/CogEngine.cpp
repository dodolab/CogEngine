#include "CogEngine.h"
#include "Node.h"
#include "ResourceCache.h"
#include "ofSoundPlayer.h"
#include "SheetAnim.h"
#include "Button.h"
#include "Collider.h"
#include "DelayAction.h"
#include "DelayRemove.h"
#include "HitEvent.h"
#include "InputKey.h"
#include "Move.h"
#include "MultiAnim.h"
#include "RotateAnim.h"
#include "SceneSwitchManager.h"
#include "TransformAnim.h"
#include "TranslateAnim.h"
#include "Tween.h"
#include "Component.h"
#include "FloatingScene.h"
#include "InputHandler.h"
#include "Selection.h"
#include "Network.h"

namespace Cog {

	
	void CogEngine::Init() {

		RegisterComponents();

		vector<Component*> components = entityStorage->GetAllComponents();
		// sort by priority
		sort(components.begin(), components.end(),
			[this](const Component* a, const Component* b) -> bool
		{
			return a->GetPriority() > b->GetPriority();
		});

		// init all components
		for (auto it = components.begin(); it != components.end(); ++it) {
			(*it)->Init();
		}
	}

	void CogEngine::Init(spt<ofxXml> config) {
		
		this->config = config;

		RegisterComponents();

		vector<Component*> components = entityStorage->GetAllComponents();
		// sort by priority
		sort(components.begin(), components.end(),
			[this](const Component* a, const Component* b) -> bool
		{
			return a->GetPriority() > b->GetPriority();
		});

		// init logger twice, because it should be always initialized by default at first
		this->logger->Init();

		// init all comopnents, using xml file
		for (auto it = components.begin(); it != components.end(); ++it) {
			(*it)->Init(config);
		}

		// go out
		config->popAll();

		if (config->pushTagIfExists("app_config") && config->pushTagIfExists("scenes")) {
			// load scenes
			auto context = GETCOMPONENT(Stage);
			stage->LoadScenesFromXml(config);
		}
	}


	void CogEngine::Update(uint64 delta, uint64 absolute) {
		COGMEASURE_BEGIN("ENGINE_UPDATE");

		frameCounter++;

		// update transforms
		stage->GetRootObject()->UpdateTransform(true);
		// update scene
		stage->GetRootObject()->Update(delta, absolute);
		
		inputHandler->HandleInputs();
		
		// remove ended inputs
		environment->RemoveEndedProcesses();

		// flush each 100th frame
		if (frameCounter % 100 == 0) {
			logger->Flush();
		}

		ofSoundUpdate();

		COGMEASURE_END("ENGINE_UPDATE");
	}

	void CogEngine::Draw(uint64 delta, uint64 absolute) {
		COGMEASURE_BEGIN("ENGINE_DRAW");
		
		// has to be here!
		stage->GetRootObject()->UpdateTransform(true);
		
		Node* root = stage->GetRootObject();
		auto children = root->GetChildren();
		
		renderer->BeginRender();

		for (auto it = children.begin(); it != children.end(); ++it) {
			// render scene one by one
			Node* scene = (*it);

			if (scene->GetRunningMode() != RunningMode::INVISIBLE && 
				scene->GetRunningMode() != RunningMode::DISABLED) {
				renderer->InitViewPort(scene->GetScene());
				renderer->ClearCounters();
				scene->Draw(delta, absolute);
				renderer->Render();
			}

		}

		renderer->EndRender();

		COGMEASURE_END("ENGINE_DRAW");
	}

	void CogEngine::RegisterComponents() {
		environment = new Environment();
		resourceCache = new ResourceCache();
		logger = new Logger();
		stage = new Stage();
		renderer = new Renderer();
		inputHandler = new InputHandler();

		REGISTER_COMPONENT(logger);
		REGISTER_COMPONENT(environment);
		REGISTER_COMPONENT(resourceCache);
		REGISTER_COMPONENT(stage);
		REGISTER_COMPONENT(renderer);
		REGISTER_COMPONENT(inputHandler);
		
		auto sceneMgr = new SceneSwitchManager();
		REGISTER_COMPONENT(sceneMgr);

		auto network = new Network();
		REGISTER_COMPONENT(network);

		REGISTER_BEHAVIOR(SheetAnim);
		REGISTER_BEHAVIOR(Button);
		REGISTER_BEHAVIOR(Collider);
		REGISTER_BEHAVIOR(DelayAction);
		REGISTER_BEHAVIOR(DelayRemove);
		REGISTER_BEHAVIOR(HitEvent);
		REGISTER_BEHAVIOR(InputKey);
		REGISTER_BEHAVIOR(Move);
		REGISTER_BEHAVIOR(MultiAnim);
		REGISTER_BEHAVIOR(RotateAnim);
		REGISTER_BEHAVIOR(TransformAnim);
		REGISTER_BEHAVIOR(TranslateAnim);
		REGISTER_BEHAVIOR(SlideTween);
		REGISTER_BEHAVIOR(FloatingScene);
		REGISTER_BEHAVIOR(Selection);

	}

}// namespace