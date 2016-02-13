#include "Transform.h"

namespace Cog {


	Trans::Trans(float posX, float posY) : localPos(ofVec3f(posX, posY)) {
		SetAbsAsLocal();
	}

	void Trans::SetAbsAsLocal() {
		absPos = localPos;
		absScale = scale;
		absRotation = rotation;
		absRotationOrigin = rotationOrigin;
	}

	void Trans::CalcAbsTransform(Trans& parent) {
		if (parent.absRotation == 0) {
			// calculate easily
			absPos = localPos*parent.absScale + parent.absPos;
		}
		else {

			// calc absolute angle
			float angle = ofDegToRad(parent.absRotation) + (atan2(localPos.y, localPos.x));
			// calc local vector length
			float length = sqrt(localPos.x*localPos.x + localPos.y*localPos.y);
			// calc unscaled absolute position
			ofVec3f rotPos = ofVec3f(length*cos(angle), length*sin(angle), localPos.z);
			// calc absolute position
			absPos = rotPos*parent.absScale + parent.absPos;
		}

		absScale = parent.absScale*scale;
		absRotation = rotation + parent.absRotation;
		absRotationOrigin = parent.absRotationOrigin + rotationOrigin;
	}


	ofMatrix4x4 Trans::GetAbsMatrix() {
		ofMatrix4x4 matrix;

		matrix.translate(-absRotationOrigin);
		matrix.rotate(absRotation, 0, 0, 1);
		matrix.translate(absRotationOrigin);


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