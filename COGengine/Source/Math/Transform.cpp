#include "Transform.h"

namespace Cog {


	Trans::Trans(float posX, float posY) : localPos(ofVec3f(posX, posY)) {
		SetAbsAsLocal();
	}

	void Trans::SetAbsAsLocal() {
		absPos = localPos;
		absScale = scale;
		absRotation = rotation;
	}

	void Trans::CalcAbsTransform(Trans& parent) {
		absScale = parent.absScale*scale;
	    absRotation = rotation + parent.absRotation;


		if (parent.absRotation == 0) {
			// calculate easily
			absPos = localPos*parent.absScale + parent.absPos;
		}
		else {
			absPos = localPos*parent.absScale + parent.absPos;

			ofVec2f diffFromParent = -((parent.absPos + parent.rotationOrigin*parent.absScale) - (absPos+rotationOrigin*absScale));

			// calc absolute angle
			float angle = ofDegToRad(parent.absRotation) + (atan2(diffFromParent.y, diffFromParent.x));
			// calc local vector length
			float length = sqrt(diffFromParent.x*diffFromParent.x + diffFromParent.y*diffFromParent.y);
			// calc unscaled absolute position
			ofVec3f rotPos = ofVec3f(length*(cos(angle)), length*(sin(angle)), localPos.z);
			// calc absolute position
			absPos = rotPos + parent.absPos + parent.rotationOrigin*parent.absScale - rotationOrigin*absScale;

		}


	}


	ofMatrix4x4 Trans::GetAbsMatrix() {
		ofMatrix4x4 matrix;

		matrix.translate(-rotationOrigin);
		matrix.rotate(absRotation, 0, 0, 1);
		matrix.translate(rotationOrigin);

		matrix.scale(absScale);
		matrix.translate(absPos);

		return matrix;
	}

	ofMatrix4x4 Trans::GetMatrix() {
		ofMatrix4x4 matrix;
		matrix.translate(-rotationOrigin);
		matrix.rotate(rotation, 0, 0, 1);
		matrix.translate(rotationOrigin);


		matrix.scale(scale);
		matrix.translate(localPos);

		return matrix;
	}

} // namespace