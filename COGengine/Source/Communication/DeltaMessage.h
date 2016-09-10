#pragma once

#include "NetMessage.h"
#include "ofxCogCommon.h"

namespace Cog {

	class DeltaMessage : public NetData {
	public:
		map<StringHash, float> deltas;
		map<StringHash, float> teleports;


		DeltaMessage() {

		}

		DeltaMessage(spt<DeltaInfo> info) {
			this->deltas = info->deltas;
			this->teleports = info->teleports;
		}

		void LoadFromStream(NetReader* reader) {
			int deltaSize = reader->ReadDWord();
			int teleportSize = reader->ReadDWord();

			for (int i = 0; i < deltaSize; i++) {
				unsigned key = reader->ReadDWord();
				float val = reader->ReadFloat();

				deltas[StringHash(key)] = val;
			}

			for (int i = 0; i < teleportSize; i++) {
				unsigned key = reader->ReadDWord();
				float val = reader->ReadFloat();

				teleports[StringHash(key)] = val;
			}
		}

		void SaveToStream(NetWriter* writer) {
			writer->WriteDWord(deltas.size());
			writer->WriteDWord(teleports.size());

			for (auto& key : deltas) {
				writer->WriteDWord(key.first.Value());
				writer->WriteFloat(key.second);
			}

			for (auto& key : teleports) {
				writer->WriteDWord(key.first.Value());
				writer->WriteFloat(key.second);
			}
		}

		int GetDataLength() {
			return (deltas.size()+teleports.size()) * 8 + 4 + 4;
		}
	};

} // namespace