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

		string ipAddress;
		int port;
		// id of this message
		int id = 0;
		// message type
		NetMsgType msgType;
		// id of action that has been invoked
		StringHash action;
		DWORD msgTime = 0; // time the message was sent
		DWORD dwordParam = 0; // custom parameter
		float floatParam1 = 0;
		float floatParam2 = 0;
		float floatParam3 = 0;
		BYTE* data = nullptr;
		int dataLength = 0;


	public:

		NetMessage() {

		}

		NetMessage(NetMsgType type, StringHash action) : 
			ipAddress(ipAddress), port(port), msgType(type), action(action) {

		}

		~NetMessage() {
			delete[] data;
		}

		string GetIpAddress() {
			return ipAddress;
		}

		void SetIpAddress(string address) {
			this->ipAddress = address;
		}

		int GetPort() {
			return port;
		}

		void SetPort(int port) {
			this->port = port;
		}

		int GetId() {
			return id;
		}

		int GetMessageLength(){
			return GetDataLength() + 256;
		}

		NetMsgType GetMsgType() {
			return msgType;
		}

		void SetMsgType(NetMsgType msgType) {
			this->msgType = msgType;
		}

		DWORD GetDWORDParameter() {
			return dwordParam;
		}

		void SetDWORDParameter(DWORD param) {
			this->dwordParam = param;
		}

		float GetFloatParameter1() {
			return floatParam1;
		}

		void SetFloatParameter1(float param) {
			this->floatParam1 = param;
		}

		float GetFloatParameter2() {
			return floatParam2;
		}

		void SetFloatParameter2(float param) {
			this->floatParam2 = param;
		}

		float GetFloatParameter3() {
			return floatParam3;
		}

		void SetFloatParameter3(float param) {
			this->floatParam3 = param;
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