#pragma once

#include <string>
#include <map>

using namespace std;

namespace Cog {

	struct MeasureBlock {

		MeasureBlock() {

		}

		MeasureBlock(string name, unsigned long time) : name(name), numCalls(1), duration(0), lastStart(time) {

		}

		string name;
		int numCalls;
		unsigned duration;
		unsigned long lastStart;

		void CallBegin(unsigned long time){
			this->lastStart = time;
			this->numCalls++;
		}

		void CallEnd(unsigned long time) {
			duration += (time - lastStart);
			lastStart = 0;
		}
	};


	// simple time measuring class
	class TimeMeasure
	{
		map<string, MeasureBlock> blocks;
		unsigned long startTime;
		unsigned long endTime;
	public:

		void Restart();

		unsigned long GetStartTime() {
			return startTime;
		}

		unsigned long GetEndTime() {
			return endTime;
		}

		void MeasureBlockStart(string blockName);

		void MeasureBlockEnd(string blockName);

		MeasureBlock GetMeasureBlock(string blockName);

		void Report(bool restart);

    // ================================= SINGLETON PART ==========================
	public:
		static TimeMeasure& GetInstance()
		{
			// guaranteed to be destroyed,
			// instantiated on first use
			static TimeMeasure instance;
			return instance;
		}
	private:
		TimeMeasure() {}
	public:
		// deleted functions should be public as it results
		// int better error messages due compiling
		TimeMeasure(TimeMeasure const&) = delete;
		void operator=(TimeMeasure const&) = delete;

	};



} // namespace
