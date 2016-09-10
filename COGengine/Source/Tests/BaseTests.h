#pragma once

#include "ofMain.h"
#include "CogEngine.h"


#ifdef TESTING
#else

#include "catch.hpp"
using namespace Cog;
#include "TransformMath.h"

	TEST_CASE("Transformation test", "[class]")
	{
		COGEngine.Init();
		COGEngine.environment->OnScreenSizeChanged(100, 100);
		auto math = TransformMath();
		

		SECTION("Absolute position calculation")
		{
			Node* child = new Node("child");
			Node* parent = new Node("parent");
			ofVec2f pos = math.CalcPosition(child, parent, ofVec2f(50,50), CalcType::ABS, 0, 0);
			REQUIRE(pos.x == 50, pos.y == 50);
			delete parent;
			delete child;
		}

	}

#endif
	
