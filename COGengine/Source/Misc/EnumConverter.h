#pragma once

#include <string>
#include "Definitions.h"
using namespace std;

#include "ofColor.h"

namespace Cog {

	enum class MeshType;
	enum class TransformType;
	enum class AttributeType;
	enum class CalcType;
	enum class AnimBlend;

	/**
	* Library class for converting between enumerated types and strings
	*/
	struct EnumConverter {

		/**
		* Converts string to MeshType
		*/
		static MeshType StrToMeshType(string val);

		/**
		* Converts string to MeasureType
		*/
		static TransformType StrToMeasureType(string val);

		/**
		* Converts string to AttributeType
		*/
		static AttributeType StrToAttributeType(string val);

		/**
		* Converts string to CalcType
		*/
		static CalcType StrToCalcType(string val);

		/**
		* Converts string to FadeFunction
		*/
		static FadeFunction StrToFadeFunction(string name);

		/**
		* Converts position into float value and calculation type
		* e.g. 150r is 150.0 with calculation type of PER
		*/
		static CalcType GetUnitValue(string val, float& floatVal);

		/**
		* Converts string to Color
		*/
		static ofColor StrToColor(string col);

		/**
		* Converts string into AnimBlend
		*/
		static AnimBlend StrToAnimBlend(string val);

	};

} // namespace