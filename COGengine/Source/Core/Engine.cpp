#include "Engine.h"
#include "TranslateAnim.h"
#include "Node.h"
#include "RotateAnim.h"
#include "Factory.h"
#include "Cache.h"
#include "ofSoundPlayer.h"

namespace Cog {

	Engine COGEngine;


	void Engine::Init(Factory* factory, spt<ofxXmlSettings> config) {

		// create components
		environmentCtrl = new Environment();
		resourceCtrl = new Cache();
		logger = new Logger(config);
		storage = new Repository();
		renderer = new Renderer();
		this->factory = factory;

		// initialize components 
		environmentCtrl->Init();
		factory->Init();
		logger->Init();
		renderer->Init();
		resourceCtrl->Init();
		// create root node
		_root = factory->CreateRoot();
	}


	void Engine::Update(uint64 delta, uint64 absolute) {
		frameCounter++;

		// update transforms
		this->_root->GetRoot()->UpdateTransform(true);
		// update scene
		this->_root->GetRoot()->Update(delta, absolute);
		// remove ended inputs
		environmentCtrl->RemoveEndedProcesses();

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
		ofSetupScreenOrtho(environmentCtrl->GetRealWidth(), environmentCtrl->GetRealHeight(), -1000, 1000);

		// draw the root node
		this->_root->GetRoot()->Draw(delta, absolute);

		renderer->Render();
	}

}
