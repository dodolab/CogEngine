#include "IwGeomFVec2.h"
#include "IwGeomFMat2D.h"

#ifndef ENTRANSFORM_H
#define ENTRANSFORM_H

class EnTransform{
	
public:
	EnTransform(float posX, float posY) : LocalPos(CIwFVec2(posX, posY)), Scale(1), RotationOrigin(CIwFVec2(0.0f, 0.0f)), Rotation(0){

	}

	CIwFVec2 LocalPos;
	float Scale;
	float Rotation;
	CIwFVec2 RotationOrigin;

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
