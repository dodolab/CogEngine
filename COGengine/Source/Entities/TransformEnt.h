#pragma once

#include "ofxCogCommon.h"
#include "DEntity.h"
#include "Settings.h"

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


	class TransformEnt : public DEntity{
	public:

		TransformEnt() : pos(0), zIndex(0), pType(CalcType::PER), anchor(0), size(0), sType(CalcType::PER), rotation(0) {

		}

		TransformEnt(ofVec2f position, int zIndex, CalcType positionCalc,
			ofVec2f anchor, ofVec2f size, CalcType sizeCalc):pos(position),zIndex(zIndex),pType(positionCalc),anchor(anchor),
			size(size),sType(sizeCalc), rotation(0){
		}

		ofVec2f pos;
		int zIndex;
		CalcType pType;
		ofVec2f anchor;
		ofVec2f size;
		CalcType sType;
		float rotation;

		void LoadFromXml(spt<ofxXml> xml, Setting& defaultSettings) {
			this->name = xml->getAttributex("name", "");

			// =================== get positions
			if (xml->attributeExists("pos")) {
				float posF = xml->getAttributex("pos", defaultSettings.GetItemValFloat("pos",0));
				pos = ofVec2f(posF, posF);
			}
			else {

				float posX = 0;
				float posY = 0;

				posX = xml->getAttributex("pos_x", defaultSettings.GetItemValFloat("pos_x", 0));
				posY = xml->getAttributex("pos_y", defaultSettings.GetItemValFloat("pos_y", 0));
				pos = ofVec2f(posX, posY);
			}


			zIndex = xml->getAttributex("z_index", defaultSettings.GetItemValFloat("z_index", 0));
			pType = StrToCalcType(xml->getAttributex("ptype", defaultSettings.GetItemVal("ptype", "per")));

			// =================== get size
			sType = StrToCalcType(xml->getAttributex("stype", defaultSettings.GetItemVal("stype", "loc")));

			float width = 0;
			float height = 0;

			if (xml->attributeExists("size")) {
				width = height = xml->getAttributex("size", 0.0);
			}
			else {
				width = xml->getAttributex("width", defaultSettings.GetItemValFloat("width", 1));
				height = xml->getAttributex("height", defaultSettings.GetItemValFloat("height", 1));
			}

			size = ofVec2f(width, height);

			// =================== get anchor
			if (xml->attributeExists("anchor")) {
				float anchorFlt = xml->getAttributex("anchor", 0.0);
				anchor = ofVec2f(anchorFlt);
			}
			else {
				anchor.x = xml->getAttributex("anchor_x", defaultSettings.GetItemValFloat("anchor_x", 0));
				anchor.y = xml->getAttributex("anchor_y", defaultSettings.GetItemValFloat("anchor_y", 0));
			}

			rotation = xml->getAttributex("rotation", defaultSettings.GetItemValFloat("rotation", 0));
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


