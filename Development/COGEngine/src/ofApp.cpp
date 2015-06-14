#include "ofApp.h"
#include "MGameEngine.h"
#include "CopterFactory.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(50);   
	ofEnableSmoothing();
	ofEnableAntiAliasing();
	// initialize game engine
	MEngine.Init(new CopterFactory(), new MGameStorage());
	absolute = ofGetSystemTime();
	delta = ofGetSystemTime();
}

//--------------------------------------------------------------
void ofApp::update(){
	delta = ofGetSystemTime() - absolute;
	absolute = ofGetSystemTime();
	MEngine.Update(delta, absolute);
}

//--------------------------------------------------------------
void ofApp::draw(){
	MEngine.Draw(delta, absolute);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	MEngine.environmentCtrl->OnKeyAction(key,true);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	MEngine.environmentCtrl->OnKeyAction(key,false);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	MEngine.environmentCtrl->OnSingleTouchMotion(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	MEngine.environmentCtrl->OnSingleTouchButton(x,y,button,true);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	MEngine.environmentCtrl->OnSingleTouchButton(x,y,button,false);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	MEngine.environmentCtrl->OnScreenSizeChanged(w,h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
