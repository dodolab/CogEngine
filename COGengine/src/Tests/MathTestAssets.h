#pragma once

#include "PathFinder.h"
#include "GridSpace.h"
#include "GridMap.h"
#include "Path.h"
#include "SteeringMath.h"

using namespace Cog;

class AStarWrapper {
public:
	AStarSearch search;
	GridMap grid;

	AStarWrapper(int width, int height) {
		grid = GridMap(MapType::TILE, 5, width, height);
	}

	void WriteGrid(){
		for (int i = 0; i < grid.GetHeight(); i++) {
			for (int j = 0; j < grid.GetWidth(); j++) {
				bool isBlock = grid.HasObstruction(j, i);
				if (isBlock) cout << " X ";
				else cout << " - ";
			}
			cout << endl;
		}
		cout << endl;
	}

	void WritePath(vector<Vec2i>& path) {
		for (int i = 0; i < grid.GetHeight(); i++) {
			for (int j = 0; j < grid.GetWidth(); j++) {
				bool isBlock = grid.HasObstruction(j, i);
				bool isPath = find(path.begin(), path.end(), Vec2i(j, i)) != path.end();
				
				if (isPath) cout << " o ";
				else if (isBlock) cout << " X ";
				else cout << " - ";
			}
			cout << endl;
		}
		cout << endl;
	}
};

class TestCellObj : public ICellObject {
public:
	Vec2i pos;
	Vec2i prevPos;

	TestCellObj() : pos(Vec2i(0)), prevPos(Vec2i(0)) {

	}

	TestCellObj(Vec2i pos) : pos(pos), prevPos(Vec2i(0)) {

	}

	virtual ofVec2f GetPosition() {
		return pos;
	}
	virtual ofVec2f GetPreviousPosition() {
		return prevPos;
	}

	void SetPos(Vec2i pos) {
		prevPos = this->pos;
		this->pos = pos;
	}
};
