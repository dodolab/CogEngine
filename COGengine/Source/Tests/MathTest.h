#pragma once

#include "ofMain.h"

#include "catch.hpp"
using namespace Cog;

#include "MathTestAssets.h"















TEST_CASE("MathTest")
{
	SECTION("AStarSearch simple path test")
	{
		AStarWrapper wr = AStarWrapper(10,10);
		wr.grid.AddBlocks(1, 0, 8, 8); // the path will go down and right

		REQUIRE(wr.grid.HasBlock(1,0));
		REQUIRE(wr.grid.HasBlock(8, 8));
		REQUIRE(wr.grid.HasBlock(5, 5));
		REQUIRE(wr.grid.HasBlock(6, 6));
		REQUIRE(!wr.grid.HasBlock(0, 5));
		REQUIRE(!wr.grid.HasBlock(7, 9));

		AStarSearchContext context;
		wr.search.Search(wr.grid, Vec2i(0, 0), Vec2i(9, 9), context, 0);
		vector<Vec2i> path;
		wr.search.CalcPathFromSteps(Vec2i(0, 0), Vec2i(9, 9), context.steps, path);
		wr.WritePath(path);

		REQUIRE(path.size() == 19);
		REQUIRE(path[0] == Vec2i(0, 0));
		REQUIRE(path.back() == Vec2i(9, 9));
		REQUIRE(path[1] == Vec2i(0, 1));
		REQUIRE(path[10] == Vec2i(1, 9));
		REQUIRE(path[15] == Vec2i(6,9));
	}

	SECTION("AStarSearch maze test")
	{
		// define zig-zag path
		AStarWrapper wr = AStarWrapper(10, 10);
		wr.grid.AddBlocks(1, 0, 1, 8);
		wr.grid.AddBlocks(3, 1, 3, 9);
		wr.grid.AddBlocks(5, 0, 5, 8);
		wr.grid.AddBlocks(7, 1, 7, 9);
		wr.grid.AddBlocks(9, 0, 9, 8);
		AStarSearchContext context;
		wr.search.Search(wr.grid, Vec2i(0, 0), Vec2i(9, 9), context, 0);
		vector<Vec2i> path;
		wr.search.CalcPathFromSteps(Vec2i(0, 0), Vec2i(9, 9), context.steps, path);
		wr.WritePath(path);

		REQUIRE(path.size() == 55);
		REQUIRE(path[0] == Vec2i(0, 0));
		REQUIRE(path.back() == Vec2i(9, 9));
		REQUIRE(path[1] == Vec2i(0, 1));
		REQUIRE(path[23] == Vec2i(4, 1));
		REQUIRE(path[47] == Vec2i(8, 3));
	}

	SECTION("AStarSearch test limited iterations")
	{
		AStarWrapper wr = AStarWrapper(10, 10);
		wr.grid.AddBlocks(2, 1, 3, 3);
		wr.grid.AddBlocks(2, 6, 3, 8);
		wr.grid.AddBlocks(5, 4, 7, 9);
		wr.grid.AddBlocks(2, 0, 7, 0);
		wr.grid.AddBlocks(7, 5, 9, 6);
		AStarSearchContext context;
		wr.search.Search(wr.grid, Vec2i(0, 0), Vec2i(9, 0), context, 15);
		vector<Vec2i> path;
		wr.search.CalcPathFromSteps(Vec2i(0, 0), context.nearestBlock, context.steps, path);
		wr.WritePath(path);

		REQUIRE(path.size() == 13);
		REQUIRE(path[0] == Vec2i(0, 0));
		REQUIRE(path.back() == Vec2i(5, 1));
		REQUIRE(path[1] == Vec2i(1, 0));
		REQUIRE(path[8] == Vec2i(4, 4));
		REQUIRE(path[10] == Vec2i(5, 3));
	}

	SECTION("Path point test")
	{
		// define rectangle
		Path pth = Path(ofVec2f(0, 0), ofVec2f(1, 0));
		pth.AddSegment(ofVec2f(1, 1));
		pth.AddSegment(ofVec2f(0, 1));
		pth.AddSegment(ofVec2f(0, 0));

		int length = pth.GetPathLength();
		int index;
		float val;
		// center of first segment
		pth.CalcPathPoint(0, ofVec2f(0, 0.5f), index, val);
		REQUIRE(index == 0);
		REQUIRE(isEqual(val, 0));

		// second third of first segment
		pth.CalcPathPoint(0, ofVec2f(0.75f, 0), index, val);
		REQUIRE(index == 0);
		REQUIRE(isEqual(val, 0.75f));


		// center of second segment, calculating from beginning
		pth.CalcPathPoint(0, ofVec2f(1, 0.5f), index, val);
		REQUIRE(index == 0);
		REQUIRE(isEqual(val, 1.5f));

		// center of second segment, calculating from the start of the second segment
		pth.CalcPathPoint(1, ofVec2f(1, 0.5f), index, val);
		REQUIRE(index == 1);
		REQUIRE(isEqual(val, 1.5f));
		
		// center of the whole rectangle, calculating from beginning
		pth.CalcPathPoint(0, ofVec2f(0.5f), index, val);
		REQUIRE(index == 0);
		REQUIRE(isEqual(val, 0.5f));
		
		// right of the rectangle, closer to the third point
		pth.CalcPathPoint(0, ofVec2f(5,0.7f), index, val);
		REQUIRE(index == 0);
		REQUIRE(isEqual(val, 1.7f));

		auto pos = pth.CalcPathPosition(0);
		REQUIRE(isEqual(pos.x, 0));
		REQUIRE(isEqual(pos.y, 0));

		pos = pth.CalcPathPosition(1);
		REQUIRE(isEqual(pos.x, 1));
		REQUIRE(isEqual(pos.y, 0));

		pos = pth.CalcPathPosition(2);
		REQUIRE(isEqual(pos.x, 1));
		REQUIRE(isEqual(pos.y, 1));

		pos = pth.CalcPathPosition(3);
		REQUIRE(isEqual(pos.x, 0));
		REQUIRE(isEqual(pos.y, 1));

		pos = pth.CalcPathPosition(4);
		REQUIRE(isEqual(pos.x, 0));
		REQUIRE(isEqual(pos.y, 0));
	}

	SECTION("GridSpace partition test")
	{
		auto space = GridSpace<TestCellObj>(ofVec2f(10,10),5);
		REQUIRE(space.GetColumns() == 2);
		REQUIRE(space.GetRows() == 2);
		REQUIRE(space.GetCellWidth() == 5);
		REQUIRE(space.GetCellHeight() == 5);
	}

	SECTION("GridSpace partition test 2")
	{
		auto space = GridSpace<TestCellObj>(ofVec2f(10, 10), 1);
		REQUIRE(space.GetColumns() == 10);
		REQUIRE(space.GetRows() == 10);
		REQUIRE(space.GetCellWidth() == 1);
		REQUIRE(space.GetCellHeight() == 1);
	}

	SECTION("GridSpace query")
	{
		auto space = GridSpace<TestCellObj>(ofVec2f(10, 10), 1);
		auto topleft = new TestCellObj(Vec2i(0,0));
		auto topright = new TestCellObj(Vec2i(0, 9));
		auto bottomleft = new TestCellObj(Vec2i(9, 0));
		auto bottomright = new TestCellObj(Vec2i(9, 9));
		space.AddObject(topleft);
		space.AddObject(topright);
		space.AddObject(bottomleft);
		space.AddObject(bottomright);

		vector<TestCellObj*> neighbors;
		space.CalcNeighbors(ofVec2f(0), 5, neighbors);
		REQUIRE(neighbors.size() == 1);
		REQUIRE(neighbors[0]->pos == Vec2i(0));

		neighbors.clear();
		space.CalcNeighbors(ofVec2f(0), 10, neighbors);
		REQUIRE(neighbors.size() == 3);

		bool exists = space.ExistsObject(ofVec2f(9, 9), 1);
		REQUIRE(exists);

		// position objects in a line
		topleft->SetPos(Vec2i(5, 5));
		topright->SetPos(Vec2i(5, 6));
		bottomleft->SetPos(Vec2i(5, 7));
		bottomright->SetPos(Vec2i(5, 8));

		space.UpdateObjects();
		neighbors.clear();

		space.CalcNeighbors(ofVec2f(5), 5, neighbors);
		REQUIRE(neighbors.size() == 4);

		neighbors.clear();
		space.CalcNeighbors(ofVec2f(0), 5, neighbors);
		REQUIRE(neighbors.size() == 0);

		exists = space.ExistsObject(ofVec2f(5, 5), 0.5f);
		REQUIRE(exists);
		
		neighbors.clear();
		space.CalcNeighbors(ofVec2f(4,4), 2, neighbors);
		REQUIRE(neighbors.size() == 1);

	}

	SECTION("GridSpace query 2")
	{
		// the same test as GridSpace query 1 but with higher partition index

		auto space = GridSpace<TestCellObj>(ofVec2f(10, 10), 4);
		auto topleft = new TestCellObj(Vec2i(0, 0));
		auto topright = new TestCellObj(Vec2i(0, 9));
		auto bottomleft = new TestCellObj(Vec2i(9, 0));
		auto bottomright = new TestCellObj(Vec2i(9, 9));
		space.AddObject(topleft);
		space.AddObject(topright);
		space.AddObject(bottomleft);
		space.AddObject(bottomright);

		vector<TestCellObj*> neighbors;
		space.CalcNeighbors(ofVec2f(0), 5, neighbors);
		REQUIRE(neighbors.size() == 1);
		REQUIRE(neighbors[0]->pos == Vec2i(0));

		neighbors.clear();
		space.CalcNeighbors(ofVec2f(0), 10, neighbors);
		REQUIRE(neighbors.size() == 3);

		bool exists = space.ExistsObject(ofVec2f(9, 9), 1);
		REQUIRE(exists);

		// position objects in a line
		topleft->SetPos(Vec2i(5, 5));
		topright->SetPos(Vec2i(5, 6));
		bottomleft->SetPos(Vec2i(5, 7));
		bottomright->SetPos(Vec2i(5, 8));

		space.UpdateObjects();
		neighbors.clear();

		space.CalcNeighbors(ofVec2f(5), 5, neighbors);
		REQUIRE(neighbors.size() == 4);

		neighbors.clear();
		space.CalcNeighbors(ofVec2f(0), 5, neighbors);
		REQUIRE(neighbors.size() == 0);

		exists = space.ExistsObject(ofVec2f(5, 5), 0.5f);
		REQUIRE(exists);

		neighbors.clear();
		space.CalcNeighbors(ofVec2f(4, 4), 2, neighbors);
		REQUIRE(neighbors.size() == 1);

	}
}

