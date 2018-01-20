#pragma once
#include "ofVec2f.h"
#include "Transform.h"
#include "Constants.h"
#include "Definitions.h"

namespace Cog {

	class Node;

	class TransformBuilder {
	private:
		ofVec2f pos;
		int zIndex;
		stenum pType = CALCTYPE_LOC;
		ofVec2f anchor;
		ofVec2f size;
		ofVec2f rotationCentroid;
		stenum sType = CALCTYPE_LOC;
		float rotation;
		int gridWidth = 0;
		int gridHeight = 0;
		bool checkSceneScale = true;
		bool checkNativeResolution = true;
	public:

		TransformBuilder() {
			SetDefaultVals();
		}

		TransformBuilder& ZIndex(int zIndex) {
			this->zIndex = zIndex;
			return *this;
		}

		TransformBuilder& Anchor(ofVec2f anchor) {
			this->anchor = anchor;
			return *this;
		}

		TransformBuilder& Anchor(float x, float y) {
			this->anchor = ofVec2f(x, y);
			return *this;
		}

		TransformBuilder& RotationCenter(ofVec2f centroid) {
			this->rotationCentroid = centroid;
			return *this;
		}

		TransformBuilder& RotationCenter(float x, float y) {
			this->rotationCentroid = ofVec2f(x, y);
			return *this;
		}

		TransformBuilder& Rotation(float rotation) {
			this->rotation = rotation;
			return *this;
		}

		TransformBuilder& AbsolutePosition(ofVec2f pos) {
			this->pos = pos;
			this->pType = CALCTYPE_ABS;
			return *this;
		}

		TransformBuilder& AbsolutePosition(float x, float y) {
			this->pos = ofVec2f(x, y);
			this->pType = CALCTYPE_ABS;
			return *this;
		}

		TransformBuilder& LocalPosition(ofVec2f pos) {
			this->pos = pos;
			this->pType = CALCTYPE_LOC;
			return *this;
		}

		TransformBuilder& LocalPosition(float x, float y) {
			this->pos = ofVec2f(x, y);
			this->pType = CALCTYPE_LOC;
			return *this;
		}

		TransformBuilder& CheckSceneScale(bool check) {
			this->checkSceneScale = check;
			return *this;
		}

		TransformBuilder& CheckNativeResolution(bool check) {
			this->checkNativeResolution = check;
			return *this;
		}

		TransformBuilder& RelativePosition(ofVec2f pos) {
			this->pos = pos;
			this->pType = CALCTYPE_PER;
			return *this;
		}

		TransformBuilder& RelativePosition(float x, float y) {
			this->pos = ofVec2f(x, y);
			this->pType = CALCTYPE_PER;
			return *this;
		}

		TransformBuilder& GridPosition(float x, float y, int gridWidth, int gridHeight) {
			this->pos = ofVec2f(x, y);
			this->gridWidth = gridWidth;
			this->gridHeight = gridHeight;
			this->pType = CALCTYPE_GRID;
			return *this;
		}

		TransformBuilder& AbsoluteScale(ofVec2f size) {
			this->size = size;
			this->sType = CALCTYPE_ABS;
			return *this;
		}

		TransformBuilder& LocalScale(ofVec2f size) {
			this->size = size;
			this->sType = CALCTYPE_LOC;
			return *this;
		}

		TransformBuilder& LocalScale(float x, float y) {
			this->size = ofVec2f(x, y);
			this->sType = CALCTYPE_LOC;
			return *this;
		}

		TransformBuilder& RelativeScale(ofVec2f size) {
			this->size = size;
			this->sType = CALCTYPE_PER;
			return *this;
		}

		TransformBuilder& RelativeScale(float x, float y) {
			this->size = ofVec2f(x, y);
			this->sType = CALCTYPE_PER;
			return *this;
		}

		TransformBuilder& GridScale(float width, float height, int gridWidth, int gridHeight) {
			this->size = ofVec2f(width, height);
			this->gridWidth = gridWidth;
			this->gridHeight = gridHeight;
			this->sType = CALCTYPE_GRID;
			return *this;
		}

		void Build(Node* element);

		/**
		 * Calculates a transformation relative to a custom parent
		 */
		void Build(Node* element, Node* parent);

		void BuildAndReset(Node* element);

		void Calculate(Trans& outputTrans, Node* element);

	private:
		void SetTransform(Node* element, Node* parent);

		void CalcTransform(Trans& outputTrans, Node* node, Node* parent);

		ofVec2f CalcPosition(Node* node, Node* parent, ofVec2f pos, stenum posCalc);

		ofVec3f CalcScale(Node* node, Node* parent, float width, float height, stenum scaleCalc);


		void SetDefaultVals() {
			pos = ofVec2f(0);
			zIndex = 1;
			pType = CALCTYPE_LOC;
			anchor = ofVec2f(0, 0);
			size = ofVec2f(1);
			rotationCentroid = ofVec2f(0.5f);
			sType = CALCTYPE_LOC;
			rotation = 0;
		}
	};

} // namespace