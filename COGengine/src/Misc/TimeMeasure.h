#pragma once

#include <string>
#include <map>

using namespace std;

namespace Cog {

	/**
	* Measured scope
	*/
	struct MeasureScope {

		MeasureScope() {

		}

		MeasureScope(string name, unsigned long time) 
			: name(name), numCalls(1), duration(0), lastStart(time) {

		}

		// scope name
		string name;
		// number of calls
		int numCalls;
		// total duration
		unsigned duration;
		// last time the scope was called
		unsigned long lastStart;

		/**
		* Records time the thread entered the scope
		*/
		void CallBegin(unsigned long time){
			this->lastStart = time;
			this->numCalls++;
		}

		/**
		* Records time the thread left the scope
		*/
		void CallEnd(unsigned long time) {
			duration += (time - lastStart);
			lastStart = 0;
		}
	};


	/**
	* Measure component for various blocks of code, methods and loops
	*/
	class TimeMeasure
	{
		// measured scopes
		map<string, MeasureScope> blocks;
		unsigned long startTime = 0;
		unsigned long endTime;
	public:

		/**
		* Restarts the component
		*/
		void Restart();

		/**
		* Gets time the time measuring started
		*/
		unsigned long GetStartTime() {
			return startTime;
		}

		/**
		* Gets time the time measuring ended
		*/
		unsigned long GetEndTime() {
			return endTime;
		}

		/**
		* Records time the thread entered a block with given name
		*/
		void MeasureBlockStart(string blockName);

		/**
		* Records time the thread left a block with given name
		*/
		void MeasureBlockEnd(string blockName);

		/**
		* Gets measured block with given name
		*/
		MeasureScope GetMeasureScope(string blockName);

		/**
		* Writes measuring report to the console
		*/
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
		// in better error messages due compiling
		TimeMeasure(TimeMeasure const&) = delete;
		void operator=(TimeMeasure const&) = delete;

	};



} // namespace
