#ifndef ENTRANSFORM_H
#define ENTRANSFORM_H

#include "Enums.h"
#include "ofVec3f.h"

class EnTransform{
	
public:
	EnTransform(float posX, float posY) : LocalPos(ofVec3f(posX, posY)), Scale(1), RotationOrigin(ofVec3f(0.0f, 0.0f)), Rotation(0){
		SetAbsAsLocal();
	}

	ofVec3f LocalPos;
	ofVec3f Scale;
	float Rotation;
	ofVec3f RotationOrigin;

	ofVec3f AbsPos;
	ofVec3f AbsScale;
	float AbsRotation;
	ofVec3f AbsRotationOrigin;

	void SetAbsAsLocal(){
		AbsPos = LocalPos;
		AbsScale = Scale;
		AbsRotation = Rotation;
		AbsRotationOrigin = RotationOrigin;
	}

	void CalcAbsTransform(EnTransform& parent){
		float angle = LocalPos.x == 0 ? 0 : parent.AbsRotation / 180.0f*PI+(atan2(LocalPos.y, LocalPos.x));
		float length = sqrt(LocalPos.x*LocalPos.x + LocalPos.y*LocalPos.y);
		ofVec3f rotPos = ofVec3f(length*cos(angle), length*sin(angle), LocalPos.z);
		AbsPos = rotPos*parent.AbsScale + parent.AbsPos;
		AbsScale = parent.AbsScale*Scale;
		AbsRotation = Rotation + parent.AbsRotation;
		AbsRotationOrigin = RotationOrigin + parent.AbsRotationOrigin;
	}
	
	ofMatrix4x4 GetAbsMatrix(){
		ofMatrix4x4 matrix;

		matrix.translate(AbsRotationOrigin);
		matrix.rotate(AbsRotation, 0, 0, 1);

		matrix.scale(AbsScale);
		matrix.translate(AbsPos);

		return matrix;
	}

	ofMatrix4x4 GetMatrix(){
		ofMatrix4x4 matrix;

		matrix.translate(RotationOrigin);
		matrix.rotate(Rotation, 0,0,1);
	
		matrix.scale(Scale);	
		matrix.translate(LocalPos);	

		return matrix;
	}
};


#endif
