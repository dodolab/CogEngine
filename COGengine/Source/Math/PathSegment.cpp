
#include "PathSegment.h"
#include "ofVec2f.h"

namespace Cog {


	void PathSegment::operator=(const PathSegment & segment)
	{
		this->start = segment.start;
		this->end = segment.end;
		this->length = segment.length;
	}

	PathSegment::PathSegment(const ofVec2f & start, const ofVec2f & end) : start(start), end(end)
	{
		length = (end - start).length();
	}

	ofVec2f PathSegment::CalcPointOnSegment(float distAlongSegment) const
	{
		float endWeight = distAlongSegment / length;
		float startWeight = 1 - endWeight;
		return startWeight * start + endWeight * end;
	}

	float PathSegment::CalcDistAlongSegment(const ofVec2f & point) const
	{
		// closer to start
		if ((point - start).dot(end - start) < 0)
		{
			return 0;
		}

		// closer to end
		if ((point - end).dot(start - end) < 0)
		{
			return length;
		}

		// closer to point between start and end
		return (end - start).dot(point - start) / length;
	}

	void PathSegment::AnalyzePoint(const ofVec2f & point, const float minDist, float & closestSegmentPoint, float & closestSegmentPointdist) const
	{
		// calculate segmentPoint
		closestSegmentPoint = CalcDistAlongSegment(point);
		
		if (closestSegmentPoint < minDist)
		{
			closestSegmentPoint = minDist;
		}
		
		// calculate 2D point the segmentPoint represents
		ofVec2f closestPoint = CalcPointOnSegment(closestSegmentPoint);
		// calculate distance between point and closestPoint
		closestSegmentPointdist = (closestPoint - point).length();
	}
}
