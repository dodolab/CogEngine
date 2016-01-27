#pragma once

#include <unordered_map>
#include <unordered_set>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <algorithm>


using std::unordered_map;
using std::unordered_set;
using std::array;
using std::vector;
using std::queue;
using std::priority_queue;
using std::pair;

namespace Cog {

	/**
	* Simple integer position, used in graph-searching
	*/
	struct Pos {

		Pos() : x(0), y(0) {

		}

		Pos(int x, int y) : x(x), y(y) {

		}

		/** position on the x axis */
		int x;
		/** position on the y axis */
		int y;


		inline operator size_t() const {
			// we use seed for Mersenne Twister generator
			return x * 1812433253 + y;
		}

		inline bool operator==(const Pos& a) const {
			return x == a.x && y == a.y;
		}

		inline bool operator!=(const Pos& a) const {
			return x != a.x || y != a.y;
		}

		inline bool operator<(const Pos& a) const {
			return ((size_t)*this) < ((size_t)a);
		}

		inline bool operator>(const Pos& a) const {
			return ((size_t)*this) > ((size_t)a);
		}
	};

}

namespace std {
	// overriding hash function for position
	template <>
	struct hash<Cog::Pos> {
		inline size_t operator()(const Cog::Pos& pos) const {
			return pos.x * 1812433253 + pos.y;
		}
	};
}


namespace Cog {

	/**
	* Grid for searching algorithms
	*/
	class Grid {

		// grid size
		int width, height;
		// places that couldn't be crossed
		unordered_set<Pos> blocks;
		// cost of positions (optimal)
		unordered_map<Pos, int> costs;

	public:

		Grid() : width(0), height(0) {

		}

		Grid(int width, int height)
			: width(width), height(height) {}


		/**
		* Inserts a rectangle area into grid that couldn't be crossed
		*/
		void AddBlocks(int x1, int y1, int x2, int y2) {
			for (int x = x1; x <= x2; x++) {
				for (int y = y1; y <= y2; y++) {
					blocks.insert(Pos{ x, y });
				}
			}
		}

		/**
		* Gets all neighbors of selected position
		*/
		vector<Pos> Neighbors(Pos pos) const {
			int dx, dy;
			vector<Pos> results;

			Pos nextLeft = Pos(pos.x - 1, pos.y);
			Pos nextDown = Pos(pos.x, pos.y - 1);
			Pos nextRight = Pos(pos.x + 1, pos.y);
			Pos nextUp = Pos(pos.x, pos.y + 1);
			
			if (IsInside(nextLeft) && !blocks.count(nextLeft)) results.push_back(nextLeft);
			if (IsInside(nextDown) && !blocks.count(nextDown)) results.push_back(nextDown);
			if (IsInside(nextRight) && !blocks.count(nextRight)) results.push_back(nextRight);
			if (IsInside(nextUp) && !blocks.count(nextUp)) results.push_back(nextUp);

			return results;
		}

		/**
		* Gets cost of the position; returns 1, if no cost was specified
		*/
		int GetCost(Pos pos) const {
			return (costs.find(pos) != costs.end()) ? costs.find(pos)->second : 1;
		}

	private:
		/** Returns true, if the position is inside the grid */
		inline bool IsInside(Pos id) const {
			return 0 <= id.x && id.x < width && 0 <= id.y && id.y < height;
		}
	};

	
	/** Operator used in priority queue which aims to prioritize by arranging */
	template <class T>
	struct greater {
		bool operator() (const T& x, const T& y) const { return x > y; }
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
		* @param jumps collection that will be filled with jumps/steps from start to goal
		* @param costSum collection that will be filled with visited positions and current price of the path
		*/
		void Search(const Grid& grid, Pos start, Pos goal, unordered_map<Pos, Pos>& jumps, unordered_map<Pos, int>& costSum)
		{
			// initialize priority queue
			typedef pair<int, Pos> QueueElem;
			priority_queue<QueueElem, vector<QueueElem>, greater<QueueElem>> priorityQueue;

			// start with the first position
			priorityQueue.emplace(0, start);
			jumps[start] = start;
			costSum[start] = 0;

			while (!priorityQueue.empty()) {
				// get current position that should be explored
				auto current = priorityQueue.top().second;
				priorityQueue.pop();

				if (current == goal) {
					break;
				}

				// explore its neighbors
				for (auto next : grid.Neighbors(current)) {
					// calculate the increment of the cost on the current path
					int newCostSum = costSum[current] + grid.GetCost(next);
					// verify if there hasn't been found a better way
					if (!costSum.count(next) || newCostSum < costSum[next]) {
						costSum[next] = newCostSum;
						// priority is price + manhattan distance between next position and the target
						int priority = newCostSum + CalcDistance(next, goal);
						// explore next position
						priorityQueue.emplace(priority, next);
						jumps[next] = current;
					}
				}
			}
		}

		/**
		* Calculates optimal path from collection of jumps that was created during the Search method
		*/
		vector<Pos> CalcPathFromJumps(Pos start, Pos goal, unordered_map<Pos, Pos>& jumps) {
			vector<Pos> path;
			Pos current = goal;
			path.push_back(current);
			while (current != start) {
				current = jumps[current];
				path.push_back(current);
			}
			// reverse path so the starting position will be in the first place
			std::reverse(path.begin(), path.end());
			return path;
		}

	private:
		/**
		* Calculates distance between two points
		* uses simple Manhattan distance 
		*/
		inline int CalcDistance(Pos a, Pos b) {
			return abs(a.x - b.x) + abs(a.y - b.y);
		}
	};

} // namespace