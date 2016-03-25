#include "DeltaMessage.h"

namespace Cog {

	void DeltaMessage::LoadFromStream(NetReader* reader) {
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

	void DeltaMessage::SaveToStream(NetWriter* writer) {
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



} // namespace