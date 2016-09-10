#include "GridGraph.h"

namespace Cog {


	void GridGraph::AddBlock(int x, int y) {
		if (blocks.find(Vec2i(x, y)) == blocks.end()) {
			blocks.insert(Vec2i(x, y));
		}
	}

	void GridGraph::RemoveBlock(int x, int y) {
		if (blocks.find(Vec2i(x, y)) != blocks.end()) {
			blocks.erase(Vec2i(x, y));
		}
	}

	void GridGraph::AddBlocks(int x1, int y1, int x2, int y2) {
		for (int x = x1; x <= x2; x++) {
			for (int y = y1; y <= y2; y++) {
				blocks.insert(Vec2i{ x, y });
			}
		}
	}

	void GridGraph::GetNeighbors(Vec2i pos, vector<Vec2i>& output) const {

		Vec2i nextLeft = Vec2i(pos.x - 1, pos.y);
		Vec2i nextDown = Vec2i(pos.x, pos.y - 1);
		Vec2i nextRight = Vec2i(pos.x + 1, pos.y);
		Vec2i nextUp = Vec2i(pos.x, pos.y + 1);

		if (IsInside(nextLeft) && !blocks.count(nextLeft)) output.push_back(nextLeft);
		if (IsInside(nextDown) && !blocks.count(nextDown)) output.push_back(nextDown);
		if (IsInside(nextRight) && !blocks.count(nextRight)) output.push_back(nextRight);
		if (IsInside(nextUp) && !blocks.count(nextUp)) output.push_back(nextUp);
	}

} // namespace