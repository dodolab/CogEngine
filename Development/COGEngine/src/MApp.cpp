#pragma once

#include "MApp.h"
#include "MGameEngine.h"

#define GAME_SPEED 60

void MApp::setup(){
	// never set vertical sync
	ofSetVerticalSync(false);
	ofSetFrameRate(GAME_SPEED);
	ofEnableAntiAliasing();
	// initialize game engine
	MEngine.Init(factory);
	// initialize time
	absolute = ofGetSystemTime();
	delta = ofGetSystemTime();
}

void MApp::draw(){
	// drawing loop
	MEngine.Draw(delta, absolute);
}

void MApp::update(){
	// update loop
	delta = ofGetSystemTime() - absolute;
	absolute = ofGetSystemTime();
	MEngine.Update(delta, absolute);
}

void MApp::keyPressed(int key){
	MEngine.environmentCtrl->OnKeyAction(key, true);
}

void MApp::keyReleased(int key){
	MEngine.environmentCtrl->OnKeyAction(key, false);
}

void MApp::windowResized(int w, int h){
	MEngine.environmentCtrl->OnScreenSizeChanged(w, h);
}



#ifdef TARGET_WINDOWS

void MApp::mouseMoved(int x, int y){

}

void MApp::mouseDragged(int x, int y, int button){
	MEngine.environmentCtrl->OnSingleTouchMotion(x, y, button);

}

void MApp::mousePressed(int x, int y, int button){
	MEngine.environmentCtrl->OnSingleTouchButton(x, y, button, true);
}

void MApp::mouseReleased(int x, int y, int button){
	MEngine.environmentCtrl->OnSingleTouchButton(x, y, button, false);
}

void MApp::dragEvent(ofDragInfo dragInfo){

}


#else


void MApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

void MApp::pause(){

}

void MApp::stop(){

}

void MApp::resume(){

}

void MApp::reloadTextures(){

}

bool MApp::backPressed(){
	return false;
}

void MApp::okPressed(){

}

void MApp::cancelPressed(){

}

void MApp::touchDown(int x, int y, int id){
	MEngine.environmentCtrl->OnMultiTouchButton(x, y, id, true);
}

void MApp::touchMoved(int x, int y, int id){
	MEngine.environmentCtrl->OnMultiTouchMotion(x, y, id);
}

void MApp::touchUp(int x, int y, int id){
	MEngine.environmentCtrl->OnMultiTouchButton(x, y, id, false);
}

void MApp::touchDoubleTap(int x, int y, int id){

}

void MApp::touchCancelled(int x, int y, int id){

}

#endif