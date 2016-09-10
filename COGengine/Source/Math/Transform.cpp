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
		absPos = localPos*parent.absScale + parent.absPos;
		absRotationCentroid = rotationCentroid * parent.absScale;

		if (parent.absRotation != 0) {
			// rotate around parent's origin
			
			// calculate real local position against parent's rotation origin
			ofVec2f realLocPos =   (absPos + absRotationCentroid) - (parent.absPos + parent.absRotationCentroid);
			float length = realLocPos.length();

			// calculate angle against parent
			float angle = ofDegToRad(parent.absRotation) + (atan2f(realLocPos.y, realLocPos.x));
	
			// calculate rotation offset
			ofVec3f rotPos = ofVec3f(length*(cos(angle)), length*(sin(angle)), localPos.z);

			// calculate absolute position according to the parent's rotation origin
			absPos = parent.absPos + parent.absRotationCentroid + rotPos - absRotationCentroid;
		}
	}

	float Trans::CalcRotationToPosition(ofVec2f pos) {
		ofVec2f diff = pos - localPos;
		if (abs(diff.x) > 1E-3 && abs(diff.y) > 1E-3) {
			return RAD_TO_DEG*(atan2f(diff.x, -diff.y));
		}
	}

	void Trans::SetRotationToPosition(ofVec2f pos) {
		this->rotation = CalcRotationToPosition(pos);
	}

	ofMatrix4x4 Trans::GetAbsMatrix() {
		ofMatrix4x4 matrix;

		matrix.translate(-absRotationCentroid / absScale);
		matrix.rotate(absRotation, 0, 0, 1);
		matrix.translate(absRotationCentroid  / absScale);

		matrix.scale(absScale);
		matrix.translate(absPos);

		return matrix;
	}

	ofMatrix4x4 Trans::GetMatrix() {
		ofMatrix4x4 matrix;
		matrix.translate(-rotationCentroid*absScale);
		matrix.rotate(rotation, 0, 0, 1);
		matrix.translate(rotationCentroid*absScale);


		matrix.scale(scale);
		matrix.translate(localPos);

		return matrix;
	}

} // namespace