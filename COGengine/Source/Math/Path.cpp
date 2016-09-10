#include "Path.h"
#include "ofxCogCommon.h"

namespace Cog {

	// ========================= PATH SEGMENT ==============================

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

	ofVec2f PathSegment::CalcPointOnSegment(float distance) const
	{
		float percentLength = distance / length;
		float percentStart = 1 - percentLength;
		return percentStart * start + percentLength * end;
	}

	float PathSegment::CalcDistToNearestPoint(const ofVec2f & point) const
	{
		// closer to start ( x  ------- )
		if ((point - start).dot(end - start) < 0) return 0;

		// closer to end ( ------- x)
		if ((point - end).dot(start - end) < 0) return length;

		// closer to point between start and end ( ------ x ------ )
		return (end - start).dot(point - start) / length;
	}

	void PathSegment::AnalyzePoint(const ofVec2f point, const float minDist, float& closestPoint, float& closestPointDist) const
	{
		closestPoint = CalcDistToNearestPoint(point);

		if (closestPoint < minDist) closestPoint = minDist;
		

		// calculate 2D point the segmentPoint represents
		ofVec2f closest2DPoint = CalcPointOnSegment(closestPoint);
		// calculate distance between point and closestPoint
		closestPointDist = (closest2DPoint - point).length();
	}

	// ========================= PATH ==============================

	Path::Path(const ofVec2f& firstSegmentStart, const ofVec2f& firstSegmentEnd)
	{
		PathSegment firstSegment(firstSegmentStart, firstSegmentEnd);
		segments.push_back(firstSegment);
		pathLength = firstSegment.length;
	}

	void Path::AddSegment(const ofVec2f& segmentEndPoint)
	{
		// connect the segment to the last one
		PathSegment segment(segments.back().end, segmentEndPoint);
		segments.push_back(segment);
		// increment total length of the path
		pathLength += segment.length;
	}

	void Path::CalcPathPoint(float beginning, const ofVec2f point, int& segmentIndex, float& segmentValue)
	{
		COGASSERT(beginning >= 0, "PATH", "beginning must be >= 0");
		COGASSERT(segments.size() >= 1, "PATH", "there is no segment to compute");

		float minDistance = beginning; // minimal distance to consider
		float pathPointSoFar = 0; // point on the path that is closest to the given point (so far)

		for (int i = 0; i < segments.size(); i++) {

			if (minDistance < segments[i].length) {
				// found segment to consider
				
				unsigned int closestSegment = i;
				float segmentPoint; // point on the segment
				float segmentPointDistance; // distance between given point and segmentPoint
				segments[i].AnalyzePoint(point, minDistance, segmentPoint, segmentPointDistance);

				// analyze next segments if a better is found
				int j = i+1;
				while (j < segments.size())
				{
					float nextSegmentPoint;
					float nextSegmentPointDistance;
					segments[j].AnalyzePoint(point, 0, nextSegmentPoint, nextSegmentPointDistance);

					// found better segment
					if (nextSegmentPointDistance < segmentPointDistance)
					{
						pathPointSoFar += segments[j - 1].length;
						segmentPoint = nextSegmentPoint;
						segmentPointDistance = nextSegmentPointDistance;
						closestSegment = j;
						++j;
					}
					else
					{
						// there is no better segment
						break;
					}
				}
				segmentIndex = i;
				segmentValue = pathPointSoFar + segmentPoint;
				return;
			}
			else if (i == segments.size() - 1) {
				segmentIndex = i;
				segmentValue = -1;
				return;
			}

			segmentIndex = i;
			minDistance -= segments[i].length;
			pathPointSoFar += segments[i].length;
		}

		segmentIndex = 0;
		segmentValue = -1;
		return;
	}

	ofVec2f Path::CalcPathPosition(float pathPoint)
	{
		COGASSERT(pathPoint >= 0, "PATH","pathPoint must be >=0");

		for (auto& seg : segments) {
			if (pathPoint < seg.length) return seg.CalcPointOnSegment(pathPoint);
			pathPoint -= seg.length;
		}

		// there is no such point...
		return segments.back().end;
	}

}
