#pragma once
#include "GNode.h"


class MGameFactory{

protected:
	ofVec3f CalcScale(spt<ofImage> img, float width, GNode* scene);

	float RelPosX(float posX, GNode* scene);

	float RelPosY(float posY, GNode* scene);

	ofVec2f GetCenter();

	void SetTransform(GNode* node, float posX, float posY, ofVec2f scale);

	void SetRenderImage(GNode* node, string imgPath, bool isHittable);

	void SetRenderImage(GNode* node, string imgPath, float pScaleX, ofVec2f pPos, ofVec2f anchor);

	ofVec3f GetPercentageVec(float xPerc, float yPerc);


public:
	void Init(){

	}
	void Terminate(){

	}

	virtual void SwitchToScene(int scene);
	
	virtual GNode* CreateRoot();


};

