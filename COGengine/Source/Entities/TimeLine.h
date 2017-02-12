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

		KeyFrame(StrId valueId, float time) : valueId(valueId), time(time) {

		}

		// id of animated value
		StrId valueId;
		FadeFunction interpolation = EasingFunc::linear;
		// the time the keyframe is located
		float time;

		bool operator==(const KeyFrame& other) {
			return valueId == other.valueId && isEqual(time, other.time);
		}
		bool operator!=(const KeyFrame& other) {
			return valueId != other.valueId || !isEqual(time, other.time);
		}
	};

	/**
	* Static animation
	*/
	struct StaticAnim {

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
		vector<StaticAnim> staticAnimations;

		float length = 1;
		float speed = 1;

	public:

		/**
		* Gets length of the time line, defined as a portion of duration and speed
		*/
		float GetLength() const {
			return length;
		}

		/**
		* Sets length of the time line, defined as a portion of duration and speed
		*/
		void SetLength(float length) {
			this->length = length;
		}

		/**
		* Gets speed of the timeline
		*/
		float GetSpeed() const {
			return speed;
		}

		/**
		* Sets speed of the timeline
		*/
		void SetSpeed(float speed) {
			this->speed = speed;
		}

		/**
		* Gets duration in milliseconds
		*/
		int GetDuration() const {
			return (int)(speed * length);
		}

		bool ContainsKeyFrames(StrId valueId) const {
			return keyframes.find(valueId) != keyframes.end();
		}

		vector<StaticAnim>& GetStaticAnims() {
			return staticAnimations;
		}


		vector<KeyFrame>& GetKeyFramesById(StrId valueId) {
			return keyframes[valueId];
		}

		/**
		* Finds all keyframes that lie to the left on timeline
		*/
		void GetKeyFramesToTheLeft(float time, vector<KeyFrame>& output) {
			GetKeyFrames(time, true, output);
		}

		/**
		* Finds all keyframes that lie to the right on timeline
		*/
		void GetKeyFramesToTheRight(float time, vector<KeyFrame>& output) {
			GetKeyFrames(time, false, output);
		}

		/**
		* Gets the next keyframe from the selected keyframe
		*/
		KeyFrame GetNextKeyFrame(KeyFrame& frame);

		/**
		* Gets the previous keyframe from the selected keyframe
		*/
		KeyFrame GetPreviousKeyFrame(KeyFrame& frame);

		/**
		* Inserts data from another timeline
		*/
		void InsertTimeLine(float timeFrom, TimeLine& innerTimeline);

		/**
		* Inserts keyframe into timeline
		*/
		void InsertKeyFrame(KeyFrame& keyFrame);

	protected:
		inline int CalcTotalBlocks() {
			return (int)((TIMELINE_BLOCKS / 60.0f) * ((speed * length) / 1000));
		}

		inline int CalcBlock(float time) {
			return (int)((time / (speed * length)) * CalcTotalBlocks());
		}
		void GetKeyFrames(float time, bool toTheLeft, vector<KeyFrame>& output);

		void RemoveKeyFrameFromBlock(int block, bool leftBlocks, KeyFrame& keyFrame);
	};

}// namespace


