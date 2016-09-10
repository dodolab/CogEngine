#pragma once

#include "NetMessage.h"
#include "ofxCogCommon.h"

namespace Cog {

	class DeltaMessage : public NetData {
	public:
		map<StringHash, float> vals;

		DeltaMessage() {

		}

		DeltaMessage(spt<DeltaInfo> info) {
			this->vals = info->deltas;
		}

		void LoadFromStream(NetReader* reader) {
			int values = reader->ReadDWord();

			for (int i = 0; i < values; i++) {
				unsigned key = reader->ReadDWord();
				float val = reader->ReadFloat();

				vals[StringHash(key)] = val;
			}
		}

		void SaveToStream(NetWriter* writer) {
			writer->WriteDWord(vals.size());

			for (auto& key : vals) {
				writer->WriteDWord(key.first.Value());
				writer->WriteFloat(key.second);
			}
		}

		int GetDataLength() {
			return vals.size() * 8 + 4;
		}
	};

} // namespace