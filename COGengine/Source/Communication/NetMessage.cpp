#include "NetMessage.h"

namespace Cog {

	BYTE NetMessage::idCounter = 0;

	void NetMessage::LoadFromStream(NetReader* reader) {

		this->id = reader->ReadByte();
		this->msgType = (NetMsgType) reader->ReadByte();
		this->action = StringHash(reader->ReadDWord());
		this->msgTime = reader->ReadDWord();
		this->dwordParam = reader->ReadDWord();
		this->floatParam1 = reader->ReadFloat();
		this->floatParam2 = reader->ReadFloat();
		this->floatParam3 = reader->ReadFloat();
		this->dataLength = reader->ReadDWord();

		LoadDataFromStream(reader);
	}

	void NetMessage::SaveToStream(NetWriter* writer) {
		
		if(this->id == 0) this->id = idCounter++;
		writer->WriteByte(this->id);
		writer->WriteByte((BYTE)this->msgType);
		writer->WriteDWord(this->action.Value());
		writer->WriteDWord(this->msgTime);
		writer->WriteDWord(this->dwordParam);
		writer->WriteFloat(this->floatParam1);
		writer->WriteFloat(this->floatParam2);
		writer->WriteFloat(this->floatParam3);
		writer->WriteDWord(this->dataLength);

		if (dataLength != 0) {
			writer->WriteBytes(this->data, this->dataLength);
		}
	}

} // namespace