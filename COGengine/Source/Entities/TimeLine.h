#pragma once

#include "Definitions.h"
#include "StrId.h"
#include "EasingFunc.h"
#include "Utils.h"

namespace Cog {

	// number of blocks of the timeline internal cache per minute
#define TIMELINE_BLOCKS 10

	/**
	* Keyframe that represents point on the timeline and animated value
	*/
	struct KeyFrame {
		KeyFrame() {

		}

		KeyFrame(StrId valueId, int time, float value) : valueId(valueId), time(time), value(value) {

		}

		// id of animated value
		StrId valueId;
		// interpolation function
		FadeFunction interpolation = EasingFunc::linear;
		// the point the interpolation should end <0,1>
		float interpolationPoint = 1.0f;
		// the time the keyframe is located
		int time;
		// actual value the object should have at given time
		float value;

		bool operator==(const KeyFrame& other) {
			return valueId == other.valueId && isEqual(time, other.time);
		}

		bool operator!=(const KeyFrame& other) {
			return valueId != other.valueId || !isEqual(time, other.time);
		}

		/**
		* Calculates the actual value at given time
		* @param next the next keyframe
		* @param actualTime the actual time
		*/
		float CalcValue(const KeyFrame& next, int actualTime);
	};


	/**
	* Animation timeline that consists of list of keyframes
	*/
	class TimeLine {

	protected:
		// main structure that holds all keyframes, ordered by time
		map<StrId, vector<KeyFrame>> keyframes;
		// keyframes ordered in blocks (number of blocks is defined by TIMELINE_BLOCKS directive)
		// each block contains list of all keyframes that can affect the block from their left border
		map<int, vector<KeyFrame>> keyframeBlocksLeft;
		// keyframes ordered in blocks; each block contains list of all keyframes that can affect the block
		// from their right border
		map<int, vector<KeyFrame>> keyframeBlocksRight;

		int length = 1;
		int speed = 1;
		int blocks = TIMELINE_BLOCKS;

	public:

		TimeLine() {

		}

		/**
		* Creates a new timeline
		* @param blocksPerMinute number of caching blocks within one minute
		*/
		TimeLine(int blocksPerMinute) {
			this->blocks = blocksPerMinute;
		}

		int GetBlocksPerMinute() {
			return blocks;
		}

		/**
		* Gets length of the time line, defined as a portion of duration and speed
		*/
		int GetLength() const {
			return length;
		}

		/**
		* Sets length of the time line, defined as a portion of duration and speed
		*/
		void SetLength(int length) {
			this->length = length;
		}

		/**
		* Gets speed of the timeline
		*/
		int GetSpeed() const {
			return speed;
		}

		/**
		* Sets speed of the timeline
		*/
		void SetSpeed(int speed) {
			this->speed = speed;
		}

		/**
		* Gets duration in milliseconds
		*/
		int GetDuration() const {
			return speed * length;
		}

		bool ContainsKeyFrames(StrId valueId) const {
			return keyframes.find(valueId) != keyframes.end();
		}

		int GetKeyFramesCount(StrId valueId) const {
			auto found = keyframes.find(valueId);
			if (found != keyframes.end()) {
				return found->second.size();
			}
			else {
				return 0;
			}
		}

		vector<KeyFrame>& GetKeyFramesById(StrId valueId) {
			return keyframes[valueId];
		}

		/**
		* Finds nearest keyframes of all types that lie to the left on timeline
		*/
		void GetNearestKeyFramesToTheLeft(int time, vector<KeyFrame>& output) {
			GetNearestKeyFrames(time, true, output);
		}

		/**
		* Finds nearest keyframes of all types that lie to the right on timeline
		*/
		void GetNearestKeyFramesToTheRight(int time, vector<KeyFrame>& output) {
			GetNearestKeyFrames(time, false, output);
		}

		/**
		* Gets the first key frame of the selected action
		*/
		KeyFrame& GetFirstKeyFrame(StrId action);

		/**
		* Gets the last key frame of the selected action
		*/
		KeyFrame& GetLastKeyFrame(StrId action);

		/**
		* Gets the next keyframe from the selected keyframe
		*/
		KeyFrame& GetNextKeyFrame(KeyFrame& frame);

		/**
		* Gets the previous keyframe from the selected keyframe
		*/
		KeyFrame& GetPreviousKeyFrame(KeyFrame& frame);

		/**
		* Inserts data from another timeline
		*/
		void InsertTimeLine(int timeFrom, TimeLine& innerTimeline);

		/**
		* Inserts keyframe into timeline
		*/
		void InsertKeyFrame(KeyFrame& keyFrame);

	protected:
		inline int CalcTotalBlocks() {
			int total = (int)((blocks / 60.0f) * ((speed * length) / 1000.0f));
			return total != 0 ? total : 1;
		}

		inline int CalcBlock(int time) {
			return (int)((time / (1.0f * speed * length)) * CalcTotalBlocks());
		}

		void GetNearestKeyFrames(int time, bool toTheLeft, vector<KeyFrame>& output);

		void RemoveKeyFrameFromBlock(int block, bool leftBlocks, KeyFrame& keyFrame);
	};

}// namespace


