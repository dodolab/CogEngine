#pragma once

#include "ofMain.h"
#include "CogEngine.h"
#include "cpplinq.hpp"

#ifdef TESTING

#include "StringHash.h"

	TEST_CASE("StringHash test", "[class]")
	{
		
		SECTION("Comparing two hashes")
		{
			StringHash st1 = StringHash("MOJO1234");
			StringHash st2 = StringHash("MOJO1234");

			REQUIRE(st1 == st2);
		}

		SECTION("Comparing with number")
		{
			unsigned num = 12345;

			StringHash st1 = StringHash(num);

			REQUIRE(st1 == num);
		}

		SECTION("Adding numbers")
		{
			unsigned num = 6789;

			StringHash st1 = StringHash(12345);
			StringHash st2 = StringHash(num);
			st1 += st2;

			REQUIRE(st1 == (12345 + 6789));
		}
	}

#endif
	
