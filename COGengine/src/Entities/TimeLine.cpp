#include "TimeLine.h"
#include "Facade.h"
#include "Definitions.h"


namespace Cog {

	float KeyFrame::CalcValue(const KeyFrame& next, int actualTime) {
		COGASSERT(valueId == next.valueId, "Timeline", "Incompatible keyframes. Value ids don't match!");

		if (actualTime < time) {
			return value;
		}

		if (actualTime > next.time) {
			return next.value;
		}

		// take interpolation configuration from the second keyframe
		return value + (next.value - value) * next.interpolation(((float)(actualTime - time)) / (next.time - time) * next.interpolationPoint) / next.interpolationPoint;
	}

	KeyFrame& TimeLine::GetFirstKeyFrame(StrId action) {
		return keyframes[action].at(0);
	}

	KeyFrame& TimeLine::GetLastKeyFrame(StrId action) {
		return keyframes[action].back();
	}

	KeyFrame& TimeLine::GetNextKeyFrame(KeyFrame& frame) {
		auto& frames = keyframes[frame.valueId];

		for (int i = 0; i < (frames.size() - 1); i++) {
			if (isEqual(frames.at(i).time, frame.time)) {
				return frames.at(i + 1);
			}
		}

		// there is no next frame -> return the frame itself
		return frame;
	}

	KeyFrame& TimeLine::GetPreviousKeyFrame(KeyFrame& frame) {
		auto& frames = keyframes[frame.valueId];

		for (int i = 1; i < frames.size(); i++) {
			if (isEqual(frames.at(i).time, frame.time)) {
				return frames.at(i - 1);
			}
		}

		// there is no previous frame -> return the frame itself
		return frame;
	}


	void TimeLine::InsertTimeLine(int timeFrom, TimeLine& innerTimeline) {
		// insert keyFrames
		for (auto& keyframePairs : innerTimeline.keyframes) {
			for (auto& keyframe : keyframePairs.second) {
				KeyFrame copy = KeyFrame(keyframe);
				copy.time += timeFrom; // shift the time
				InsertKeyFrame(copy);
			}
		}
	}

	void TimeLine::InsertKeyFrame(KeyFrame& keyFrame) {

		// insert into main structure
		if (keyframes.find(keyFrame.valueId) == keyframes.end()) {
			keyframes[keyFrame.valueId] = vector<KeyFrame>();
		}

		auto& keyframesOfThisValue = keyframes[keyFrame.valueId];
		keyframesOfThisValue.push_back(keyFrame);

		// order keyframes by time
		sort(keyframesOfThisValue.begin(), keyframesOfThisValue.end(),
			[](const KeyFrame&  a, const KeyFrame& b) -> bool
		{
			return a.time < b.time;
		});

		int actualBlock = CalcBlock(keyFrame.time);


		KeyFrame& previous = GetPreviousKeyFrame(keyFrame);
		KeyFrame& next = GetNextKeyFrame(keyFrame);

		int blockOfPrevious = CalcBlock(previous.time);
		int blockOfNext = CalcBlock(next.time);

		if (previous != keyFrame) {
			// fill all blocks between the previous one and the actual one with the actual one
			for (int i = blockOfPrevious; i <= actualBlock; i++) {
				keyframeBlocksLeft[i].push_back(keyFrame);
			}

			// remove all blocks between the actual and the next where the previous one was placed
			for (int i = (next != keyFrame ? blockOfNext : (CalcTotalBlocks() - 1)); i > actualBlock; i--) {
				RemoveKeyFrameFromBlock(i, false, previous);
			}
		}
		else {
			// fill all other blocks
			for (int i = 0; i <= actualBlock; i++) {
				keyframeBlocksLeft[i].push_back(keyFrame);
			}
		}

		if (next != keyFrame) {
			// fill all blocks between the actual one and the next one with the actual one
			for (int i = actualBlock; i <= blockOfNext; i++) {
				keyframeBlocksRight[i].push_back(keyFrame);
			}

			// remove all blocks between the previous one and the actual one where the next one was placed
			for (int i = (previous != keyFrame ? blockOfPrevious : 0); i < actualBlock; i++) {
				RemoveKeyFrameFromBlock(i, true, next);
			}
		}
		else {
			// fill all other blocks
			for (int i = actualBlock; i <= CalcTotalBlocks(); i++) {
				keyframeBlocksRight[i].push_back(keyFrame);
			}
		}
	}

	void TimeLine::GetNearestKeyFrames(int time, bool toTheLeft, vector<KeyFrame>& output) {

		// if the time is out of range of the time line, don't return anything
		int actualBlock = CalcBlock(time);


		map<StrId, KeyFrame> foundFrames;

		if (toTheLeft) {
			auto& keyFramesRight = keyframeBlocksRight.find(actualBlock);

			if (keyFramesRight != keyframeBlocksRight.end()) {
				for (auto& keyFrame : keyFramesRight->second) {
					if (isEqualOrLower(keyFrame.time, time)) {

						auto foundFrame = foundFrames.find(keyFrame.valueId);

						if (foundFrame == foundFrames.end() || (isEqualOrGreater(keyFrame.time, foundFrame->second.time))) {
							// keep only the nearest frame that has the same id
							foundFrames[keyFrame.valueId] = keyFrame;
						}
					}
				}
			}
		}
		else {
			auto& keyFramesLeft = keyframeBlocksLeft.find(actualBlock);

			if (keyFramesLeft != keyframeBlocksLeft.end()) {
				for (auto& keyFrame : keyFramesLeft->second) {
					if (isEqualOrGreater(keyFrame.time, time)) {

						auto foundFrame = foundFrames.find(keyFrame.valueId);

						if (foundFrame == foundFrames.end() || (isEqualOrLower(keyFrame.time, foundFrame->second.time))) {
							// keep only the nearest frame that has the same id
							foundFrames[keyFrame.valueId] = keyFrame;
						}
					}
				}
			}
		}


		for (auto& pair : foundFrames) {
			output.push_back(pair.second);
		}
	}

	void TimeLine::RemoveKeyFrameFromBlock(int block, bool leftBlocks, KeyFrame& keyFrame) {
		auto& found = leftBlocks ? keyframeBlocksLeft.find(block) : keyframeBlocksRight.find(block);

		if ((leftBlocks && found != keyframeBlocksLeft.end()) || (!leftBlocks && found != keyframeBlocksRight.end())) {
			auto& frames = found->second;

			for (auto it = frames.begin(); it != frames.end(); ++it) {
				if ((*it) == keyFrame) {
					frames.erase(it);
					return;
				}
			}
		}
	}

} // namespace