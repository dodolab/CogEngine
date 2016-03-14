#pragma once

namespace Cog {

	class DeltaInfo {
	public:
		uint64 time;
		map<StringHash, float> deltas;
		map<StringHash, float> teleports;

		float GetDelta(StringHash val) {
			auto found = deltas.find(val);
			return (found != deltas.end()) ? (*found).second : 0;
		}
		
		float GetTeleport(StringHash val) {
			auto found = teleports.find(val);
			return (found != teleports.end()) ? (*found).second : 0;
		}

		float GetVal(StringHash val) {
			auto foundDelta = deltas.find(val);

			if (foundDelta != deltas.end()) return (*foundDelta).second;

			auto foundTelep = teleports.find(val);
			return (foundTelep != teleports.end()) ? (*foundTelep).second : 0;
		}
	};


} // namespace