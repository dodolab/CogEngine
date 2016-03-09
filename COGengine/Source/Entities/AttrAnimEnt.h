#pragma once

#include "ofxCogCommon.h"
#include "DEntity.h"
#include "Settings.h"
#include "TransformEnt.h"
#include "EasingFunc.h"

namespace Cog {

	/**
	* Type of transformation measure
	*/
	enum class MeasureType {
		DIRECT,		/*!< direct change */
		DIFF		/*!< difference */
	};

	/**
	* Attribute to animate
	*/
	enum class AttributeType {
		COMMON,				/*!< not specified */
		POS_X,				/*!< position on the x axis */
		POS_Y,				/*!< position on the y axis */
		ROTATION,			/*!< rotation */
		ROT_CENTER_X,		/*!< rotation centroid on the x axis */
		ROT_CENTER_Y,		/*!< rotation centroid on the y axis */
		SIZE_X,				/*!< size on the x axis */
		SIZE_Y,				/*!< size on the y axis */
		SIZE,				/*!< size overall */
		SIZE_CENTER_X,		/*!< size centroid on the x axis */
		SIZE_CENTER_Y		/*!< size centroid on the y axis */
	};

	/**
	* Attribute animation entity
	*/
	class AttrAnimEnt : public DEntity{
	public:

		AttrAnimEnt() {
			this->name = "";
		}

		AttrAnimEnt(string name) {
			this->name = name;
		}

		AttrAnimEnt(string name, AttributeType attribute, float fromVal, float toVal, int begin, int end, int duration, int repeat)
			:attributeType(attribute), fromVal(fromVal), toVal(toVal), begin(begin), end(end), duration(duration)
		{
			RecalcDuration();
		}

		~AttrAnimEnt() {

		}

		// attribute to animate
		string attribute = "";
		// type of attribute to animate (should be taken from the string above)
		AttributeType attributeType = AttributeType::COMMON;
		// initial value of the animation
		float fromVal = 0;
		// final value of the animation
		float toVal = 0;
		// if true, the FROM value is taken from actual state of the object
		bool fromValFromActual = false;
		// duration in ms
		int duration = 0;
		// begin of the animation in ms (needed when there is more than one animation at a time)
		int begin = 0;
		// end of the animation in ms (needed when there is more than one animation at a time)
		int end = 0;
		// transformation type
		CalcType transformType = CalcType::LOC;
		// measure type 
		MeasureType measureType = MeasureType::DIRECT;
		// fade function of the animation
		FadeFunction fadeFunction = nullptr;


		void LoadFromXml(spt<ofxXml> xml, Setting& defaultSettings) {
			this->name = xml->getAttributex("name", "");
			this->attribute = xml->getAttributex("attr","");

			if (this->attribute.empty()) throw IllegalArgumentException("Argument must be specified!");

			if (xml->getAttributex("from", "x").compare("x") == 0) {
				fromValFromActual = true;
				fromVal = 0;
			}
			else {
				fromValFromActual = false;
				string fromValStr = xml->getAttributex("from", "");
				if(!fromValStr.empty()) this->transformType = CalcTypeConverter::GetUnitValue(fromValStr, fromVal);
				else fromVal = 0;
			}

			string toValStr = xml->getAttributex("to", "");
			if(!toValStr.empty()) this->transformType = CalcTypeConverter::GetUnitValue(toValStr, toVal);
			else toVal = 0;

			this->duration = xml->getAttributex("duration", 0);
			this->begin = xml->getAttributex("begin", 0);
			this->end = xml->getAttributex("end", 0);

			this->measureType = StrToMeasureType(xml->getAttributex("mtype", "direct"));
			this->attributeType = StrToAttributeType(xml->getAttributex("attr", ""));
			
			RecalcDuration();

			string easing = xml->getAttributex("easefunc","");
			if (!easing.empty()) {
				this->fadeFunction = EasingManager::GetFadeFunction(easing);
			}
		}

	private:

		void RecalcDuration() {
			// recalculate duration according to the beginning and end
			if (this->end == 0) this->end = duration;
			if (this->duration == 0) this->duration = (end - begin);
		}

		/**
		* Transforms string into MeasureType enum
		*/
		MeasureType StrToMeasureType(string val) {
			if (val.compare("direct") == 0) return MeasureType::DIRECT;
			else if (val.compare("diff") == 0) return MeasureType::DIFF;

			return MeasureType::DIRECT;
		}

		/**
		* Transforms string into AttributeType enum
		*/
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


