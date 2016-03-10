#pragma once

#include "NetReader.h"
#include "NetWriter.h"
#include "StringHash.h"

namespace Cog {

	enum class NetMsgType {
		DELTA_UPDATE = 1
	};

	class NetMessage {
	private:
		// identifier counter
		static int idCounter;
		// id of this message
		int id = 0;
		// message type
		NetMsgType msgType;
		// id of action that has been invoked
		StringHash action;
		DWORD msgTime; // time the message was sent
		DWORD param; // custom parameter
		BYTE* data = nullptr;
		int dataLength = 0;

	public:

		NetMessage() {

		}

		NetMessage(NetMsgType type, StringHash action) : msgType(type), action(action) {

		}

		~NetMessage() {
			delete[] data;
		}

		int GetId() {
			return id;
		}

		NetMsgType GetMsgType() {
			return msgType;
		}

		void SetMsgType(NetMsgType msgType) {
			this->msgType = msgType;
		}

		DWORD GetParameter() {
			return param;
		}

		void SetParameter(DWORD param) {
			this->param = param;
		}

		DWORD GetMsgTime() {
			return msgTime;
		}

		void SetMsgTime(DWORD time) {
			this->msgTime = time;
		}

		StringHash GetAction() {
			return action;
		}

		void SetAction(StringHash action) {
			this->action = action;
		}

		BYTE* GetData() {
			return data;
		}

		int GetDataLength() {
			return dataLength;
		}

		void SetData(BYTE* data, int dataLength) {
			this->data = data;
			this->dataLength = dataLength;
		}
		
		void LoadFromStream(NetReader* reader);

		void SaveToStream(NetWriter* writer);
	};

} // namespace