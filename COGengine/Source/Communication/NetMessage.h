#pragma once

#include "NetReader.h"
#include "NetWriter.h"
#include "StringHash.h"
#include "DeltaInfo.h"

namespace Cog {

	enum class NetMsgType {
		DISCOVER_REQUEST = 1,
		DISCOVER_RESPONSE = 2,
		CONNECT_REQUEST = 3,
		CONNECT_RESPONSE = 4,
		UPDATE = 5,
		CALLBACK_UPDATE = 6,
		DISCONNECT = 7
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
		tBYTE id = 0;
		NetMsgType msgType = NetMsgType::CALLBACK_UPDATE;
		StringHash action = StringHash();
		tDWORD msgTime = 0;
		tBYTE* data = nullptr;
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

		tBYTE GetId() {
			return id;
		}

		void SetId(tBYTE id) {
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

		tDWORD GetMsgTime() {
			return msgTime;
		}

		void SetMsgTime(tDWORD time) {
			this->msgTime = time;
		}

		StringHash GetAction() {
			return action;
		}

		void SetAction(StringHash action) {
			this->action = action;
		}

		tBYTE* GetData() {
			return data;
		}

		int GetDataLength() {
			return dataLength;
		}

		void SetData(tBYTE* data, int dataLength) {
			this->data = data;
			this->dataLength = dataLength;
		}

		void LoadFromStream(NetReader* reader);
	};



	class NetOutputMessage {
	private:
		tBYTE id = 0;
		NetMsgType msgType = NetMsgType::CALLBACK_UPDATE;
		StringHash action = StringHash();
		tDWORD msgTime = 0;
		NetData* data = nullptr;

	public:
		NetOutputMessage(int id) : id(id) {

		}

		NetOutputMessage(int id, NetMsgType msgType) : id(id), msgType(msgType) {

		}

		~NetOutputMessage() {
			//delete data;
		}


		tBYTE GetId() {
			return id;
		}

		void SetId(tBYTE id) {
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

		tDWORD GetMsgTime() {
			return msgTime;
		}

		void SetMsgTime(tDWORD time) {
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