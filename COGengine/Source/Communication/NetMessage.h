#pragma once

#include "ofxSmartPointer.h"
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
		ACCEPT = 6,
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
		tBYTE syncId = 0;
		tBYTE acceptedId = 0;
		NetMsgType msgType = NetMsgType::UPDATE;
		StringHash action = StringHash();
		tDWORD msgTime = 0;
		tBYTE* data = nullptr;
		int dataLength = 0;

	public:
		NetInputMessage(int messageLength){
			this->dataLength = messageLength - GetHeaderLength();
		}

		NetInputMessage(int messageLength, int syncId) : NetInputMessage(messageLength) {
			this->syncId = syncId;
		}

		NetInputMessage(int messageLength, int syncId, NetMsgType msgType) : NetInputMessage(messageLength, syncId) {
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

		tBYTE GetSyncId() {
			return syncId;
		}

		void SetSyncId(tBYTE id) {
			this->syncId = syncId;
		}

		tBYTE GetAcceptedId() {
			return acceptedId;
		}

		void SetAcceptedId(tBYTE id) {
			this->acceptedId = id;
		}

		int GetMessageLength() {
			return GetHeaderLength() + GetDataLength();
		}

		int GetHeaderLength() {
			return 	1 // sync id
				+ 1 // accepted id
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

		template<class T>
		spt<T> GetData() {
			auto netReader = new NetReader(data, GetDataLength());
			spt<T> innerMsg = spt<T>(new T());
			innerMsg->LoadFromStream(netReader);
			return innerMsg;
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
		tBYTE syncId = 0;
		tBYTE acceptedId = 0;
		NetMsgType msgType = NetMsgType::UPDATE;
		StringHash action = StringHash();
		tDWORD msgTime = 0;
		NetData* data = nullptr;

	public:
		NetOutputMessage(int syncId) : syncId(syncId) {

		}

		NetOutputMessage(int syncId, NetMsgType msgType) : syncId(syncId), msgType(msgType) {

		}

		~NetOutputMessage() {
			delete data;
		}

		tBYTE GetSyncId() {
			return syncId;
		}

		void SetSyncId(tBYTE syncId) {
			this->syncId = syncId;
		}

		tBYTE GetAcceptedId() {
			return acceptedId;
		}

		void SetAcceptedId(tBYTE id) {
			this->acceptedId = id;
		}

		int GetMessageLength() {
			return (data == nullptr ? 0 : data->GetDataLength())
				+ 1 // sync id
				+ 1 // accepted id
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