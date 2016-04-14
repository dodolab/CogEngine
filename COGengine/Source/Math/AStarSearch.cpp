#include "AStarSearch.h"

namespace Cog {

	
	bool AStarSearch::Search(const GridGraph& grid, Vec2i start, Vec2i goal, AStarSearchContext& outputCtx, int maxIteration)
	{

		// initialize priority queue, using GREATER functional template that provides descending order of priorities
		priority_queue<pair<int, Vec2i>, vector<pair<int, Vec2i>>, greater<pair<int, Vec2i>>> priorityQueue;
		unordered_map<Vec2i, int> costSum;

		// start with the first position
		priorityQueue.emplace(0, start);
		outputCtx.steps[start] = start;
		costSum[start] = 0;
		int iteration = 0;

		while (!priorityQueue.empty()) {
			// get current position that should be explored
			auto current = priorityQueue.top().second;
			priorityQueue.pop();

			if (current == goal) {
				return true; // the goal was achieved
			}

			// get neighbors of the current grid block
			vector<Vec2i> neighbors;
			grid.GetNeighbors(current, neighbors);

			// explore neighbors
			for (auto next : neighbors) {
				// calculate the increment of the cost on the current path
				int newCostSum = costSum[current] + grid.GetCost(next);
				// verify if a better way was found
				if (!costSum.count(next) || newCostSum < costSum[next]) {
					costSum[next] = newCostSum;
					// priority is price + manhattan distance between next position and the target
					int manhattanDist = Vec2i::ManhattanDist(next, goal);
					int priority = newCostSum + manhattanDist;

					// check the new block
					if (outputCtx.nearestDistance == -1 || outputCtx.nearestDistance > manhattanDist) {
						outputCtx.nearestDistance = manhattanDist;
						outputCtx.nearestBlock = next;
					}

					// explore next block
					priorityQueue.emplace(priority, next);
					outputCtx.steps[next] = current;
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

	void AStarSearch::CalcPathFromSteps(Vec2i start, Vec2i goal, unordered_map<Vec2i, Vec2i>& steps, vector<Vec2i>& output) {

		Vec2i current = goal;
		output.push_back(current);
		while (current != start) {
			current = steps[current];
			output.push_back(current);
		}
		// reverse path so the starting position will be on the first place
		std::reverse(output.begin(), output.end());
	}


} // namespace