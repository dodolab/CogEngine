#pragma once

#include <vector>
#include "PathSegment.h"

namespace Cog {


	/**
	* Sequence of line segments used in steering behaviors
	*/
	class Path
	{
	public:
		std::vector<PathSegment> segments;
		float pathLength;

		/**
		* Creates a new path
		* @param start starting point of the first segment
		* @param end ending point of the first segment
		*/
		Path(const ofVec2f & start, const ofVec2f & end);

		/**
		* Calculates point on the path from given starting point that is nearest
		* to the given 2D point that doesn't lie on the path
		* Returns value between the beginning point and the last segment of the path <beginning, pathLength>
		*/
		float CalcPathPoint(float beginning, const ofVec2f & point, int& segment);

		/**
		* Calculates 2D position on the path according to the given point on the path
		*/
		ofVec2f CalcPathPosition(float pathPoint);

		/**
		* Adds a new segment with given endpoint
		*/
		void AddSegment(const ofVec2f & endPoint);

	};


}
