#pragma once
#include "GNode.h"
#include "BeSceneManager.h"


class MGameFactory{

protected:

	ofVec3f CalcScale(spt<ofImage> img, float width, GNode* scene);

	float RelPosX(float posX, GNode* scene);

	float RelPosY(float posY, GNode* scene);

	ofVec2f GetCenter();

	
	void SetRenderImage(GNode* node, string imgPath, bool isHittable);

	void SetRenderImage(GNode* node, string imgPath, float pScaleX, ofVec2f pPos, ofVec2f anchor);

	void SetRenderImageAbsolute(GNode* node, string imgPath, ofVec2f pos, bool absolutePos, float scaleX, bool absoluteScale, ofVec2f anchor, GNode* parent);

	ofVec3f GetPercentageVec(float xPerc, float yPerc);

	void SetTransformRelPixel(GNode* node, float posX, float posY, ofVec2f scale);

	void SetTransformAbs(GNode* node, ofVec2f pos, bool absolutePos, float scaleX, bool absoluteScale, ofVec2f anchor, int width, int height, GNode* parent);


public:
	BeSceneManager* sceneManager;

	void Init(){

	}
	void Terminate(){

	}

	virtual void SwitchToScene(int scene);
	
	virtual GNode* CreateRoot();


};

