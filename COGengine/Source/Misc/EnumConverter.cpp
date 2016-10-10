#include "EnumConverter.h"
#include "Mesh.h"
#include "AttrAnimEnt.h"
#include "TransformEnt.h"
#include "EasingFunc.h"
#include "TransformAnim.h"
#include "Facade.h"
#include "Tween.h"

namespace Cog {

	MeshType EnumConverter::StrToMeshType(string val) {
		std::transform(val.begin(), val.end(), val.begin(), ::tolower);

		if (val.compare("image") == 0) {
			return MeshType::IMAGE;
		}
		else if (val.compare("rectangle") == 0) {
			return MeshType::RECTANGLE;
		}
		else if (val.compare("text") == 0) {
			return MeshType::TEXT;
		}
		else if (val.compare("plane") == 0) {
			return MeshType::PLANE;
		}
		else if (val.compare("sprite") == 0) {
			return MeshType::SPRITE;
		}
		else if (val.compare("multisprite") == 0) {
			return MeshType::MULTISPRITE;
		}
		else if (val.compare("label") == 0) {
			return MeshType::LABEL;
		}
		else if (val.compare("bounding_box") == 0) {
			return MeshType::BOUNDING_BOX;
		}

		return MeshType::NONE;
	}

	TransformType EnumConverter::StrToMeasureType(string val) {
		std::transform(val.begin(), val.end(), val.begin(), ::tolower);

		if (val.compare("direct") == 0) return TransformType::DIRECT;
		else if (val.compare("diff") == 0) return TransformType::DIFF;

		return TransformType::DIRECT;
	}

	AttributeType EnumConverter::StrToAttributeType(string val) {
		std::transform(val.begin(), val.end(), val.begin(), ::tolower);

		if (val.compare("pos_x") == 0) return AttributeType::POS_X;
		else if (val.compare("pos_y") == 0) return AttributeType::POS_Y;
		else if (val.compare("rotation") == 0) return AttributeType::ROTATION;
		else if (val.compare("rot_center_x") == 0) return AttributeType::ROT_CENTER_X;
		else if (val.compare("rot_center_y") == 0) return AttributeType::ROT_CENTER_Y;
		else if (val.compare("size_x") == 0) return AttributeType::SIZE_X;
		else if (val.compare("size_y") == 0) return AttributeType::SIZE_Y;
		else if (val.compare("size") == 0) return AttributeType::SIZE;
		
		return AttributeType::NONE;
	}

	CalcType EnumConverter::StrToCalcType(string val) {
		std::transform(val.begin(), val.end(), val.begin(), ::tolower);

		if (val.compare("r") == 0) return CalcType::PER;
		else if (val.compare("gr") == 0) return CalcType::GRID;
		else if (val.compare("rp") == 0) return CalcType::ABS_PER;
		else if (val.compare("un") == 0) return CalcType::ABS;
		else if (val.empty()) return CalcType::LOC;

		CogLogError("EnumConverter", "Error! %s is not valid unit, expected [r, gr, rp, un] or empty", val.c_str());

		return CalcType::LOC;
	}


	FadeFunction EnumConverter::StrToFadeFunction(string name) {
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);

		if (name.compare("linear") == 0) {
			return EasingFunc::linear;
		}
		if (name.compare("elastic_in") == 0) {
			return EasingFunc::elasticIn;
		}
		if (name.compare("elastic_out") == 0) {
			return EasingFunc::elasticOut;
		}
		if (name.compare("elastic") == 0) {
			return EasingFunc::elasticInOut;
		}
		if (name.compare("expo_in") == 0) {
			return EasingFunc::expoIn;
		}
		if (name.compare("expo_out") == 0) {
			return EasingFunc::expoOut;
		}
		if (name.compare("expo") == 0) {
			return EasingFunc::expoInOut;
		}
		if (name.compare("quad_in") == 0) {
			return EasingFunc::quadIn;
		}
		if (name.compare("quad_out") == 0) {
			return EasingFunc::quadOut;
		}
		if (name.compare("quad") == 0) {
			return EasingFunc::quadInOut;
		}
		if (name.compare("cosine_in") == 0) {
			return EasingFunc::cosineIn;
		}
		if (name.compare("cosine_out") == 0) {
			return EasingFunc::cosineOut;
		}
		if (name.compare("cosine") == 0) {
			return EasingFunc::cosineInOut;
		}
		if (name.compare("sin_in") == 0) {
			return EasingFunc::sinIn;
		}
		if (name.compare("sin_out") == 0) {
			return EasingFunc::sinOut;
		}
		if (name.compare("sin") == 0) {
			return EasingFunc::sinInOut;
		}

		CogLogError("EnumConverter","No such easing function : %s", name.c_str());
		return EasingFunc::linear;
	}

	CalcType EnumConverter::GetUnitValue(string val, float& floatVal) {
		std::transform(val.begin(), val.end(), val.begin(), ::tolower);

		stringstream ss;
		unsigned i;
		for (i = 0; i < val.length(); i++) {
			if (isdigit(val[i]) || val[i] == '.' || val[i] == '-') ss << val[i];
			else break;
		}

		if (i != 0) floatVal = ofToFloat(ss.str());
		return StrToCalcType(val.substr(i));
	}

	ofColor EnumConverter::StrToColor(string col) {
		std::transform(col.begin(), col.end(), col.begin(), ::tolower);

		int hexColor = ofHexToInt(col.substr(2));
		float alpha = 255;

		if (col.length() == 10) {
			// get alpha
			alpha = ofHexToInt(col.substr(8));
		}

		ofColor color = ofColor::fromHex(hexColor, alpha);
		return color;
	}

	AnimBlend EnumConverter::StrToAnimBlend(string val) {
		std::transform(val.begin(), val.end(), val.begin(), ::tolower);

		if (val.compare("additive") == 0) return AnimBlend::ADDITIVE;
		else if (val.compare("overlay") == 0) return AnimBlend::OVERLAY;

		return AnimBlend::ADDITIVE;
	}

	TweenDirection EnumConverter::StrToTweenDirection(string val) {
		std::transform(val.begin(), val.end(), val.begin(), ::tolower);

		if (val.compare("none") == 0) return TweenDirection::NONE;
		else if (val.compare("left") == 0) return TweenDirection::LEFT;
		else if (val.compare("right") == 0) return TweenDirection::RIGHT;
		else if (val.compare("up") == 0) return TweenDirection::UP;
		else if (val.compare("down") == 0) return TweenDirection::DOWN;

		return TweenDirection::NONE;
	}

} // namespace