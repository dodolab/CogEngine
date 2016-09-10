#pragma once

#include "ofMain.h"
#include "CogEngine.h"


#include "catch.hpp"
using namespace Cog;
#include "StrId.h"












TEST_CASE("StrId")
{

	SECTION("Comparing two hashes")
	{
		StrId st1 = StrId("MOJO1234");
		StrId st2 = StrId("MOJO1234");

		REQUIRE(st1 == st2);
	}

	SECTION("Comparing with number")
	{
		unsigned num = 12345;

		StrId st1 = StrId(num);

		REQUIRE(st1 == num);
	}

	SECTION("Adding numbers")
	{
		unsigned num = 6789;

		StrId st1 = StrId(12345);
		StrId st2 = StrId(num);
		st1 += st2;

		REQUIRE(st1 == (12345 + 6789));
	}
}
