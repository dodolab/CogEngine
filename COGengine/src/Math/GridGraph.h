#pragma once

using namespace std;

#include <unordered_map>
#include <unordered_set>
#include <array>
#include "Vec2i.h"

namespace Cog {

	/**
	* Grid-based graph for searching algorithms
	*/
	class GridGraph {
		// grid size
		int width, height;
		// places that couldn't be crossed
		unordered_set<Vec2i> blocks;
		// optimal cost of positions 
		unordered_map<Vec2i, int> costs;

	public:

		GridGraph() : width(0), height(0) {

		}

		GridGraph(int width, int height)
			: width(width), height(height) {}


		int GetWidth() const {
			return width;
		}

		int GetHeight() const {
			return height;
		}

		/**
		* Adds a new block at selected position that can't be crossed
		*/
		void AddBlock(int x, int y);

		/**
		* Removes block at selected position
		*/
		void RemoveBlock(int x, int y);

		/**
		* Inserts a rectangle area into grid that couldn't be crossed
		*/
		void AddBlocks(int x1, int y1, int x2, int y2);

		/**
		* Returns true, if the grid has block at selected position
		*/
		bool HasBlock(int x, int y);

		/**
		* Gets all neighbors of selected position
		* @param pos position of reference cell
		* @param output output collection
		*/
		void GetNeighbors(Vec2i pos, vector<Vec2i>& output) const;

		/**
		* Gets cost of the position; returns 1, if no cost was specified
		*/
		int GetCost(Vec2i pos) const {
			return (costs.find(pos) != costs.end()) ? costs.find(pos)->second : 1;
		}

	private:
		/** Returns true, if the position is inside the grid */
		inline bool IsInside(Vec2i id) const {
			return 0 <= id.x && id.x < width && 0 <= id.y && id.y < height;
		}
	};

} // namespace