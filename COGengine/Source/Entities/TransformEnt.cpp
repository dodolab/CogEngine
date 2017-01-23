#include "TransformEnt.h"
#include "EnumConverter.h"

namespace Cog {

	void TransformEnt::LoadFromXml(spt<ofxXml> xml, Setting& defaultSettings) {

		this->name = xml->getAttributex("name", "");

		// calculation type is LOCAL by default
		pType = sType = CalcType::LOC;

		// =================== get positions
		if (xml->attributeExists("pos")) {
			string posString = xml->getAttributex("pos", "");
			float posF = defaultSettings.GetItemValFloat("pos", 0.0);
			pType = EnumConverter::GetUnitValue(posString, posF);
			pos = ofVec2f(posF, posF);
		}
		else {
			string posXStr = xml->getAttributex("pos_x", "");
			string posYStr = xml->getAttributex("pos_y", "");

			float posX = defaultSettings.GetItemValFloat("pos_x", 0.0);
			float posY = defaultSettings.GetItemValFloat("pos_y", 0.0);

			if (!posXStr.empty()) pType = EnumConverter::GetUnitValue(posXStr, posX);
			if (!posYStr.empty()) pType = EnumConverter::GetUnitValue(posYStr, posY);

			pos = ofVec2f(posX, posY);
		}


		zIndex = xml->getAttributex("z_index", defaultSettings.GetItemValFloat("z_index", 0));

		// =================== get size

		float width = 0;
		float height = 0;

		if (xml->attributeExists("size")) {
			string sizeStr = xml->getAttributex("size", "1.0");
			float size = defaultSettings.GetItemValFloat("size", 1.0);
			sType = EnumConverter::GetUnitValue(sizeStr, size);

			width = height = size;
		}
		else {
			string widthStr = xml->getAttributex("width", "");
			string heightStr = xml->getAttributex("height", "");

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
		if (xml->attributeExists("anchor")) {
			float anchorFlt = xml->getAttributex("anchor", 0.0);
			anchor = ofVec2f(anchorFlt);
		}
		else {
			anchor.x = xml->getAttributex("anchor_x", defaultSettings.GetItemValFloat("anchor_x", 0.0));
			anchor.y = xml->getAttributex("anchor_y", defaultSettings.GetItemValFloat("anchor_y", 0.0));
		}

		// get rotation and centroid

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

} // namespace