#pragma once

#include "ofMain.h"
#include "CogEngine.h"

#include "catch.hpp"
using namespace Cog;
#include "TimeMeasure.h"







TEST_CASE("Measuring test")
{

	SECTION("Simple measure")
	{
		COGMEASURE_BEGIN("Outer block");

		for (int i = 0; i < 1000000; i++) {
			if (i % 100 == 0) {
				COGMEASURE_BEGIN("Inner block");
				// make pause
				for (int j = 0; j < 10000; j++) {

				}

				COGMEASURE_END("Inner block");
			}
		}

		COGMEASURE_END("Outer block");

		auto outer = TimeMeasure::GetInstance().GetMeasureScope("Outer block");
		auto inner = TimeMeasure::GetInstance().GetMeasureScope("Inner block");

		REQUIRE(outer.duration > 0);
		REQUIRE(inner.duration > 0);
		REQUIRE(inner.numCalls == 10000);
		REQUIRE(outer.numCalls == 1);

	}
}
