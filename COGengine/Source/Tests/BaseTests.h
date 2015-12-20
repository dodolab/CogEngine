#pragma once

#include "ofMain.h"
#include "CogEngine.h"


#ifdef TESTING
#else

class DummyShape : public Shape {
public:
	int width;
	int height;

	DummyShape(int width, int height) : width(width), height(height) {

	}

	float GetWidth() {
		return width;
	}

	float GetHeight() {
		return height;
	}
};


#include "catch.hpp"
using namespace Cog;
#include "TransformMath.h"

	TEST_CASE("Transformation test", "[class]")
	{
		COGEngine.Init();
		COGEngine.environment->SetScreenSizeHard(ofVec2f(100, 100));
		auto math = TransformMath();
		

		SECTION("Topleft corner")
		{
			Node* child = new Node("child");
			child->SetShape(spt<DummyShape>(new DummyShape(50, 50)));
			Node* parent = new Node("parent");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, child, parent, 
				ofVec2f(0, 0),  // position
				0, 
				CalcType::ABS,	// pos type
				ofVec2f(0, 0),  // anchor
				ofVec2f(0, 0),	// size 
				CalcType::ABS,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(parent->GetTransform());
			REQUIRE(((int)tr.absPos.x) == 0, ((int)tr.absPos.y) == 0);
			delete parent;
			delete child;
		}

		SECTION("Center")
		{
			Node* child = new Node("child");
			child->SetShape(spt<DummyShape>(new DummyShape(50, 50)));
			Node* parent = new Node("parent");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, child, parent,
				ofVec2f(0.5f, 0.5f),  // position
				0,
				CalcType::ABS_PER,	// pos type
				ofVec2f(0, 0),  // anchor
				ofVec2f(0, 0),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(parent->GetTransform());
			REQUIRE(((int)tr.absPos.x) == 50, ((int)tr.absPos.y) == 50);
			delete parent;
			delete child;
		}

		SECTION("Center centered")
		{
			Node* child = new Node("child");
			child->SetShape(spt<DummyShape>(new DummyShape(50, 50)));
			Node* parent = new Node("parent");
			Trans tr = Trans(0, 0);
			math.CalcTransform(tr, child, parent,
				ofVec2f(0.5f, 0.5f),  // position
				0,
				CalcType::ABS_PER,	// pos type
				ofVec2f(0.5f, 0.5f),  // anchor
				ofVec2f(0, 0),	// size 
				CalcType::LOC,	// scale type
				0, 0);			// grid size
			tr.CalcAbsTransform(parent->GetTransform());
			REQUIRE(((int)tr.absPos.x) == 25, ((int)tr.absPos.y) == 25);
			delete parent;
			delete child;
		}

	}

#endif
	
