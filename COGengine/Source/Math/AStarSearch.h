#pragma once

#include <unordered_map>
#include <unordered_set>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <algorithm>
#include "Vec2i.h"

using std::unordered_map;
using std::unordered_set;
using std::array;
using std::vector;
using std::queue;
using std::priority_queue;
using std::pair;


namespace Cog {

	/**
	* Grid for searching algorithms
	*/
	class Grid {

		// grid size
		int width, height;
		// places that couldn't be crossed
		unordered_set<Vec2i> blocks;
		// cost of positions (optimal)
		unordered_map<Vec2i, int> costs;

	public:

		Grid() : width(0), height(0) {

		}

		Grid(int width, int height)
			: width(width), height(height) {}


		void AddBlock(int x, int y) {
			if (blocks.find(Vec2i(x, y)) == blocks.end()) {
				blocks.insert(Vec2i(x, y));
			}
		}

		void RemoveBlock(int x, int y) {
			if (blocks.find(Vec2i(x, y)) != blocks.end()) {
				blocks.erase(Vec2i(x, y));
			}
		}

		/**
		* Inserts a rectangle area into grid that couldn't be crossed
		*/
		void AddBlocks(int x1, int y1, int x2, int y2) {
			for (int x = x1; x <= x2; x++) {
				for (int y = y1; y <= y2; y++) {
					blocks.insert(Vec2i{ x, y });
				}
			}
		}

		/**
		* Gets all neighbors of selected position
		*/
		vector<Vec2i> Neighbors(Vec2i pos) const {

			vector<Vec2i> results;

			Vec2i nextLeft = Vec2i(pos.x - 1, pos.y);
			Vec2i nextDown = Vec2i(pos.x, pos.y - 1);
			Vec2i nextRight = Vec2i(pos.x + 1, pos.y);
			Vec2i nextUp = Vec2i(pos.x, pos.y + 1);
			
			if (IsInside(nextLeft) && !blocks.count(nextLeft)) results.push_back(nextLeft);
			if (IsInside(nextDown) && !blocks.count(nextDown)) results.push_back(nextDown);
			if (IsInside(nextRight) && !blocks.count(nextRight)) results.push_back(nextRight);
			if (IsInside(nextUp) && !blocks.count(nextUp)) results.push_back(nextUp);

			return results;
		}

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

	
	/** Operator used in priority queue which aims to prioritize by arranging */
	template <class T>
	struct greater {
		bool operator() (const T& x, const T& y) const { return x > y; }
	};

	struct AStarSearchContext {
		// collection that will be filled with jumps/steps from start to goal
		unordered_map<Vec2i, Vec2i> jumps;
		// collection that will be filled with visited positions and current price of the path
		unordered_map<Vec2i, int> costSum;
		// nearest found block
		Vec2i nearestBlock = Vec2i(0);
		// manhattan distance of the nearest found block
		int nearestDistance = -1;
	};

	/**
	* AStar search algorithm class
	* A* is a computer algorithm that is widely used in pathfinding and graph traversal,
	* the process of plotting an efficiently traversable path between multiple points, called nodes
	*
	* A* uses a best-first search and finds a least-cost path from a given initial node to one goal node
	*/
	class AStarSearch {
	public:

		/**
		* Executes A* search algorithm and finds a least-cost path from start to goal
		* @param grid grid, above which will be conducted searching 
		* @param start start position
		* @param goal target position
		* @param outputCtx output context
		* @return true, if the path was found
		*/
		bool Search(const Grid& grid, Vec2i start, Vec2i goal, AStarSearchContext& outputCtx, int maxIteration)
		{
			// initialize priority queue
			typedef pair<int, Vec2i> QueueElem;
			priority_queue<QueueElem, vector<QueueElem>, greater<QueueElem>> priorityQueue;

			// start with the first position
			priorityQueue.emplace(0, start);
			outputCtx.jumps[start] = start;
			outputCtx.costSum[start] = 0;
			int iteration = 0;

			while (!priorityQueue.empty()) {
				// get current position that should be explored
				auto current = priorityQueue.top().second;
				priorityQueue.pop();

				if (current == goal) {
					return true;
				}

				// explore its neighbors
				for (auto next : grid.Neighbors(current)) {
					// calculate the increment of the cost on the current path
					int newCostSum = outputCtx.costSum[current] + grid.GetCost(next);
					// verify if there hasn't been found a better way
					if (!outputCtx.costSum.count(next) || newCostSum < outputCtx.costSum[next]) {
						outputCtx.costSum[next] = newCostSum;
						// priority is price + manhattan distance between next position and the target
						int manhattanDist = Vec2i::ManhattanDist(next, goal);
						int priority = newCostSum + manhattanDist;

						if (outputCtx.nearestDistance == -1 || outputCtx.nearestDistance > manhattanDist) {
							outputCtx.nearestDistance = manhattanDist;
							outputCtx.nearestBlock = next;
						}

						// explore next position
						priorityQueue.emplace(priority, next);
						outputCtx.jumps[next] = current;
					}
				}

				if (maxIteration != 0 && iteration >= maxIteration) {
					// abort searching
					return false;
				}

				iteration++;
			}

			return false;
		}

		/**
		* Calculates optimal path from collection of jumps that was created during the Search method
		*/
		vector<Vec2i> CalcPathFromJumps(Vec2i start, Vec2i goal, unordered_map<Vec2i, Vec2i>& jumps) {
			vector<Vec2i> path;
			Vec2i current = goal;
			path.push_back(current);
			while (current != start) {
				current = jumps[current];
				path.push_back(current);
			}
			// reverse path so the starting position will be in the first place
			std::reverse(path.begin(), path.end());
			return path;
		}

	};

} // namespace