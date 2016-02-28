#pragma once

#include "ofVec2f.h"

namespace Cog {

	/**
	* Segment of the path
	*/
	struct PathSegment
	{
		PathSegment(const ofVec2f & start, const ofVec2f & end);
		virtual ~PathSegment() { }

		void operator=(const PathSegment & segment);
		
		/**
		* Calculates point on the segment with given distance
		*/
		ofVec2f CalcPointOnSegment(float distAlongSegment) const;

		/**
		* Calculates distance to the nearest point on segment
		*/
		float CalcDistAlongSegment(const ofVec2f & point) const;

		/**
		* Analyzes given point, calculates distance along segment and distance to the closest point
		*/
		void AnalyzePoint(const ofVec2f & point, const float minDist, float & closestSegmentPoint, float & closestSegmentPointdist) const;

		ofVec2f start;
		ofVec2f end;
		float length;
	};


}
