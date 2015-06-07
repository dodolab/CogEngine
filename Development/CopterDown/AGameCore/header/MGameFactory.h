#ifndef GAMEFACTORY_H
#define GAMEFACTORY_H


#include "GNode.h"

class MGameFactory{

protected:
	void SetTransform(GNode* node, float posX, float posY, float scale);

	void SetRenderImage(GNode* node, string imgPath);

	void SetRenderHitImage(GNode* node, string imgPath);

	void SetSingleBackground(GNode* node, string imgPath);

	Vectorf2 GetPercentageVec(float xPerc, float yPerc);


public:
	void Init(){

	}
	void Terminate(){

	}
	
	virtual GNode* CreateRoot();


};



#endif