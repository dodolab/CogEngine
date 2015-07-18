#pragma once

#include "GBehavior.h"
#include "GNode.h"
#include "SmartPointer.h"
#include "EnPolygon.h"
#include "BeRender.h"


/**
* Behavior for temporary render
*/
class BeTempRender : public GBehavior{
private:
	spt<ofImage> image;
	int framesNum;
	int actualFrame;

public:
	BeTempRender(spt<ofImage> image, int framesNum) : GBehavior(ElemType::VIEW){
		this->image = image;
		this->framesNum = framesNum;
		this->actualFrame = 0;
	}


	virtual void Update(const uint64 delta, const uint64 absolute){

		if (actualFrame++ > framesNum){
			owner->RemoveBehavior(this,false,true);
			return;
		}

		ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
		ofLoadMatrix(absM);
		ofSetColor(0x000000ff);
		image->draw(-image->getWidth() / 2, -image->getHeight() / 2);

	}

};
