#pragma once

#include "ofMain.h"
#include "ofxCogEngine.h"
#include "catch.hpp"
using namespace Cog;
#include "CoroutineContext.h"


TEST_CASE("Coroutine test")
{

	SECTION("Lambda coroutine")
	{
		CoroutineContext ctx;

		// declare some input object
		vector<int> samples;
		samples.push_back(1);
		samples.push_back(2);
		samples.push_back(3);

		int result = 0;

		ctx.scope = [samples, &result](CoroutineContext& ctx) {

			for (auto i : samples) {
				result += i;
			}
		};

		ctx.initialized = true;


		for (int i = 0; i < 10; i++) {
			ctx.CallCoroutine();
		}

		REQUIRE(result == 60);
	}
}
