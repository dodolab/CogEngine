#pragma once

#include "NetReader.h"
#include "NetWriter.h"
#include "StringHash.h"
#include "DeltaInfo.h"

namespace Cog {

	enum class NetMsgType {
		CONNECT_REQUEST = 1,
		UPDATE = 2,
		CLIENT_CALLBACK = 3,
		DISCONNECT = 4,
		CONNECT_RESPONSE = 5
	};

	class NetData {
	public:
		virtual void LoadFromStream(NetReader* reader) = 0;

		virtual void SaveToStream(NetWriter* writer) = 0;

		virtual int GetDataLength() = 0;
	};

	class NetInputMessage {
	private:
		string sourceIp;
		int sourcePort;
		BYTE id = 0;
		NetMsgType msgType = NetMsgType::CLIENT_CALLBACK;
		StringHash action = StringHash();
		DWORD msgTime = 0;
		BYTE* data = nullptr;
		int dataLength = 0;

	public:
		NetInputMessage(int messageLength){
			this->dataLength = messageLength - GetHeaderLength();
		}

		NetInputMessage(int messageLength, int id) : NetInputMessage(messageLength) {
			this->id = id;
		}

		NetInputMessage(int messageLength, int id, NetMsgType msgType) : NetInputMessage(messageLength, id) {
			this->msgType = msgType;
		}

		~NetInputMessage() {
			delete[] data;
		}

		string GetSourceIp() {
			return sourceIp;
		}

		void SetSourceIp(string ip) {
			this->sourceIp = ip;
		}

		int GetSourcePort() {
			return sourcePort;
		}

		void SetSourcePort(int sourcePort) {
			this->sourcePort = sourcePort;
		}

		BYTE GetId() {
			return id;
		}

		void SetId(BYTE id) {
			this->id = id;
		}

		int GetMessageLength() {
			return GetHeaderLength() + GetDataLength();
		}

		int GetHeaderLength() {
			return 	1 // id
				+ 1 // msgType
				+ 4 // action
				+ 4; // msgTime
		}

		NetMsgType GetMsgType() {
			return msgType;
		}

		void SetMsgType(NetMsgType msgType) {
			this->msgType = msgType;
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
	};



	class NetOutputMessage {
	private:
		BYTE id = 0;
		NetMsgType msgType = NetMsgType::CLIENT_CALLBACK;
		StringHash action = StringHash();
		DWORD msgTime = 0;
		NetData* data = nullptr;

	public:
		NetOutputMessage(int id) : id(id) {

		}

		NetOutputMessage(int id, NetMsgType msgType) : id(id), msgType(msgType) {

		}

		~NetOutputMessage() {
			//delete data;
		}


		BYTE GetId() {
			return id;
		}

		void SetId(BYTE id) {
			this->id = id;
		}

		int GetMessageLength() {
			return (data == nullptr ? 0 : data->GetDataLength())
				+ 1 // id
				+ 1 // msgType
				+ 4 // action
				+ 4; // msgTime

		}

		NetMsgType GetMsgType() {
			return msgType;
		}

		void SetMsgType(NetMsgType msgType) {
			this->msgType = msgType;
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

		NetData* GetData() {
			return data;
		}

		void SetData(NetData* data) {
			this->data = data;
		}
	
		void SaveToStream(NetWriter* writer);

	};

} // namespace