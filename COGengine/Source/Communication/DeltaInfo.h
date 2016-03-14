#pragma once

namespace Cog {

	class DeltaInfo {
	public:
		uint64 time;
		map<StringHash, float> deltas;

		float GetVal(StringHash val) {
			auto found = deltas.find(val);
			return (found != deltas.end()) ? (*found).second : 0;
		}
	};


} // namespace