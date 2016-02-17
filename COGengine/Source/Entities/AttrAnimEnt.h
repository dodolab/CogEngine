#pragma once

#include "ofxCogCommon.h"
#include "DEntity.h"
#include "Settings.h"
#include "TransformEnt.h"
#include "EasingFunc.h"

namespace Cog {

	enum class MeasureType {
		DIRECT,
		DIFF
	};

	enum class AttributeType {
		COMMON,
		POS_X,
		POS_Y,
		ROTATION,
		ROT_CENTER_X,
		ROT_CENTER_Y,
		SIZE_X,
		SIZE_Y,
		SIZE,
		SIZE_CENTER_X,
		SIZE_CENTER_Y
	};

	/**
	* Transformation animation
	*/
	class AttrAnimEnt : public DEntity{
	public:

		AttrAnimEnt() {
			this->name = "";
		}

		AttrAnimEnt(string name) {
			this->name = name;
		}

		AttrAnimEnt(string name, AttributeType attribute, float fromVal, float toVal, int duration, int repeat)
			:attributeType(attribute), fromVal(fromVal), toVal(toVal), duration(duration), repeat(repeat)
		{
			this->isInfinite = this->repeat == 0; // todo: dangerous !!
			this->hasValues = !this->attribute.empty(); // todo: dangerous !!
		}

		~AttrAnimEnt() {

		}

		string attribute = "";
		AttributeType attributeType = AttributeType::COMMON;
		float fromVal = 0;
		bool isFixed = false;
		float toVal = 0;
		int duration = 0;
		int repeat = 1;
		bool isInfinite = false;
		CalcType transformType = CalcType::ABS;
		MeasureType measureType = MeasureType::DIRECT;
		bool biDirectional = false;
		FadeFunction fadeFunction;
		bool inverted = false;
		bool hasValues = false;

		void LoadFromXml(spt<ofxXml> xml, Setting& defaultSettings) {
			this->name = xml->getAttributex("name", "");
			this->attribute = xml->getAttributex("attr","");

			this->hasValues = !this->attribute.empty();

			if (xml->getAttributex("from", "x").compare("x") == 0) {
				isFixed = true;
				fromVal = 0;
			}
			else {
				isFixed = false;
				this->fromVal = xml->getAttributex("from", 0.0f);
			}


			this->toVal = xml->getAttributex("to", 0.0f);
			this->duration = xml->getAttributex("duration", 0);
			this->repeat = xml->getAttributex("repeat", 1);
			this->isInfinite = this->repeat == 0;
			this->biDirectional = xml->getBoolAttributex("bidirect", false);
			this->inverted = xml->getBoolAttributex("inverted", false);

			this->transformType = StrToCalcType(xml->getAttributex("ttype", "abs"));
			this->measureType = StrToMeasureType(xml->getAttributex("mtype", "direct"));
			this->attributeType = StrToAttributeType(xml->getAttributex("attr", ""));
			
			string easing = xml->getAttributex("easefunc","");
			if (!easing.empty()) {
				this->fadeFunction = EasingManager::GetFadeFunction(easing);
			}
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

		MeasureType StrToMeasureType(string val) {
			if (val.compare("direct") == 0) return MeasureType::DIRECT;
			else if (val.compare("diff") == 0) return MeasureType::DIFF;

			return MeasureType::DIRECT;
		}

		AttributeType StrToAttributeType(string val) {
			if (val.compare("pos_x") == 0) return AttributeType::POS_X;
			else if (val.compare("pos_y") == 0) return AttributeType::POS_Y;
			else if (val.compare("rotation") == 0) return AttributeType::ROTATION;
			else if (val.compare("rot_center_x") == 0) return AttributeType::ROT_CENTER_X;
			else if (val.compare("rot_center_y") == 0) return AttributeType::ROT_CENTER_Y;
			else if (val.compare("size_x") == 0) return AttributeType::SIZE_X;
			else if (val.compare("size_y") == 0) return AttributeType::SIZE_Y;
			else if (val.compare("size") == 0) return AttributeType::SIZE;
			else if (val.compare("size_center_x") == 0) return AttributeType::SIZE_CENTER_X;
			else if (val.compare("size_center_y") == 0)return AttributeType::SIZE_CENTER_Y;

			return AttributeType::COMMON;
		}
	};


}// namespace


