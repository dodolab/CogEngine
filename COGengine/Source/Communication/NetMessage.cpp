#include "NetMessage.h"

namespace Cog {

	
	void NetInputMessage::LoadFromStream(NetReader* reader) {

		this->syncId = reader->ReadByte();
		this->acceptedId = reader->ReadByte();
		this->msgType = (NetMsgType) reader->ReadByte();
		this->action = StringHash(reader->ReadDWord());
		this->msgTime = reader->ReadDWord();

		if (this->dataLength != 0) {
			this->data = new tBYTE[this->dataLength];
			reader->ReadBytes(this->data, this->dataLength);
		}
	}

	void NetOutputMessage::SaveToStream(NetWriter* writer) {
		
		writer->WriteByte(this->syncId);
		writer->WriteByte(this->acceptedId);
		writer->WriteByte((tBYTE)this->msgType);
		writer->WriteDWord(this->action.Value());
		writer->WriteDWord(this->msgTime);

		if (this->data != nullptr) {
			data->SaveToStream(writer);
		}
	}
} // namespace