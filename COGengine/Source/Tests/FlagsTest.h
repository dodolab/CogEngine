#pragma once

#include "ofMain.h"
#include "CogEngine.h"


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

			REQUIRE(st.HasState(1));
			REQUIRE(st.HasState(2)); 
			REQUIRE(st.HasState(3));
		}

		SECTION("Exceeding state test")
		{
			Flags st = Flags();
			st.SetState(1000);
			st.SetState(2000);
			st.SetState(3000);

			REQUIRE(st.HasState(1000));
			REQUIRE(st.HasState(2000));
			REQUIRE(st.HasState(3000));
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

			REQUIRE(!st.HasState(1));
			REQUIRE(!st.HasState(2));
			REQUIRE(!st.HasState(3));
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

			REQUIRE(!st.HasState(1000));
			REQUIRE(!st.HasState(2000));
			REQUIRE(!st.HasState(3000));
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

			REQUIRE(st.HasState(1));
			REQUIRE(st.HasState(2));
		}

		SECTION("Switch states 2")
		{
			Flags st = Flags();
			st.SetState(1);

			st.SwitchState(1, 2);

			REQUIRE(!st.HasState(1));
			REQUIRE(st.HasState(2));
		}

		SECTION("Copy states")
		{
			Flags st = Flags();
			st.SetState(1);
			st.SetState(2);
			st.SetState(1000);
			st.SetState(2000);

			Flags st2 = st;

			REQUIRE(st2.HasState(1));
			REQUIRE(st2.HasState(2)); 
			REQUIRE(st2.HasState(1000));
			REQUIRE(st2.HasState(2000));
		}

		SECTION("Get states")
		{
			Flags st = Flags();
			st.SetState(1);
			st.SetState(2);
			st.SetState(1000);
			st.SetState(2000);
			vector<unsigned> states = st.GetAllStates();

			REQUIRE(states.size() == 4);
			REQUIRE(states[0] == 1);
			REQUIRE(states[1] == 2);
			REQUIRE(states[2] == 1000);
			REQUIRE(states[3] == 2000);
		}

		SECTION("Overriding operator = int")
		{
			Flags st = 12;
			REQUIRE(st.HasState(12));
		}

		SECTION("Overriding operator = StrId")
		{
			unsigned mojo = StrId("MOJO");
			Flags st = StrId("MOJO");
			REQUIRE(st.HasState(mojo));
		}

		SECTION("Overriding operator ==Flags")
		{
			Flags st1 = Flags(12);
			st1.SetState(123);
			st1.SetState(333);

			Flags st2 = Flags(12);
			st2.SetState(333);
			st2.SetState(123);

			REQUIRE(st1 == st2);
		}

		SECTION("Overriding operator ==int")
		{
			Flags st1 = Flags(12);

			REQUIRE(st1 == 12);
		}

		SECTION("Overriding operator +")
		{
			Flags st1 = Flags(12);
			Flags st3 = st1 + 13;

			REQUIRE(st3.HasState(12));
			REQUIRE(st3.HasState(13));
		}

		SECTION("Overriding operator +=")
		{
			Flags st1 = Flags(12);
			st1 += 13;

			REQUIRE(st1.HasState(12));
			REQUIRE(st1.HasState(13));
		}
	}
