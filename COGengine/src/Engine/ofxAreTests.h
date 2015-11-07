#pragma once

#include "ofxAreMain.h
#include "ofxAreEngine.h"
#include "ofxArbRotateAnim.h"
#include "ofxAraTransform.h"


class MTestFactory : public ofxArcFactory{
private: 
	int actualTest;
	ofxAreNode* root;
public:
	MTestFactory(){
		actualTest = 0;
	}

	ofxAreNode* CreateRoot(){
		root = new ofxAreNode(ObjType::ROOT, 12,"root");
		root->AddAttr(Attrs::COLOR, ofColor(222,222,111));
		root->AddAttr(Attrs::SIZE, ofVec3f(100));
		root->AddBehavior(new ofxArbRotateAnim(0, 0, 2, false));
		root->GetTransform().scale = ofVec3f(0.4f);
		return root;
	}
};



/**
* Basic unit tests for COGEngine
*/
class MTestApp
#ifdef TARGET_WINDOWS
	:public ofBaseApp
#else
	: public ofxAndroidApp
#endif
{
protected:
	// time elapsed from engine initialization
	uint64 absolute;
	// time elapsed ofxArbTween frames
	uint64 delta;
	// link to default factory
	ofxArcFactory* factory;

public:

	/**
	* Creates a new application wrapper
	* @param factory default factory
	*/
	MTestApp() : factory(new MTestFactory()){
	}


	void setup(){
		ofSetVerticalSync(false);
		ofSetFrameRate(60);
		COGEngine.Init(factory,spt<ofxXmlSettings>());
	}

	void draw(){
		COGEngine.Draw(delta, absolute);
	}

	void update(){
		delta = ofGetSystemTime() - absolute;
		absolute = ofGetSystemTime();

	}
};
