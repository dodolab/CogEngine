#pragma once

#include "NetMessage.h"
#include "ofxCogCommon.h"

namespace Cog {

	class DeltaMessage : public NetData {
	public:
		map<int, float> deltas;
		map<int, float> teleports;


		DeltaMessage() {

		}

		DeltaMessage(spt<DeltaInfo> info) {
			this->deltas = info->deltas;
			this->teleports = info->teleports;
		}

		void LoadFromStream(NetReader* reader);

		void SaveToStream(NetWriter* writer);

		int GetDataLength() {
			return (deltas.size()+teleports.size()) * 8 + 4 + 4;
		}
	};

} // namespace