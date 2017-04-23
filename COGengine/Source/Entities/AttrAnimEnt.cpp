#include "AttrAnimEnt.h"
#include "EnumConverter.h"

namespace Cog {

	void AttrAnimEnt::LoadFromXml(xml_node& xml, Setting& defaultSettings) {
		this->name = xml.attribute("name").as_string();
		this->attribute = xml.attribute("attr").as_string();

		if (this->attribute.empty()) throw IllegalArgumentException("Argument must be specified!");

		string fromStr = xml.attribute("from").as_string("x");
		string toStr = xml.attribute("to").as_string("x");

		// if the "from" attribute contains "x", the value will be taken from animted object 
		if (fromStr.find("x") != string::npos) {
			takeInitialFromObject = true;
			fromVal = 0;
			string fromValMultStr = fromStr.substr(0, fromStr.find("x"));
			if (!fromValMultStr.empty()) {
				// take initial multiplier value
				if (fromValMultStr.compare("-") == 0) {
					fromValMult = -1;
				}
				else {
					fromValMult = ofToFloat(fromValMultStr);
				}
			}
		}
		else {
			takeInitialFromObject = false;
			if (!fromStr.empty()) this->calcType = EnumConverter::GetUnitValue(fromStr, fromVal);
			else fromVal = 0;
		}


		// if the "to" attribute contains "x", the value will be taken from animted object 
		if (toStr.find("x") != string::npos) {
			takeFinalFromObject = true;
			toVal = 0;
			string toValMultStr = toStr.substr(0, toStr.find("x"));
			if (!toValMultStr.empty()) {
				// take initial multiplier value
				if (toValMultStr.compare("-") == 0) {
					toValMult = -1;
				}
				else {
					toValMult = ofToFloat(toValMultStr);
				}
			}
		}
		else {
			takeFinalFromObject = false;
			if (!toStr.empty()) this->calcType = EnumConverter::GetUnitValue(toStr, toVal);
			else toVal = 0;
		}

		this->duration = xml.attribute("duration").as_int(0);
		this->begin = xml.attribute("begin").as_int(0);
		this->end = xml.attribute("end").as_int(0);

		this->transType = EnumConverter::StrToMeasureType(xml.attribute("mtype").as_string("direct"));
		this->attributeType = EnumConverter::StrToAttributeType(xml.attribute("attr").as_string(""));

		RecalcDuration();

		// get easing function
		string easing = xml.attribute("easefunc").as_string();
		if (!easing.empty()) {
			this->fadeFunction = EnumConverter::StrToFadeFunction(easing);
		}
	}

} // namespace