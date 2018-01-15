#pragma once

#include <utility>
#include <queue>
#include <algorithm>
#include "Vec2i.h"

#include "GridGraph.h"

using namespace std;

namespace Cog {

	/**
	* Context object for A* search algorithm
	* Contains collection of output entities
	*/
	struct AStarSearchContext {
		// map with steps from start to goal
		unordered_map<Vec2i, Vec2i> steps;
		// nearest found block (equal to goal, if the path was found)
		Vec2i nearestBlock = Vec2i(0);
		// manhattan distance of the nearest found block
		int nearestDistance = -1;
	};

	/**
	* AStar search algorithm 
	*
	* A* is widely used in pathfinding and graph traversal,
	* the process of plotting an efficiently traversable path between multiple points, called nodes
	* 
	* The main advantage is that it uses heuristic method (manhattan distance) in grid-based graph. If the path
	* from start to goal follows the rules for real topologies (paths in real world), it can be found in a very
	* short time
	*
	*/
	class AStarSearch {
	public:

		/**
		* Executes A* search algorithm and finds a least-cost path from start to goal
		* @param grid grid, above which will be conducted searching 
		* @param start start position
		* @param goal target position
		* @param outputCtx output context
		* @param maxIteration maximum number of iterations (0 for unlimited)
		* @return true, if the path was found
		*/
		bool Search(const GridGraph& grid, Vec2i start, Vec2i goal, AStarSearchContext& outputCtx, int maxIteration);

		/**
		* Calculates collection of steps from start to goal, using the map of steps from the output of the search algorithm
		* @param start start position
		* @param goal target position
		* @param steps unordered map of steps
		* @param output output sorted collection of steps
		*/
		void CalcPathFromSteps(Vec2i start, Vec2i goal, unordered_map<Vec2i, Vec2i>& steps, vector<Vec2i>& output);

	};

} // namespace