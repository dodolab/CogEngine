#include "UpdateMessage.h"

namespace Cog {

	void UpdateMessage::LoadFromStream(NetReader* reader) {
		
		// load both collections
		int contSize = reader->ReadDWord();
		int discrSize = reader->ReadDWord();

		for (int i = 0; i < contSize; i++) {
			unsigned key = reader->ReadDWord();
			float val = reader->ReadFloat();

			continuousValues[key] = val;
		}

		for (int i = 0; i < discreteValues.size(); i++) {
			unsigned key = reader->ReadDWord();
			float val = reader->ReadFloat();

			discreteValues[key] = val;
		}
	}

	void UpdateMessage::SaveToStream(NetWriter* writer) {
		writer->WriteDWord(continuousValues.size());
		writer->WriteDWord(discreteValues.size());

		for (auto& key : continuousValues) {
			writer->WriteDWord(key.first);
			writer->WriteFloat(key.second);
		}

		for (auto& key : discreteValues) {
			writer->WriteDWord(key.first);
			writer->WriteFloat(key.second);
		}
	}



} // namespace