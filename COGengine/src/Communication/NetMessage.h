#pragma once

#include "Definitions.h"
#include "NetReader.h"
#include "NetWriter.h"
#include "StrId.h"

namespace Cog {

	/**
	* Type of network message
	*/
	enum class NetMsgType {
		DISCOVER_REQUEST = 1,
		DISCOVER_RESPONSE = 2,
		CONNECT_REQUEST = 3,
		CONNECT_RESPONSE = 4,
		UPDATE = 5,
		ACCEPT = 6,
		DISCONNECT = 7
	};

	/**
	* Abstract base class for entities that are part of network message
	* as a payload; both NetInputMessage and NetOutputMessage can contain
	* a payload, consisting of this object
	*/
	class NetData {
	public:
		/**
		* Loads object from stream
		*/
		virtual void LoadFromStream(NetReader* reader) = 0;

		/**
		* Saves objects to stream
		*/
		virtual void SaveToStream(NetWriter* writer) = 0;

		/**
		* Gets length of the data, very important for serialization
		*/
		virtual int GetDataLength() = 0;
	};

	/**
	* Network message that arrived from remote point
	*/
	class NetInputMessage {
	private:
		// source ip address
		string sourceIp;
		// source port
		int sourcePort;
		// synchronization id
		ABYTE syncId = 0;
		// acceptation id (only for messages sent from clients to host)
		ABYTE acceptedId = 0;
		// type of message
		NetMsgType msgType = NetMsgType::UPDATE;
		// type of action
		StrId action = StrId();
		// the time at which the message was created
		ADWORD msgTime = 0;
		// data payload
		ABYTE* data = nullptr;
		// length of data payload
		int dataLength = 0;

	public:
		/**
		* Creates a new input message 
		* @param messageLength length of the data payload
		*/
		NetInputMessage(int messageLength){
			this->dataLength = messageLength - GetHeaderLength();
		}

		/**
		* Creates a new input message
		* @param messageLength length of the data payload
		* @param syncId synchronization id
		*/
		NetInputMessage(int messageLength, ABYTE syncId) : NetInputMessage(messageLength) {
			this->syncId = syncId;
		}

		/**
		* Creates a new input message
		* @param messageLength length of the data payload
		* @param syncId synchronization id
		* @param msgType type of the message
		*/
		NetInputMessage(int messageLength, ABYTE syncId, NetMsgType msgType)
			: NetInputMessage(messageLength, syncId) {
			this->msgType = msgType;
		}

		~NetInputMessage() {
			delete[] data;
		}

		/**
		* Gets source ip address
		*/
		string GetSourceIp() const {
			return sourceIp;
		}

		/**
		* Gets source port
		*/
		int GetSourcePort() const {
			return sourcePort;
		}

		/**
		* Gets synchronization id
		*/
		ABYTE GetSyncId() const {
			return syncId;
		}

		/**
		* Gets acceptation id
		*/
		ABYTE GetAcceptedId() const {
			return acceptedId;
		}

		/**
		* Gets type of the message
		*/
		NetMsgType GetMsgType() const {
			return msgType;
		}

		/**
		* Gets message time
		*/
		ADWORD GetMsgTime() const {
			return msgTime;
		}

		/**
		* Gets type of action
		*/
		StrId GetAction() const {
			return action;
		}

		/**
		* Gets data payload
		*/
		ABYTE* GetData() const {
			return data;
		}

		/**
		* Gets data payload by type, using the LoadFromStream method
		* The type of the data payload should be determined by the type of the message
		* together with the type of the action
		*/
		template<class T>
		spt<T> GetData() {
			auto netReader = new NetReader(data, GetDataLength());
			spt<T> innerMsg = spt<T>(new T());
			innerMsg->LoadFromStream(netReader);
			return innerMsg;
		}

		/**
		* Gets length of the whole message in bytes
		*/
		int GetMessageLength() const {
			return GetHeaderLength() + GetDataLength();
		}

		/**
		* Gets length of the header in bytes
		*/
		int GetHeaderLength() const {
			return 	1 // sync id
				+ 1 // accepted id
				+ 1 // msgType
				+ 4 // action
				+ 4; // msgTime
		}

		/**
		* Gets length of the data payload in bytes
		*/
		int GetDataLength() const {
			return dataLength;
		}


		/**
		* Loads the message from stream
		*/
		void LoadFromStream(NetReader* reader);

		friend class NetworkManager;
	};


	/**
	* Network message that is serialized into stream of bytes and sent
	* to the destination point
	*/
	class NetOutputMessage {
	private:
		// synchronization id
		ABYTE syncId = 0;
		// acceptation id
		ABYTE acceptedId = 0;
		// type of message
		NetMsgType msgType = NetMsgType::UPDATE;
		// type of action
		StrId action = StrId();
		// time in which the message was sent
		ADWORD msgTime = 0;
		// data payload
		NetData* data = nullptr;

	public:

		/**
		* Creates a new output message
		* @param syncId synchronization id
		*/
		NetOutputMessage(ABYTE syncId) : syncId(syncId) {

		}

		/**
		* Creates a new output message
		* @param syncId synchronization id
		* @param msgType type of the message
		*/
		NetOutputMessage(ABYTE syncId, NetMsgType msgType) 
			: syncId(syncId), msgType(msgType) {

		}

		~NetOutputMessage() {
			delete data;
		}

		/**
		* Gets synchronization id
		*/
		ABYTE GetSyncId() const {
			return syncId;
		}

		/**
		* Sets synchronization id
		*/
		void SetSyncId(ABYTE syncId) {
			this->syncId = syncId;
		}

		/**
		* Gets acceptation id
		*/
		ABYTE GetAcceptedId() const {
			return acceptedId;
		}

		/**
		* Sets acceptation id
		*/
		void SetAcceptedId(ABYTE id) {
			this->acceptedId = id;
		}

		/**
		* Gets type of the message
		*/
		NetMsgType GetMsgType() const {
			return msgType;
		}

		/**
		* Sets type of the message
		*/
		void SetMsgType(NetMsgType msgType) {
			this->msgType = msgType;
		}

		/**
		* Gets the time in which the message was sent
		*/
		ADWORD GetMsgTime() const {
			return msgTime;
		}

		/**
		* Sets the time in which the message will be sent
		*/
		void SetMsgTime(ADWORD time) {
			this->msgTime = time;
		}

		/**
		* Gets type of the action
		*/
		StrId GetAction() const {
			return action;
		}

		/**
		* Sets type of the action
		*/
		void SetAction(StrId action) {
			this->action = action;
		}

		/**
		* Gets data payload
		*/
		NetData* GetData() const {
			return data;
		}

		/**
		* Sets data payload
		*/
		void SetData(NetData* data) {
			this->data = data;
		}

		/**
		* Gets length of the message, including data payload (if attached)
		*/
		int GetMessageLength() {
			return (data == nullptr ? 0 : data->GetDataLength())
				+ 1 // sync id
				+ 1 // accepted id
				+ 1 // msgType
				+ 4 // action
				+ 4; // msgTime

		}
	
		/**
		* Saves data into stream
		*/
		void SaveToStream(NetWriter* writer);
	};

} // namespace