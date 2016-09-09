#ifndef ENTRANSFORM_H
#define ENTRANSFORM_H

#include "Enums.h"

class EnTransform{
	
public:
	EnTransform(float posX, float posY) : LocalPos(Vectorf3(posX, posY)), Scale(1), RotationOrigin(Vectorf3(0.0f, 0.0f)), Rotation(0){

	}

	Vectorf3 LocalPos;
	float Scale;
	float Rotation;
	Vectorf3 RotationOrigin;

	ofMatrix4x4 GetMatrix(){
		ofMatrix4x4 matrix;

		
		matrix.translate(RotationOrigin);
		matrix.rotate(Rotation, 0,0,1);
	
		matrix.scale(Scale,Scale,1);	
		matrix.translate(LocalPos);	
		
		return matrix;
	}
};


#endif
