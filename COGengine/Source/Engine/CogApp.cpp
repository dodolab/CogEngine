#pragma once

#include "CogApp.h"
#include "CogEngine.h"

#define APP_SPEED 60

namespace Cog {

	void CogApp::InitComponents() {

	}

	void CogApp::InitEngine() {
		CogEngine::GetInstance().Init();
	}

void CogApp::setup(){
	// never set vertical sync
	ofSetVerticalSync(false);
	ofSetFrameRate(APP_SPEED);
	ofEnableAntiAliasing();
	// initialize COG engine
	this->InitComponents();
	this->InitEngine();
	
	// initialize time
	absolute = ofGetSystemTime();
	delta = ofGetSystemTime();
}

void CogApp::draw(){
	// drawing loop
	CogEngine::GetInstance().Draw(delta, absolute);
}

void CogApp::update(){
	// update loop
	delta = ofGetSystemTime() - absolute;
	absolute = ofGetSystemTime();

	uint64 semiFixedDelta = (delta < 16) ? 16 : (delta < 32) ? delta : 32;

	CogEngine::GetInstance().Update(semiFixedDelta, absolute);
}

void CogApp::keyPressed(int key){
	CogEngine::GetInstance().environment->OnKeyAction(key, true);
}

void CogApp::keyReleased(int key){
	CogEngine::GetInstance().environment->OnKeyAction(key, false);
}

void CogApp::windowResized(int w, int h){
	CogEngine::GetInstance().environment->OnScreenSizeChanged(w, h);
}



#ifdef WIN32

void CogApp::mouseMoved(int x, int y){

}

void CogApp::mouseDragged(int x, int y, int button){
	CogEngine::GetInstance().environment->OnSingleTouchMotion(x, y, button);

}

void CogApp::mousePressed(int x, int y, int button){
	CogEngine::GetInstance().environment->OnSingleTouchButton(x, y, button, true);
}

void CogApp::mouseReleased(int x, int y, int button){
	CogEngine::GetInstance().environment->OnSingleTouchButton(x, y, button, false); 
}

void CogApp::dragEvent(ofDragInfo dragInfo){

}


#else


void CogApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

void CogApp::pause(){

}

void CogApp::stop(){

}

void CogApp::resume(){

}

void CogApp::reloadTextures(){

}

bool CogApp::backPressed(){
	CogEngine::GetInstance().stage->SwitchBackToScene(TweenDirection::NONE);
	return true;
}

void CogApp::okPressed(){

}

void CogApp::cancelPressed(){

}

void CogApp::touchDown(int x, int y, int id){
	CogEngine::GetInstance().environment->OnMultiTouchButton(x, y, id, true);
}

void CogApp::touchMoved(int x, int y, int id){
	CogEngine::GetInstance().environment->OnMultiTouchMotion(x, y, id);
}

void CogApp::touchUp(int x, int y, int id){
	CogEngine::GetInstance().environment->OnMultiTouchButton(x, y, id, false);
}

void CogApp::touchDoubleTap(int x, int y, int id){

}

void CogApp::touchCancelled(int x, int y, int id){

}

#endif

}// namespace