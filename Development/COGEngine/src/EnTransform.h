#ifndef ENTRANSFORM_H
#define ENTRANSFORM_H

#include "Enums.h"
#include "ofVec3f.h"

class EnTransform{
	
public:
	EnTransform(float posX, float posY) : LocalPos(ofVec3f(posX, posY)), Scale(1), RotationOrigin(ofVec3f(0.0f, 0.0f)), Rotation(0), Anchor(0,0){

	}

	ofVec3f LocalPos;
	ofVec3f Scale;
	float Rotation;
	ofVec3f RotationOrigin;
	ofVec2f Anchor;

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
