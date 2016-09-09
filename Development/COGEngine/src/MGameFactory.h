#pragma once
#include "GNode.h"
#include "BeSceneManager.h"

/**
* Game factory with methods for calculating positions
* All game object construction methods should be implemented
* in classes derived from this one
*/
class MGameFactory{

public:

	ofVec2f GetCenter();

	void SetRenderImage(GNode* node, string imgPath, bool isHittable);

	void SetRenderImage(GNode* node, string imgPath, float pScaleX, ofVec2f pPos, ofVec2f anchor);

	void SetRenderImageAbsolute(GNode* node, string imgPath, ofVec2f pos, bool absolutePos, float scaleX, bool absoluteScale, ofVec2f anchor, GNode* parent);

	ofVec2f CalcPositionAbsolute(GNode* node, ofVec2f pos, bool absolutePos, ofVec2f anchor, GNode* parent);

	ofVec3f CalsScaleAbsolute(GNode* node, float scaleX, float width, bool absoluteScale, GNode* parent);

	ofVec3f GetPercentageVec(float xPerc, float yPerc);

	void SetTransformRelPixel(GNode* node, float posX, float posY, ofVec2f scale);

	void SetTransformAbs(GNode* node, ofVec2f pos, bool absolutePos, float scaleX, bool absoluteScale, ofVec2f anchor, int width, int height, GNode* parent);

	float TranslateSpeed(float speed);

	BeSceneManager* sceneManager;

	virtual void Init(){

	}

	virtual void SwitchToScene(int scene);
	
	virtual GNode* CreateRoot();


};

