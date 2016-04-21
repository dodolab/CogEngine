#include "CogEngine.h"
#include "ResourceCache.h"
#include "Logger.h"
#include "Node.h"
#include "Stage.h"
#include "Tween.h"
#include "SceneSwitchManager.h"
#include "Renderer.h"
#include "Environment.h"
#include "EntityStorage.h"
#include "InputHandler.h"
#include "ResourceCache.h"
#include "ofSoundPlayer.h"
#include "Button.h"
#include "DelayAction.h"
#include "HitEvent.h"
#include "Move.h"
#include "MultiAnim.h"
#include "TransformAnim.h"
#include "Component.h"
#include "FloatingScene.h"
#include "InputHandler.h"
#include "MultiSelection.h"
#include "Network.h"
#include "SheetAnimator.h"
#include "AttribAnimator.h"
#include "Selection.h"
#include "Slider.h"


namespace Cog {

	CogEngine::CogEngine() {
		entityStorage = new EntityStorage();
	}

	void CogEngine::Init() {

		RegisterComponents();

		vector<Component*> components;
		entityStorage->GetAllComponents(components);
	
		// sort by priority
		sort(components.begin(), components.end(),
			[this](const Component* a, const Component* b) -> bool
		{
			return a->GetPriority() > b->GetPriority();
		});

		// init all components
		for (auto it = components.begin(); it != components.end(); ++it) {
			(*it)->OnInit();
		}
	}

	void CogEngine::Init(string xmlPath) {
		ofxXml* xml = new ofxXml();
		xml->loadFile(xmlPath.c_str());
		auto xmlPtr = spt<ofxXml>(xml);
		Init(xmlPtr);
	}

	void CogEngine::Init(spt<ofxXml> config) {
		
		this->config = config;

		RegisterComponents();

		vector<Component*> components;  
		entityStorage->GetAllComponents(components);

		// sort by priority
		sort(components.begin(), components.end(),
			[this](const Component* a, const Component* b) -> bool
		{
			return a->GetPriority() > b->GetPriority();
		});

		// init logger twice, because it should be always initialized by default at first
		this->logger->OnInit();

		// init all components, using xml file
		for (auto it = components.begin(); it != components.end(); ++it) {
			(*it)->OnInit(config);
		}
	}

	void CogEngine::LoadStageFromXml(spt<ofxXml> config) {
		config->popAll();

		if (config->pushTagIfExists("app_config") && config->pushTagIfExists("scenes")) {
			// load scenes
			auto context = GETCOMPONENT(Stage);
			stage->LoadScenesFromXml(config);
		}
	}

	void CogEngine::Update(uint64 delta, uint64 absolute) {
		COGMEASURE_BEGIN("ENGINE_UPDATE");

		lastAbsoluteTime = absolute;
		frameCounter++;

		// update transform tree
		stage->GetRootObject()->UpdateTransform(true);
		// update scene
		stage->GetRootObject()->Update(delta, absolute);

		// update components
		auto& components = entityStorage->GetComponents();

		for (auto& comp : components) {
			auto state = comp.second->GetComponentState();
			if (state == ComponentState::ACTIVE_UPDATES || state == ComponentState::ACTIVE_ALL) {
				comp.second->Update(delta, absolute);
			}
		}

		// execute post-update actions
		for (auto action : actions) {
			action();
		}

		actions.clear();

		// update transform tree again
		stage->GetRootObject()->UpdateTransform(true);

		// update sound buffer
		ofSoundUpdate();

		COGMEASURE_END("ENGINE_UPDATE");
	}

	void CogEngine::Draw(uint64 delta, uint64 absolute) {
		COGMEASURE_BEGIN("ENGINE_DRAW");

		Node* root = stage->GetRootObject();
		auto children = root->GetChildren();

		renderer->BeginRender();

		for (auto it = children.begin(); it != children.end(); ++it) {
			// render scene nodes one by one
			Node* sceneNode = (*it);

			if (sceneNode->GetRunningMode() != RunningMode::INVISIBLE &&
				sceneNode->GetRunningMode() != RunningMode::DISABLED) {
				// set viewports first since the virtual aspect ratio may differ from the real one
				renderer->InitViewPort(sceneNode->GetScene());
				renderer->ClearBuffers();
				sceneNode->Draw(delta, absolute);
				renderer->Render();
			}
		}

		renderer->EndRender();

		COGMEASURE_END("ENGINE_DRAW");
	}

	void CogEngine::AddPostUpdateAction(function<void()> action) {
		actions.push_back(action);
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

		// register all behaviors that are part of the engine
		REGISTER_BEHAVIOR(SheetAnimator);
		REGISTER_BEHAVIOR(Button);
		REGISTER_BEHAVIOR(DelayAction);
		REGISTER_BEHAVIOR(HitEvent);
		REGISTER_BEHAVIOR(Move);
		REGISTER_BEHAVIOR(MultiAnim);
		REGISTER_BEHAVIOR(TransformAnim);
		REGISTER_BEHAVIOR(SlideTween);
		REGISTER_BEHAVIOR(FloatingScene);
		REGISTER_BEHAVIOR(MultiSelection);
		REGISTER_BEHAVIOR(AttribAnimator);
		REGISTER_BEHAVIOR(Selection);
		REGISTER_BEHAVIOR(Slider);
	}

	void CogEngine::Clear() {
		// only entity storage holds all other objects
		delete entityStorage;
		entityStorage = new EntityStorage();
	}

}// namespace