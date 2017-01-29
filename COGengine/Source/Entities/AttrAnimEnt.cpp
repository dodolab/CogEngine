#include "AttrAnimEnt.h"
#include "EnumConverter.h"

namespace Cog {

	void AttrAnimEnt::LoadFromXml(spt<ofxXml> xml, Setting& defaultSettings) {
		this->name = xml->getAttributex("name", "");
		this->attribute = xml->getAttributex("attr", "");

		if (this->attribute.empty()) throw IllegalArgumentException("Argument must be specified!");

		string fromStr = xml->getAttributex("from", "x");
		string toStr = xml->getAttributex("to", "x");

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