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
					<item key=\"test_int_key\" value=\"12345\" />\
			      </setting>"

		// load xml from string
		xml_document* xml = new xml_document();
		xml->load_string(SETTINGS);

		auto settings = Settings();
		settings.LoadFromXml(*xml);

		auto val = settings.GetSettingVal<string>("test_settings", "test_key");

		REQUIRE(val.compare("test_val") == 0);

		auto val2 = settings.GetSettingVal<int>("test_settings", "test_int_key");

		REQUIRE(val2 == 12345);
	}
}

