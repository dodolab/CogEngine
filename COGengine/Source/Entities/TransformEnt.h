#pragma once

#include "ofxCogCommon.h"
#include "DEntity.h"
#include "Settings.h"

namespace Cog {

	/**
	* Calculation type
	*/
	enum class CalcType {
		ABS,	/*!< absolute (un) */
		LOC,	/*!< local (-) */
		PER,	/*!< percentage <0..1> (r)*/
		GRID,	/*!< grid positioning (gr)*/
		ABS_PER /*!< absolute percentage (rp) */
	};

	struct CalcTypeConverter {
		static CalcType GetUnitValue(string val, float& floatVal) {
			stringstream ss;
			unsigned i;
			for (i = 0; i < val.length(); i++) {
				if (isdigit(val[i]) || val[i] == '.' || val[i] == '-') ss << val[i];
				else break;
			}

			if (i != 0) floatVal = ofToFloat(ss.str());
			return StrToCalcType(val.substr(i));
		}


		static CalcType StrToCalcType(string val) {
			if (val.compare("r") == 0) return CalcType::PER;
			else if (val.compare("gr") == 0) return CalcType::GRID;
			else if (val.compare("rp") == 0) return CalcType::ABS_PER;
			else if (val.compare("un") == 0) return CalcType::ABS;
			else if (val.empty()) return CalcType::LOC;

			CogLogError("CalcTypeConverter", "Error! %s is not valid unit, expected [r, gr, rp, un] or empty", val.c_str());

			return CalcType::LOC;
		}
	};

	class TransformEnt : public DEntity{
	public:

		TransformEnt() : pos(0), zIndex(0), pType(CalcType::PER), anchor(0), size(1), sType(CalcType::LOC), rotation(0), rotationCentroid(0.5f){
			this->name = "";
		}

		TransformEnt(string name) : TransformEnt() {
			this->name = name;
		}

		TransformEnt(string name, ofVec2f position, CalcType positionCalc, float rotation) : TransformEnt(){
			this->name = name;
			this->pos = position;
			this->pType = positionCalc;
			this->rotation = rotation;
		}

		TransformEnt(ofVec2f position, int zIndex, CalcType positionCalc,
			ofVec2f anchor, ofVec2f size, CalcType sizeCalc) :pos(position), zIndex(zIndex), pType(positionCalc), anchor(anchor),
			size(size), sType(sizeCalc), rotation(0), rotationCentroid(0.5f){
			this->name = "";
		}

		TransformEnt(string name, ofVec2f position, int zIndex, CalcType positionCalc,
			ofVec2f anchor, ofVec2f size, CalcType sizeCalc, float rotation):pos(position),zIndex(zIndex),pType(positionCalc),anchor(anchor),
			size(size),sType(sizeCalc), rotation(rotation), rotationCentroid(0.5f){
			this->name = name;
		}

		TransformEnt(string name, ofVec2f position, int zIndex, CalcType positionCalc,
			ofVec2f anchor, ofVec2f size, CalcType sizeCalc, float rotation, ofVec2f rotationOrigin) 
			:pos(position), zIndex(zIndex), pType(positionCalc), anchor(anchor),
			size(size), sType(sizeCalc), rotation(rotation), rotationCentroid(rotationOrigin) {
			this->name = name;
		}

		TransformEnt(const TransformEnt& copy) {
			this->name = copy.name;
			this->anchor = copy.anchor;
			this->pos = copy.pos;
			this->pType = copy.pType;
			this->rotation = copy.rotation;
			this->rotationCentroid = copy.rotationCentroid;
			this->size = copy.size;
			this->sType = copy.sType;
			this->zIndex = copy.zIndex;
		}

		~TransformEnt() {

		}

		ofVec2f pos;
		int zIndex;
		CalcType pType;
		ofVec2f anchor;
		ofVec2f size;
		ofVec2f rotationCentroid;
		CalcType sType;
		float rotation;

		void LoadFromXml(spt<ofxXml> xml, Setting& defaultSettings) {
			this->name = xml->getAttributex("name", "");

			pType = sType = CalcType::LOC;

			// =================== get positions
			if (xml->attributeExists("pos")) {
				string posString = xml->getAttributex("pos", "");
				float posF = defaultSettings.GetItemValFloat("pos", 0.0);
				pType = CalcTypeConverter::GetUnitValue(posString, posF);
				pos = ofVec2f(posF, posF);
			}
			else {
				string posXStr = xml->getAttributex("pos_x", "");
				string posYStr = xml->getAttributex("pos_y", "");

				float posX = defaultSettings.GetItemValFloat("pos_x", 0.0);
				float posY = defaultSettings.GetItemValFloat("pos_y", 0.0);

				if(!posXStr.empty()) pType = CalcTypeConverter::GetUnitValue(posXStr, posX);
				if(!posYStr.empty()) pType = CalcTypeConverter::GetUnitValue(posYStr, posY);

				pos = ofVec2f(posX, posY);
			}


			zIndex = xml->getAttributex("z_index", defaultSettings.GetItemValFloat("z_index", 0));
			
			// =================== get size
			
			float width = 0;
			float height = 0;

			if (xml->attributeExists("size")) {
				string sizeStr = xml->getAttributex("size", "1.0");
				float size = defaultSettings.GetItemValFloat("size", 1.0);
				sType = CalcTypeConverter::GetUnitValue(sizeStr, size);

				width = height = size;
			}
			else {
				string widthStr = xml->getAttributex("width", "");
				string heightStr = xml->getAttributex("height", "");

				width = defaultSettings.GetItemValFloat("width", 1.0);
				height = defaultSettings.GetItemValFloat("height", 1.0);

				// calc width and height
			 	if(!widthStr.empty()) sType = CalcTypeConverter::GetUnitValue(widthStr, width);
				if(!heightStr.empty()) sType = CalcTypeConverter::GetUnitValue(heightStr, height);

				// set the other property to 0 so that we know that it should have the same value
				if (width != height) {
					if (width != 1 && height == 1) height = 0;
					else if (height != 1 && width == 1) width = 0;
				}

			}

			size = ofVec2f(width, height);

			// =================== get anchor
			if (xml->attributeExists("anchor")) {
				float anchorFlt = xml->getAttributex("anchor", 0.0);
				anchor = ofVec2f(anchorFlt);
			}
			else {
				anchor.x = xml->getAttributex("anchor_x", defaultSettings.GetItemValFloat("anchor_x", 0.0));
				anchor.y = xml->getAttributex("anchor_y", defaultSettings.GetItemValFloat("anchor_y", 0.0));
			}

			rotation = xml->getAttributex("rotation", defaultSettings.GetItemValFloat("rotation", 0.0));

			if (xml->attributeExists("rotation_origin")) {
				float rotOrigin = xml->getAttributex("rotation_origin", 0.5);
				rotationCentroid = ofVec2f(rotOrigin);
			}
			else {
				rotationCentroid.x = xml->getAttributex("rotation_origin_x", defaultSettings.GetItemValFloat("rotation_origin_x", 0.0));
				rotationCentroid.y = xml->getAttributex("rotation_origin_y", defaultSettings.GetItemValFloat("rotation_origin_y", 0.0));
			}
		}

	};


}// namespace


