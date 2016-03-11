#include "NetMessage.h"

namespace Cog {

	int NetMessage::idCounter = 0;

	void NetMessage::LoadFromStream(NetReader* reader) {

		this->id = reader->ReadDWord();
		this->msgType = (NetMsgType) reader->ReadDWord();
		this->action = StringHash(reader->ReadDWord());
		this->msgTime = reader->ReadDWord();
		this->dwordParam = reader->ReadDWord();
		this->floatParam1 = reader->ReadFloat();
		this->floatParam2 = reader->ReadFloat();
		this->floatParam3 = reader->ReadFloat();
		this->dataLength = reader->ReadDWord();
		if (this->dataLength != 0) {
			this->data = new BYTE[this->dataLength];
			reader->ReadBytes(this->data, this->dataLength);
		}
	}

	void NetMessage::SaveToStream(NetWriter* writer) {
		
		if(this->id == 0) this->id = idCounter++;
		writer->WriteDWord(this->id);
		writer->WriteDWord((DWORD)this->msgType);
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