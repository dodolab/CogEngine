#pragma once

#include "ofMain.h"
#include "CogEngine.h"


#ifdef TESTING
#else

#include "catch.hpp"
using namespace Cog;
#include "Flags.h"

	TEST_CASE("Flag test", "[class]")
	{
		
		SECTION("Simple state test")
		{
			Flags st = Flags();
			st.SetState(1);
			st.SetState(2);
			st.SetState(3);

			REQUIRE(st.HasState(1), st.HasState(2), st.HasState(3));
		}

		SECTION("Exceeding state test")
		{
			Flags st = Flags();
			st.SetState(1000);
			st.SetState(2000);
			st.SetState(3000);

			REQUIRE(st.HasState(1000), st.HasState(2000), st.HasState(3000));
		}

		SECTION("Reset state")
		{
			Flags st = Flags();
			st.SetState(1);
			st.SetState(2);
			st.SetState(3);
			st.ResetState(1);
			st.ResetState(2);
			st.ResetState(3);

			REQUIRE(!st.HasState(1), !st.HasState(2), !st.HasState(3));
		}

		SECTION("Exceeding reset")
		{
			Flags st = Flags();
			st.SetState(1000);
			st.SetState(2000);
			st.SetState(3000);
			st.ResetState(1000);
			st.ResetState(2000);
			st.ResetState(3000);

			REQUIRE(!st.HasState(1000), !st.HasState(2000), !st.HasState(3000));
		}

		SECTION("Comparing positive")
		{
			Flags st = Flags();
			Flags st2 = Flags();
			st.SetState(1000);
			st.SetState(2000);
			st.SetState(3000);
			st2.SetState(1000);


			REQUIRE(st.Contains(st2));
		}

		SECTION("Comparing positive 2")
		{
			Flags st = Flags();
			Flags st2 = Flags();
			st.SetState(1000);
			st.SetState(2000);
			st.SetState(3000);
			st2.SetState(1000);
			st2.SetState(2000);
			st2.SetState(3000);

			REQUIRE(st.Contains(st2));
		}

		SECTION("Comparing negative")
		{
			Flags st = Flags();
			Flags st2 = Flags();
			st.SetState(1000);
			st.SetState(2000);
			st.SetState(3000);
			st2.SetState(1000);
			st2.SetState(2000);
			st2.SetState(8000);

			REQUIRE(!st.Contains(st2));
		}

		SECTION("Switch states")
		{
			Flags st = Flags();
			st.SetState(1);
			st.SetState(2);

			st.SwitchState(1, 2);

			REQUIRE(st.HasState(1), st.HasState(2));
		}

		SECTION("Switch states 2")
		{
			Flags st = Flags();
			st.SetState(1);

			st.SwitchState(1, 2);

			REQUIRE(!st.HasState(1), st.HasState(2));
		}

		SECTION("Copy states")
		{
			Flags st = Flags();
			st.SetState(1);
			st.SetState(2);
			st.SetState(1000);
			st.SetState(2000);

			Flags st2 = st;

			REQUIRE(st2.HasState(1), st2.HasState(2), st2.HasState(1000), st2.HasState(2000));
		}

		SECTION("Get states")
		{
			Flags st = Flags();
			st.SetState(1);
			st.SetState(2);
			st.SetState(1000);
			st.SetState(2000);
			vector<unsigned> states = st.GetAllStates();

			REQUIRE(states.size() == 4, states[0] == 1, states[1] == 2, states[2] == 1000, states[3] == 2000);
		}
	}

#endif
	
