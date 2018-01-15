#include "TimeMeasure.h"
#include "ofUtils.h"

namespace Cog {

	extern void CogLogInfo(const char* module, const char* format, ...);

	void TimeMeasure::Restart() {
		blocks.clear();
		this->startTime = ofGetElapsedTimeMicros();
		this->endTime = ofGetElapsedTimeMicros();
	}

	void TimeMeasure::MeasureBlockStart(string blockName) {
		if(startTime == 0) {
			startTime = ofGetElapsedTimeMicros();
		}

		auto existing = blocks.find(blockName);

		if (existing != blocks.end()) {
			(*existing).second.CallBegin(ofGetElapsedTimeMicros());
		}
		else {
			blocks[blockName] = MeasureScope(blockName, ofGetElapsedTimeMicros());
		}

		this->endTime = ofGetElapsedTimeMicros();
	}

	void TimeMeasure::MeasureBlockEnd(string blockName) {
		auto existing = blocks.find(blockName);

		if (existing != blocks.end()) {
			(*existing).second.CallEnd(ofGetElapsedTimeMicros());
		}

		this->endTime = ofGetElapsedTimeMicros();
	}

	MeasureScope TimeMeasure::GetMeasureScope(string blockName) {
		auto found = blocks.find(blockName);

		if (found != blocks.end()) return (*found).second;
		else return MeasureScope();

	}


	void TimeMeasure::Report(bool restart) {

		int total = (this->endTime - this->startTime);
		CogLogInfo("Measure", "Report: Total %d ms",total/1000);
		// get longest block
		unsigned int longest = 0;
		int maxCall = 0;
		unsigned int maxDur = 0;
		for(auto& rec : blocks) {
			longest = max(longest, rec.second.name.length());
			maxCall = max(maxCall, rec.second.numCalls);
			maxDur = max(maxDur, rec.second.duration / 1000);
		}

		// calculate number of digits of the highest number of call
		int maxCallLength = ofToString(maxCall).length();
		int maxDurLength = ofToString(maxDur).length();

		for (auto& rec : blocks) {
			MeasureScope& scope = rec.second;
			int percentage = (int)((scope.duration / ((float)total))*100);

			stringstream ss;
			ss.precision(2); // 2 decimal places

			ss << std::setfill(' ') << std::left << std::setw(longest + 1) << scope.name.c_str();
			ss << ":Total[" << std::setfill('0') << std::setw(2) << percentage << "%%]";
			ss << " Calls[" << std::right << std::setw(maxCallLength) << std::setfill('0') << scope.numCalls << "] ";
			ss << " Dur[" << std::right << std::setw(maxDurLength) << std::setfill('0') << scope.duration / 1000 << " ms]";
			ss << " Freq[" << std::right << std::setfill('0') << scope.numCalls / (total / 1000000.0f) << "]";
			CogLogInfo("Measure", ss.str().c_str());
		}
		
		if (restart) this->Restart();
	}

} // namespace