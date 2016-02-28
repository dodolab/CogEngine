#include "Path.h"
#include "ofxCogCommon.h"

namespace Cog {

	Path::Path(const ofVec2f & start, const ofVec2f & end)
	{
		PathSegment firstSegment(start, end);
		segments.push_back(firstSegment);
		pathLength = firstSegment.length;
	}


	float Path::CalcPathPoint(float beginning, const ofVec2f & point)
	{
		COGASSERT(beginning >= 0, "PATH", "beginning must be >= 0");
		COGASSERT(segments.size() >= 1, "PATH", "there is no segment to compute");

		float minDistance = beginning; // minimal distance to consider
		float pathPointSoFar = 0; // point on the path that is closest to the given point (so far)

		for (int i = 0; i < segments.size(); i++) {

			// found segment to consider
			if (minDistance < segments[i].length) {
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

				return pathPointSoFar + segmentPoint;
			}
			else if (i == segments.size() - 1) {
				return -1;
			}

			minDistance -= segments[i].length;
			pathPointSoFar += segments[i].length;
		}
	}

	ofVec2f Path::CalcPathPosition(float pathPoint)
	{
		COGASSERT(pathPoint >= 0, "PATH","pathPoint must be >=0");

		for (unsigned int i = 0; i < segments.size(); ++i)
		{
			if (pathPoint < segments[i].length)
			{
				// The position is in the ith segment.
				return segments[i].CalcPointOnSegment(pathPoint);
			}
			pathPoint -= segments[i].length;
		}
		
		// there is no such point...
		return ofVec2f();
	}

	void Path::AddSegment(const ofVec2f & segmentEndPoint)
	{
		// connect the segment to the last one
		PathSegment segment(segments.back().end, segmentEndPoint);
		segments.push_back(segment);
		pathLength += segment.length;
	}
}
