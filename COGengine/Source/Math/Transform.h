#pragma once

#include "ofMain.h"

namespace Cog {

	/**
	* Transformation entity that holds local and absolute position
	* For now 2D transformation is supported with the possibility of setting Z position and scale
	*/
	class Trans {

	public:
		Trans() {

		}

		/**
		* Creates a new transformation entity
		* @param posX position in X axis
		* @param posY position in Y axis
		*/
		Trans(float posX, float posY);

		// local position
		ofVec3f localPos = ofVec3f(0);
		// local scale
		ofVec3f scale = ofVec3f(1);
		// local rotation
		float rotation = 0;
		// local rotation centroid
		ofVec3f rotationCentroid = ofVec3f(0);

		// absolute position - recalculated due to parent's transform
		ofVec3f absPos = ofVec3f(0);
		// absolute scale    - recalculated due to parent's transform
		ofVec3f absScale = ofVec3f(0);
		// absolute rotation - recalculated due to parent's transform
		float absRotation = 0;
		// absolute rotation centroid
		ofVec3f absRotationCentroid = ofVec3f(0);

		/**
		* Sets absolute coordinates to be the same as local
		* Mostly used for root objects that haven't any parent
		*/
		void SetAbsAsLocal();

		/**
		* Calculates absolute coordinates
		* @param parent parent whose transform will be calculated according to
		*/
		void CalcAbsTransform(Trans& parent);

		/**
		* Calculates rotation to the selected position
		*/
		float CalcRotationToPosition(ofVec2f pos);

		/**
		* Sets rotation to the selected local position so that the object will
		* be oriented to the direction
		*/
		void SetRotationToPosition(ofVec2f pos);

		ofMatrix4x4 GetAbsMatrix();

		ofMatrix4x4 GetMatrix();
	};

}// namespace