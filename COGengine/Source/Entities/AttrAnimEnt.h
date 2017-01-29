#pragma once

#include "DEntity.h"
#include "Settings.h"
#include "TransformEnt.h"
#include "EasingFunc.h"

namespace Cog {

	/**
	* Type of transformation
	*/
	enum class TransformType {
		DIRECT,		/** direct change */
		DIFF		/** delta change */
	};

	/**
	* Type of attribute to animate
	*/
	enum class AttributeType {
		NONE,				/** not specified */
		POS_X,				/** position on the x axis */
		POS_Y,				/** position on the y axis */
		ROTATION,			/** rotation */
		ROT_CENTER_X,		/** rotation centroid on the x axis */
		ROT_CENTER_Y,		/** rotation centroid on the y axis */
		SIZE_X,				/** size on the x axis */
		SIZE_Y,				/** size on the y axis */
		SIZE				/** size in general */
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

		/**
		* Creates a new attribute animation entity
		* @param name entity name
		* @param attrType type of attribute to animate
		* @param fromVal starting value
		* @param toVal final value
		* @param calcType calculation type
		* @param begin beginning of the animation in ms (used when more than one animation run simultaneously), by default 0
		* @param end end of the animation in ms (used when more than one animation run simultaneously), by default 0
		* @param duration duration of the animation in ms
		*/
		AttrAnimEnt(string name, AttributeType attrType, float fromVal, float toVal, CalcType calcType, int begin, int end, int duration)
			:attributeType(attrType), fromVal(fromVal), toVal(toVal), calcType(calcType), begin(begin), end(end), duration(duration)
		{
			RecalcDuration();
		}

		~AttrAnimEnt() {

		}

		// attribute to animate in string
		string attribute = "";
		// type of attribute to animate (should be taken from the string above)
		AttributeType attributeType = AttributeType::NONE;
		// initial value of the animation
		float fromVal = 0;
		// multiplier value of initial animation (used when the initial value is taken from object)
		float fromValMult = 0;
		// final value of the animation
		float toVal = 0;
		// multiplier value of final animation (used when the final value is taken from object)
		float toValMult = 0;
		// if true, fromVal attribute is taken from actual state of the object
		bool takeInitialFromObject = false;
		// if true. toVal attribute is taken from actual state of the object
		bool takeFinalFromObject = false;
		// duration in ms
		int duration = 0;
		// start time of the animation in ms (needed when there is more than one animation at a time)
		int begin = 0;
		// end time of the animation in ms (needed when there is more than one animation at a time)
		int end = 0;
		// calculation type (local, relative etc.)
		CalcType calcType = CalcType::LOC;
		// transformation type 
		TransformType transType = TransformType::DIRECT;
		// fade function of the animation
		FadeFunction fadeFunction = nullptr;

		/**
		* Loads entity from xml
		*/
		void LoadFromXml(spt<ofxXml> xml, Setting& defaultSettings);

	private:

		/**
		* Recalculates time and duration due to each others' values
		*/
		void RecalcDuration() {
			if (this->end == 0) this->end = duration;
			if (this->duration == 0) this->duration = (end - begin);
		}

	};


}// namespace


