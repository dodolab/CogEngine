#pragma once

#include "ofxCogMain.h
#include "CogEngine.h"
#include "RotateAnim.h"
#include "Transform.h"


class CogTestFactory : public CogFactory{
private: 
	int actualTest;
	CogNode* root;
public:
	CogTestFactory(){
		actualTest = 0;
	}

	CogNode* CreateRoot(){
		root = new CogNode(CogObjType::ROOT, 12,"root");
		root->AddAttr(Attrs::COLOR, ofColor(222,222,111));
		root->AddAttr(Attrs::SIZE, ofVec3f(100));
		root->AddBehavior(new CogRotateAnim(0, 0, 2, false));
		root->GetTransform().scale = ofVec3f(0.4f);
		return root;
	}
};



/**
* Basic unit tests for COGEngine
*/
class MTestApp
#ifdef WIN32
	:public ofBaseApp
#else
	: public ofxAndroidApp
#endif
{
protected:
	// time elapsed from engine initialization
	uint64 absolute;
	// time elapsed Tween frames
	uint64 delta;
	// link to default factory
	CogFactory* factory;

public:

	/**
	* Creates a new application wrapper
	* @param factory default factory
	*/
	MTestApp() : factory(new CogTestFactory()){
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
