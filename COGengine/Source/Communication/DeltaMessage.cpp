#include "DeltaMessage.h"

namespace Cog {

	void DeltaMessage::LoadFromStream(NetReader* reader) {
		
		// load both collections
		int deltaSize = reader->ReadDWord();
		int teleportSize = reader->ReadDWord();

		for (int i = 0; i < deltaSize; i++) {
			unsigned key = reader->ReadDWord();
			float val = reader->ReadFloat();

			deltas[key] = val;
		}

		for (int i = 0; i < teleportSize; i++) {
			unsigned key = reader->ReadDWord();
			float val = reader->ReadFloat();

			teleports[key] = val;
		}
	}

	void DeltaMessage::SaveToStream(NetWriter* writer) {
		writer->WriteDWord(deltas.size());
		writer->WriteDWord(teleports.size());

		for (auto& key : deltas) {
			writer->WriteDWord(key.first);
			writer->WriteFloat(key.second);
		}

		for (auto& key : teleports) {
			writer->WriteDWord(key.first);
			writer->WriteFloat(key.second);
		}
	}



} // namespace