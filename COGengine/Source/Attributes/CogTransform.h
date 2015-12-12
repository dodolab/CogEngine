#pragma once

#include "ofxCogMain.h"


namespace Cog {

	/**
	* Transformation entity that holds local and absolute position
	* For now 2D transformation is supported with the possibility of setting Z position and scale
	*/
	class CogTrans {

	public:

		/**
		* Creates a new transformation entity
		* @param posX position in X axis
		* @param posY position in Y axis
		*/
		CogTrans(float posX, float posY) : localPos(ofVec3f(posX, posY)), scale(1), rotationOrigin(ofVec3f(0.0f, 0.0f)), rotation(0) {
			SetAbsAsLocal();
		}

		// local position
		ofVec3f localPos;
		// local scale
		ofVec3f scale;
		// local rotation
		float rotation;
		// local rotation origin
		ofVec3f rotationOrigin;

		// absolute position - recalculated due to parent's transform
		ofVec3f absPos;
		// absolute scale    - recalculated due to parent's transform
		ofVec3f absScale;
		// absolute rotation - recalculated due to parent's transform
		float absRotation;
		// absolute rotation origin - recalculated due to parent's transform
		ofVec3f absRotationOrigin;

		/**
		* Sets absolute coordinates to be the same as local
		* Mostly used for root objects that haven't any parent
		*/
		void SetAbsAsLocal() {
			absPos = localPos;
			absScale = scale;
			absRotation = rotation;
			absRotationOrigin = rotationOrigin;
		}

		/**
		* Calculates absolute coordinates
		* @param parent parent whose transform will be calculated according to
		*/
		void CalcAbsTransform(CogTrans& parent) {
			// calc absolute angle
			float angle = ofDegToRad(parent.absRotation) + (atan2(localPos.y, localPos.x));
			// calc local vector length
			float length = sqrt(localPos.x*localPos.x + localPos.y*localPos.y);
			// calc unscaled absolute position
			ofVec3f rotPos = ofVec3f(length*cos(angle), length*sin(angle), localPos.z);
			// calc absolute position
			absPos = rotPos*parent.absScale + parent.absPos;
			absScale = parent.absScale*scale;
			absRotation = rotation + parent.absRotation;
			absRotationOrigin = rotationOrigin + parent.absRotationOrigin;
		}


		ofMatrix4x4 GetAbsMatrix() {
			ofMatrix4x4 matrix;

			matrix.translate(absRotationOrigin);
			matrix.rotate(absRotation, 0, 0, 1);

			matrix.scale(absScale);
			matrix.translate(absPos);

			return matrix;
		}

		ofMatrix4x4 GetMatrix() {
			ofMatrix4x4 matrix;
			matrix.translate(rotationOrigin);
			matrix.rotate(rotation, 0, 0, 1);

			matrix.scale(scale);
			matrix.translate(localPos);

			return matrix;
		}
	};

}