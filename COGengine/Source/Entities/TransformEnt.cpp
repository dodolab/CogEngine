#include "TransformEnt.h"
#include "EnumConverter.h"

namespace Cog {

	void TransformEnt::LoadFromXml(xml_node& xml, Setting& defaultSettings) {

		this->name = xml.attribute("name").as_string();

		// calculation type is LOCAL by default
		pType = sType = CalcType::LOC;

		// =================== get positions
		auto posAttrib = xml.attribute("pos");
		if (posAttrib) {
			string posString = posAttrib.as_string();
			float posF = defaultSettings.GetItemValFloat("pos", 0.0);
			pType = EnumConverter::GetUnitValue(posString, posF);
			pos = ofVec2f(posF, posF);
		}
		else {
			string posXStr = xml.attribute("pos_x").as_string();
			string posYStr = xml.attribute("pos_y").as_string();

			float posX = defaultSettings.GetItemValFloat("pos_x", 0.0);
			float posY = defaultSettings.GetItemValFloat("pos_y", 0.0);

			if (!posXStr.empty()) pType = EnumConverter::GetUnitValue(posXStr, posX);
			if (!posYStr.empty()) pType = EnumConverter::GetUnitValue(posYStr, posY);

			pos = ofVec2f(posX, posY);
		}


		zIndex = xml.attribute("z_index").as_int(defaultSettings.GetItemValFloat("z_index", 0));

		// =================== get size

		float width = 0;
		float height = 0;

		auto sizeAttr = xml.attribute("size");
		if (sizeAttr) {
			string sizeStr = sizeAttr.as_string("1.0");
			float size = defaultSettings.GetItemValFloat("size", 1.0);
			sType = EnumConverter::GetUnitValue(sizeStr, size);

			width = height = size;
		}
		else {
			string widthStr = xml.attribute("width").as_string();
			string heightStr = xml.attribute("height").as_string();

			width = defaultSettings.GetItemValFloat("width", 0.0);
			height = defaultSettings.GetItemValFloat("height", 0.0);

			// calc width and height
			if (!widthStr.empty()) sType = EnumConverter::GetUnitValue(widthStr, width);
			// 0 means that the other value will be taken from aspect ratio multiplied by the width
			if (!heightStr.empty()) {
				if (heightStr.compare("0") != 0) {
					// calculate scaleType from height (it may be different)
					sType = EnumConverter::GetUnitValue(heightStr, height);
				}
				else {
					height = 0;
				}
			}

			// set the other property to 0 so that we know that it should have the same value
			if (width != height) {
				if (width != 0 && height == 1) height = 0;
				else if (height != 0 && width == 1) width = 0;
			}

		}

		size = ofVec2f(width, height);

		// =================== get anchor
		auto anchorAttr = xml.attribute("anchor");
		if (anchorAttr) {
			float anchorFlt = anchorAttr.as_float(0.0f);
			anchor = ofVec2f(anchorFlt);
		}
		else {
			anchor.x = xml.attribute("anchor_x").as_float(defaultSettings.GetItemValFloat("anchor_x", 0.0));
			anchor.y = xml.attribute("anchor_y").as_float(defaultSettings.GetItemValFloat("anchor_y", 0.0));
		}

		// get rotation and centroid

		rotation = xml.attribute("rotation").as_float(defaultSettings.GetItemValFloat("rotation", 0.0));

		auto rotOrigAttr = xml.attribute("rotation_origin");
		if (rotOrigAttr) {
			float rotOrigin = rotOrigAttr.as_float(0.5f);
			rotationCentroid = ofVec2f(rotOrigin);
		}
		else {
			rotationCentroid.x = xml.attribute("rotation_origin_x").as_float(defaultSettings.GetItemValFloat("rotation_origin_x", 0.0));
			rotationCentroid.y = xml.attribute("rotation_origin_y").as_float(defaultSettings.GetItemValFloat("rotation_origin_y", 0.0));
		}
	}

} // namespace