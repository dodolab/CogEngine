
#include "TimeMeasure.h"
#include "ofMain.h"

namespace Cog {

	extern void CogLogInfo(const char* module, const char* format, ...);

	void TimeMeasure::Restart() {
		blocks.clear();
		this->startTime = ofGetElapsedTimeMicros();
		this->endTime = ofGetElapsedTimeMicros();
	}

	void TimeMeasure::MeasureBlockStart(string blockName) {
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
		CogLogInfo("Measure", "Report:: total %d ms",total/1000);
		for (auto& rec : blocks) {
			MeasureScope& scope = rec.second;
			int percentage = (int)((scope.duration / ((float)total))*100);

			CogLogInfo("Measure", "%s:Total[%d %%],Calls[%d],Dur[%d ms],Freq[%.2f]", 
				scope.name.c_str(), percentage, scope.numCalls, scope.duration/1000, scope.numCalls/(total/1000000.0f));
		}

		if (restart) this->Restart();
	}

} // namespace