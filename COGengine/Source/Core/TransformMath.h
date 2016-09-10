#pragma once

#include "ofxCogMain.h"

namespace Cog {

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
		void SetSizeToScreen(Node* node, Node* parent) {
			SetTransform(node, parent, ofVec2f(0, 0), 1, CalcType::PER, ofVec2f(0, 0), ofVec2f(1, 0) // zero because we want to scale according to the x axis
				, CalcType::ABS_PER, 0, 0);
		}

		void SetTransform(Node* node, Node* parent, ofVec2f position, int zIndex, CalcType positionCalc, ofVec2f anchor, ofVec2f size, CalcType sizeCalc, int gridWidth = 0, int gridHeight = 0) {
			
			ofVec2f scrSize = CogGetScreenSize();

			Trans nodeTransform = Trans(0, 0);

			CalcTransform(nodeTransform, node, parent, position, zIndex, positionCalc, anchor, size, sizeCalc, gridWidth, gridHeight);

			// refresh transform (recalculate from parent)
			nodeTransform.CalcAbsTransform(parent->GetTransform());

			node->SetTransform(nodeTransform);
		}

		void CalcTransform(Trans& outputTrans, Node* node, Node* parent, ofVec2f position, int zIndex, CalcType positionCalc, 
			ofVec2f anchor, ofVec2f size, CalcType sizeCalc, int gridWidth = 0, int gridHeight = 0) {

			ofVec2f scrSize = CogGetScreenSize();

			Trans& parentTrans = parent->GetTransform();

			// calculate scale
			ofVec2f scale = CalcScale(node, parent, size.x, size.y, sizeCalc, gridWidth, gridHeight);
			// calculate position
			ofVec2f absPos = CalcPosition(node, parent, position, positionCalc, gridWidth, gridHeight);

			// fix position according to the anchor
			auto shape = node->GetShape();


			absPos.x += (0.0f - anchor.x) * node->GetShape()->GetWidth()*scale.x;
			absPos.y += (0.0f - anchor.y) * node->GetShape()->GetHeight()*scale.y;

			// if zIndex is equal to 0, the value will be taken from the parent
			if (zIndex == 0) zIndex = parentTrans.localPos.z;

			// set transformation
			outputTrans.localPos = ofVec3f(absPos.x, absPos.y, zIndex);
			outputTrans.scale = ofVec3f(scale.x, scale.y, 1);
		}

		ofVec2f GetCenter() {
			return ofVec2f(CogGetScreenWidth() / 2, CogGetScreenHeight() / 2);
		}


		ofVec2f CalcPosition(Node* node, Node* parent, ofVec2f pos, CalcType posCalc, int gridWidth = 0, int gridHeight = 0) {
			ofVec2f scrSize = CogGetScreenSize();
			Trans& parentTrans = parent->GetTransform();
			ofVec2f absPos;

			switch (posCalc) {
			case CalcType::ABS:
				// absolute position in device pixels
				absPos = ofVec2f((pos.x - parentTrans.absPos.x) / parentTrans.absScale.x, 
					(pos.y - parentTrans.absPos.y) / parentTrans.absScale.y);
				break;
			case CalcType::LOC:
				// local position, is scaled according to the parent absolute scale
				absPos = pos;
				break;
			case CalcType::ABS_PER:
				// absolute percentage -> screen size is 1.0 x 1.0
				absPos = ofVec2f((pos.x*scrSize.x - parentTrans.absPos.x) / parentTrans.absScale.x,
					(pos.y*scrSize.y - parentTrans.absPos.y) / parentTrans.absScale.y);
				break;
			case CalcType::PER:
				// relative percentage -> parent size is 1.0 x 1.0
				absPos = ofVec2f(pos.x*parent->GetShape()->GetWidth(),
					pos.y*parent->GetShape()->GetHeight());
				break;
			case CalcType::GRID:
				// grid percentage -> grid size must be specified
				float percentagePosx = pos.x / gridWidth;
				float percentagePosy = pos.y / gridHeight;
				absPos = ofVec2f((percentagePosx*scrSize.x - parentTrans.absPos.x) / parentTrans.absScale.x, 
					(percentagePosy*scrSize.y - parentTrans.absPos.y) / parentTrans.absScale.y);
				break;
			}

			return absPos;
		}


		ofVec3f CalcScale(Node* node, Node* parent, float width, float height, CalcType scaleCalc, int gridWidth = 0, int gridHeight = 0) {
			ofVec2f scrSize = CogGetScreenSize();
			Trans& parentTrans = parent->GetTransform();

			float scaleX = 1;
			float scaleY = 1;


			switch (scaleCalc) {
			case CalcType::ABS:
				// absolute scale
				scaleX = width / parentTrans.absScale.x;
				scaleY = height / parentTrans.absScale.y;
				break;
			case CalcType::LOC:
				// local scale, is multiplied by parent scale
				if (width == 0) width = 1;
				if (height == 0) height = 1;
				scaleX = width;
				scaleY = height;
				break;
			case CalcType::ABS_PER:
				// absolute percentage scale -> 1.0 x 1.0 will fit the whole screen
				scaleX = (width* scrSize.x / node->GetShape()->GetWidth()) / parentTrans.absScale.x;
				scaleY = (height* scrSize.y / node->GetShape()->GetHeight()) / parentTrans.absScale.y;
				break;
			case CalcType::PER:
				// relative percentage scale ->1.0 x 1.0 will fit the whole parent
				if (width == 0) width = 1;
				if (height == 0) height = 1;
				scaleX = (width * parent->GetShape()->GetWidth() / node->GetShape()->GetWidth());
				scaleY = (height * parent->GetShape()->GetHeight() / node->GetShape()->GetHeight());
				break;

			case CalcType::GRID:
				// grid scale -> todo, not tested yet :-)
				float percentageWidth = width / gridWidth;
				float percentageHeight = height / gridHeight;
				scaleX = (percentageWidth* scrSize.x / node->GetShape()->GetWidth()) / parentTrans.absScale.x;
				scaleY = (percentageHeight* scrSize.y / node->GetShape()->GetHeight()) / parentTrans.absScale.y;
				break;
			}

			if (width != 0 && height != 0) return ofVec3f(scaleX, scaleY, 1);
			else if (width == 0) return ofVec3f(scaleY, scaleY, 1);
			else if (height == 0)return ofVec3f(scaleX, scaleX, 1);

		}
		

		/**
		* Loads transformation from XML
		*/
		void LoadTransformFromXml(spt<ofxXml> xml, Node* node, Node* parent, Settings& settings) {
			TransformMath math = TransformMath();

			ofVec2f pos = ofVec2f();
			int zIndex = 0;
			CalcType posCalc = CalcType::PER;
			CalcType sizeCalc = CalcType::PER;
			ofVec2f anchor = ofVec2f();
			ofVec2f size = ofVec2f();

			// =================== get positions
			if (xml->attributeExists("pos")) {
				float posF = xml->getAttributex("pos", 0.0);
				pos = ofVec2f(posF, posF);
			}
			else {

				float posX = 0;
				float posY = 0;

				posX = xml->getAttributex("pos_x", 0.0);
				posY = xml->getAttributex("pos_y", 0.0);
				pos = ofVec2f(posX, posY);
			}


			zIndex = xml->getAttributex("z_index", 0);
			posCalc = StrToCalcType(xml->getAttributex("ptype", ""));
			
			// =================== get size
			sizeCalc = StrToCalcType(xml->getAttributex("stype", ""));

			// todo: for text, size has to be equal to 1 and stype must be loc !!

			float width = 0;
			float height = 0;

			if (xml->attributeExists("size")) {
				width = height = xml->getAttributex("size", 0.0);
			}
			else {
				width = xml->getAttributex("width", 0.0);
				height = xml->getAttributex("height", 0.0);
			}

			size = ofVec2f(width, height);

			// =================== get anchor
			if (xml->attributeExists("anchor")) {
				float anchorFlt = xml->getAttributex("anchor", 0.0);
				anchor = ofVec2f(anchorFlt);
			}
			else {
				anchor.x = xml->getAttributex("anchor_x", 0.0);
				anchor.y = xml->getAttributex("anchor_y", 0.0);
			}

			// =================== get grid size (if specified)
			int gridWidth = settings.GetSettingValInt("transform", "grid_width");
			int gridHeight = settings.GetSettingValInt("transform", "grid_height");

			// set transform according to the parsed values
			SetTransform(node, parent, pos, zIndex, posCalc, anchor, size, sizeCalc, gridWidth, gridHeight);
		}

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