#pragma once

#include "ofMain.h"
#ifndef TARGET_WINDOWS
#include "ofxAndroid.h"
#endif
#include "MEngine.h"
#include "BeRender.h"
#include "BeRotateAnim.h"
#include "EnTransform.h"


class MTestFactory : public MFactory{
private: 
	int actualTest;
	GNode* root;
public:
	MTestFactory(){
		actualTest = 0;
	}

	GNode* CreateRoot(){
		root = new GNode(ObjType::ROOT, 12,"root");
		root->AddAttr(Attrs::COLOR, ofColor(222,222,111));
		root->AddAttr(Attrs::SIZE, ofVec3f(100));
		root->AddBehavior(new BeRender(RenderType::RECTANGLE),true);
		root->AddBehavior(new BeRotateAnim(0, 0, 2, false),true);
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
	// time elapsed between frames
	uint64 delta;
	// link to default factory
	MFactory* factory;

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
		COGEngine.Init(factory);
	}

	void draw(){
		COGEngine.Draw(delta, absolute);
	}

	void update(){
		delta = ofGetSystemTime() - absolute;
		absolute = ofGetSystemTime();

	}
};
