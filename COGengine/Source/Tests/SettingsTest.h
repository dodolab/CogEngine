#pragma once

#include "ofMain.h"
#include "ofxCogEngine.h"

#include "catch.hpp"
using namespace Cog;











TEST_CASE("Settings test")
{

	SECTION("Simple settings parser")
	{
#define SETTINGS "<setting name=\"test_settings\">\
		            <item key=\"test_key\" value=\"test_val\" />\
			      </setting>"

		// load xml from string
		spt<ofxXml> xml = spt<ofxXml>(new ofxXml());
		xml->loadFromBuffer(SETTINGS);

		auto settings = Settings();
		settings.LoadFromXml(xml);

		auto val = settings.GetSettingVal("test_settings", "test_key");

		REQUIRE(val.compare("test_val") == 0);
	}
}

