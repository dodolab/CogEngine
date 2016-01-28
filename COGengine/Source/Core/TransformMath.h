#pragma once

#include "ofxCogCommon.h"

namespace Cog {

	class Node;
	class Settings;

	/**
	* Calculation type
	*/
	enum class CalcType {
		ABS,	/*!< absolute */
		LOC,	/*!< local */
		PER,	/*!< percentage <0..1> */
		GRID,	/*!< grid positioning */
		ABS_PER /*!< absolute percentage */
	};

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

		void SetTransform(Node* node, Node* parent, ofVec2f position, int zIndex, CalcType positionCalc, ofVec2f anchor, ofVec2f size, CalcType sizeCalc, int gridWidth = 0, int gridHeight = 0);

		void CalcTransform(Trans& outputTrans, Node* node, Node* parent, ofVec2f position, int zIndex, CalcType positionCalc,
			ofVec2f anchor, ofVec2f size, CalcType sizeCalc, int gridWidth = 0, int gridHeight = 0);

		ofVec2f GetCenter() {
			return ofVec2f(CogGetScreenWidth() / 2.0f, CogGetScreenHeight() / 2.0f);
		}


		ofVec2f CalcPosition(Node* node, Node* parent, ofVec2f pos, CalcType posCalc, int gridWidth = 0, int gridHeight = 0);


		ofVec3f CalcScale(Node* node, Node* parent, float width, float height, CalcType scaleCalc, int gridWidth = 0, int gridHeight = 0);
		

		/**
		* Loads transformation from XML
		*/
		void LoadTransformFromXml(spt<ofxXml> xml, Node* node, Node* parent, Settings& settings);

		private:
			CalcType StrToCalcType(string val) {
				if (val.compare("per") == 0) return CalcType::PER;
				else if (val.compare("abs") == 0) return CalcType::ABS;
				else if (val.compare("grid") == 0) return CalcType::GRID;
				else if (val.compare("absper") == 0) return CalcType::ABS_PER;
				else if (val.compare("loc") == 0) return CalcType::LOC;

				return CalcType::PER;
			}
	};

}// namespace