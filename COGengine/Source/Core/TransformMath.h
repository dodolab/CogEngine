#pragma once

#include "TransformEnt.h"

namespace Cog {

	class Node;
	class Settings;


	/**
	* Transformation helper
	*/
	class TransformMath  {

	private:


	public:
		/**
		* Sets size to fit the screen
		*/
		void SetSizeToScreen(Node* node, Node* parent);

		void SetTransform(Node* node, Node* parent, TransformEnt entity, int gridWidth = 0, int gridHeight = 0);

		void CalcTransform(Trans& outputTrans, Node* node, Node* parent, TransformEnt entity, int gridWidth = 0, int gridHeight = 0);

		ofVec2f GetCenter() {
			return ofVec2f(CogGetScreenWidth() / 2.0f, CogGetScreenHeight() / 2.0f);
		}


		ofVec2f CalcPosition(Node* node, Node* parent, ofVec2f pos, CalcType posCalc, int gridWidth = 0, int gridHeight = 0);


		ofVec3f CalcScale(Node* node, Node* parent, float width, float height, CalcType scaleCalc, int gridWidth = 0, int gridHeight = 0);
		
	};

}// namespace