#ifndef GAMEFACTORY_H
#define GAMEFACTORY_H


#include "GNode.h"

class MGameFactory{

protected:
	Vectorf3 CalcScale(spt<ofImage> img, float width, GNode* scene);

	float RelPosX(float posX, GNode* scene);

	float RelPosY(float posY, GNode* scene);

	void SetTransform(GNode* node, float posX, float posY, float scale);

	void SetRenderImage(GNode* node, string imgPath, bool isHittable);

	void SetSingleBackground(GNode* node, string imgPath, bool isHittable);

	Vectorf3 GetPercentageVec(float xPerc, float yPerc);


public:
	void Init(){

	}
	void Terminate(){

	}

	virtual void SwitchToScene(int scene);
	
	virtual GNode* CreateRoot();


};



#endif