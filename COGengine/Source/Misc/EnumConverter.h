#pragma once

#include <string>
#include "Definitions.h"
using namespace std;

namespace Cog {

	enum class ShapeType;
	enum class TransformType;
	enum class AttributeType;
	enum class CalcType;

	struct EnumConverter {

		static ShapeType StrToShapeType(string val);

		/**
		* Transforms string into TransformType enum
		*/
		static TransformType StrToMeasureType(string val);

		/**
		* Transforms string into AttributeType enum
		*/
		static AttributeType StrToAttributeType(string val);

		static CalcType GetUnitValue(string val, float& floatVal);

		static CalcType StrToCalcType(string val);

		static FadeFunction GetFadeFunction(string name);
	};

} // namespace