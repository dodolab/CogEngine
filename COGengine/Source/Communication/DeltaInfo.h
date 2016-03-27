#pragma once

namespace Cog {

	class DeltaInfo {
	public:

		DeltaInfo() {

		}

		DeltaInfo(uint64 time) : time(time){

		}

		uint64 time;
		map<int, float> deltas;
		map<int, float> teleports;

		float GetDelta(int val) {
			auto found = deltas.find(val);
			return (found != deltas.end()) ? (*found).second : 0;
		}
		
		float GetTeleport(int val) {
			auto found = teleports.find(val);
			return (found != teleports.end()) ? (*found).second : 0;
		}

		float GetVal(int val) {
			auto foundDelta = deltas.find(val);

			if (foundDelta != deltas.end()) return (*foundDelta).second;

			auto foundTelep = teleports.find(val);
			return (foundTelep != teleports.end()) ? (*foundTelep).second : 0;
		}
	};


} // namespace