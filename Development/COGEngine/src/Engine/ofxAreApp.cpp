#pragma once

#include "ofxAreApp.h"
#include "ofxAreEngine.h"

#define APP_SPEED 60

void ofxAreApp::setup(){
	// never set vertical sync
	ofSetVerticalSync(false);
	ofSetFrameRate(APP_SPEED);
	ofEnableAntiAliasing();
	// initialize COG engine
	COGEngine.Init(factory, config);
	// initialize time
	absolute = ofGetSystemTime();
	delta = ofGetSystemTime();
}

void ofxAreApp::draw(){
	// drawing loop
	COGEngine.Draw(delta, absolute);
}

void ofxAreApp::update(){
	// update loop
	delta = ofGetSystemTime() - absolute;
	absolute = ofGetSystemTime();

	uint64 semiFixedDelta = (delta < 16) ? 16 : (delta < 32) ? delta : 32;

	COGEngine.Update(semiFixedDelta, absolute);
}

void ofxAreApp::keyPressed(int key){
	COGEngine.environmentCtrl->OnKeyAction(key, true);
}

void ofxAreApp::keyReleased(int key){
	COGEngine.environmentCtrl->OnKeyAction(key, false);
}

void ofxAreApp::windowResized(int w, int h){
	COGEngine.environmentCtrl->OnScreenSizeChanged(w, h);
}



#ifdef TARGET_WINDOWS

void ofxAreApp::mouseMoved(int x, int y){

}

void ofxAreApp::mouseDragged(int x, int y, int button){
	COGEngine.environmentCtrl->OnSingleTouchMotion(x, y, button);

}

void ofxAreApp::mousePressed(int x, int y, int button){
	COGEngine.environmentCtrl->OnSingleTouchButton(x, y, button, true);
}

void ofxAreApp::mouseReleased(int x, int y, int button){
	COGEngine.environmentCtrl->OnSingleTouchButton(x, y, button, false);
}

void ofxAreApp::dragEvent(ofDragInfo dragInfo){

}


#else


void ofxAreApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

void ofxAreApp::pause(){

}

void ofxAreApp::stop(){

}

void ofxAreApp::resume(){

}

void ofxAreApp::reloadTextures(){

}

bool ofxAreApp::backPressed(){
	return false;
}

void ofxAreApp::okPressed(){

}

void ofxAreApp::cancelPressed(){

}

void ofxAreApp::touchDown(int x, int y, int id){
	COGEngine.environmentCtrl->OnMultiTouchButton(x, y, id, true);
}

void ofxAreApp::touchMoved(int x, int y, int id){
	COGEngine.environmentCtrl->OnMultiTouchMotion(x, y, id);
}

void ofxAreApp::touchUp(int x, int y, int id){
	COGEngine.environmentCtrl->OnMultiTouchButton(x, y, id, false);
}

void ofxAreApp::touchDoubleTap(int x, int y, int id){

}

void ofxAreApp::touchCancelled(int x, int y, int id){

}

#endif