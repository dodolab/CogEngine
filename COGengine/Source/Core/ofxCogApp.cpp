#include "ofxCogApp.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "ofxCogEngine.h"
#include "Stage.h"
#include "Environment.h"
#include "ComponentStorage.h"
#include "Tween.h"

namespace Cog {

	void ofxCogApp::InitEngine() {
		if (!xmlConfig) {
			ofxCogEngine::GetInstance().Init();
		}
		else {
			ofxCogEngine::GetInstance().Init(xmlConfig);
		}
	}

	void ofxCogApp::InitStage(Stage* stage) {
		// nothing to do here
	}

	void ofxCogApp::setup() {
		// never set vertical sync since it may cause flickering
		ofSetVerticalSync(false);
		ofSetFrameRate(this->fps);
		ofEnableAntiAliasing();
		setupEngine();
	}

	void ofxCogApp::setupEngine() {
		this->RegisterComponents();
		this->InitEngine();
		this->InitComponents();

		auto stage = GETCOMPONENT(Stage);
		this->InitStage(stage);

		// initialize time
		absolute = ofGetSystemTime();
		delta = ofGetSystemTime();

		engineInitialized = true;
	}

	void ofxCogApp::draw() {
		if (engineInitialized) {
			// drawing loop
			ofxCogEngine::GetInstance().Draw(delta, absolute);
		}
	}

	void ofxCogApp::update() {

		if (engineInitialized) {
			// update loop
			delta = ofGetSystemTime() - absolute;
			absolute = ofGetSystemTime();

			float fpsThreshold = 1000 / this->GetFps();
			// when performance goes down, the maximum delta value is fixed
			uint64 semiFixedDelta = (delta < fpsThreshold) ? fpsThreshold : (delta < (2 * fpsThreshold)) ? delta : (2 * fpsThreshold);
			ofxCogEngine::GetInstance().Update(semiFixedDelta, absolute);
		}
	}

	void ofxCogApp::keyPressed(int key) {
		ofxCogEngine::GetInstance().environment->OnKeyAction(key, true);
	}

	void ofxCogApp::keyReleased(int key) {
		ofxCogEngine::GetInstance().environment->OnKeyAction(key, false);
	}

	void ofxCogApp::windowResized(int w, int h) {
		if (engineInitialized) {
			ofxCogEngine::GetInstance().environment->OnScreenSizeChanged(w, h);
		}
	}



#ifdef WIN32

	void ofxCogApp::mouseMoved(int x, int y) {

	}

	void ofxCogApp::mouseDragged(int x, int y, int button) {
		ofxCogEngine::GetInstance().environment->OnSingleTouchMotion(x, y, button);

	}

	void ofxCogApp::mousePressed(int x, int y, int button) {
		ofxCogEngine::GetInstance().environment->OnSingleTouchButton(x, y, button, true);
	}

	void ofxCogApp::mouseReleased(int x, int y, int button) {
		ofxCogEngine::GetInstance().environment->OnSingleTouchButton(x, y, button, false);
	}

	void ofxCogApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
		ofxCogEngine::GetInstance().environment->OnSingleScroll(x, y, scrollX, scrollY);
	}

	void ofxCogApp::dragEvent(ofDragInfo dragInfo) {

	}


#else


	void ofxCogApp::swipe(ofxAndroidSwipeDir swipeDir, int id) {

	}

	void ofxCogApp::pause() {

	}

	void ofxCogApp::stop() {

	}

	void ofxCogApp::resume() {

	}

	void ofxCogApp::reloadTextures() {

	}

	bool ofxCogApp::backPressed() {
		if (OnBackPress()) {
			ofxCogEngine::GetInstance().stage->SwitchBackToScene(TweenDirection::NONE);
		}
		return true;
	}

	void ofxCogApp::okPressed() {

	}

	void ofxCogApp::cancelPressed() {

	}

	void ofxCogApp::touchDown(int x, int y, int id) {
		ofxCogEngine::GetInstance().environment->OnMultiTouchButton(x, y, id, true);
	}

	void ofxCogApp::touchMoved(int x, int y, int id) {
		ofxCogEngine::GetInstance().environment->OnMultiTouchMotion(x, y, id);
	}

	void ofxCogApp::touchUp(int x, int y, int id) {
		ofxCogEngine::GetInstance().environment->OnMultiTouchButton(x, y, id, false);
	}

	void ofxCogApp::touchDoubleTap(int x, int y, int id) {

	}

	void ofxCogApp::touchCancelled(int x, int y, int id) {

	}

#endif


}// namespace