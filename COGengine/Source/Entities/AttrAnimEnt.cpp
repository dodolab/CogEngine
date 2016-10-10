#include "AttrAnimEnt.h"
#include "EnumConverter.h"

namespace Cog {

	void AttrAnimEnt::LoadFromXml(spt<ofxXml> xml, Setting& defaultSettings) {
		this->name = xml->getAttributex("name", "");
		this->attribute = xml->getAttributex("attr", "");

		if (this->attribute.empty()) throw IllegalArgumentException("Argument must be specified!");

		// if the "from" attribute contains "x", the value will be taken from animted object 
		if (xml->getAttributex("from", "x").compare("x") == 0) {
			takeInitialFromObject = true;
			fromVal = 0;
		}
		else {
			takeInitialFromObject = false;
			string fromValStr = xml->getAttributex("from", "");
			if (!fromValStr.empty()) this->calcType = EnumConverter::GetUnitValue(fromValStr, fromVal);
			else fromVal = 0;
		}

		string toValStr = xml->getAttributex("to", "");
		if (!toValStr.empty()) this->calcType = EnumConverter::GetUnitValue(toValStr, toVal);
		else toVal = 0;

		this->duration = xml->getAttributex("duration", 0);
		this->begin = xml->getAttributex("begin", 0);
		this->end = xml->getAttributex("end", 0);

		this->transType = EnumConverter::StrToMeasureType(xml->getAttributex("mtype", "direct"));
		this->attributeType = EnumConverter::StrToAttributeType(xml->getAttributex("attr", ""));

		RecalcDuration();

		// get easing function
		string easing = xml->getAttributex("easefunc", "");
		if (!easing.empty()) {
			this->fadeFunction = EnumConverter::StrToFadeFunction(easing);
		}
	}

} // namespace