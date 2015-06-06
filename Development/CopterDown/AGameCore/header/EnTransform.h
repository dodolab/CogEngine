#ifndef ENTRANSFORM_H
#define ENTRANSFORM_H

#include "IwGeomFVec2.h"
#include "IwGeomFMat2D.h"
#include "Enums.h"

class EnTransform{
	
public:
	EnTransform(float posX, float posY) : LocalPos(Vectorf2(posX, posY)), Scale(1), RotationOrigin(Vectorf2(0.0f, 0.0f)), Rotation(0){

	}

	Vectorf2 LocalPos;
	float Scale;
	float Rotation;
	Vectorf2 RotationOrigin;

	CIwFMat2D GetMatrix(){
		CIwFMat2D matrix;

		
		//matrix.SetTrans(RotationOrigin);
		
		matrix.SetRot(Rotation, RotationOrigin);
		
		matrix.SetTrans(matrix.GetTrans() + LocalPos);
		matrix.ScaleRot(Scale);
		return matrix;
	}
};


#endif
