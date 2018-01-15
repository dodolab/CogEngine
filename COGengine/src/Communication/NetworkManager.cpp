#include "NetworkManager.h"
#include "Error.h"

namespace Cog {


	void NetworkManager::SetupTCPSender(string ip, int port, bool nonBlocking) {
		// TcpManager somehow accepts char* instead of const char*
		char* cstr = new char[ip.length() + 1];
		strcpy(cstr, ip.c_str());
		tcpManager.Connect(cstr, port);
		tcpManager.SetNonBlocking(nonBlocking);
	}

	void NetworkManager::SetupTCPReceiver(int port, int bufferSize, bool nonBlocking) {
		tcpManager.Bind(port);
		tcpManager.SetNonBlocking(nonBlocking);
		tcpManager.SetReceiveBufferSize(bufferSize);
		this->tcpListenPort = port;
		tcpBufferStream = new NetReader(bufferSize);
	}

	void NetworkManager::SendTCPMessage(ABYTE applicationId, spt<NetOutputMessage> msg) {
		NetWriter* writer = PrepareMessage(applicationId, msg);
		auto buffer = writer->GetBuffer();
		tcpManager.Send((char*)buffer, writer->GetUsedBites() / 8);
		delete writer;
	}

	void NetworkManager::SendTCPMessage(ABYTE applicationId, NetWriter* writer) {
		NetWriter* writer2 = PrepareMessage(applicationId, writer);
		auto buffer = writer2->GetBuffer();
		tcpManager.Send((char*)buffer, writer2->GetUsedBites() / 8);
		delete writer2;
	}

	NetReader* NetworkManager::ReceiveTCPMessage(ABYTE applicationId, int timeoutSec) {
		return ReceiveMessage(applicationId, timeoutSec, MANAGER_TCP);
	}

	spt<NetInputMessage> NetworkManager::ReceiveTCPMessage(ABYTE applicationId, int timeoutSec, bool emptyBuffer) {
		return ReceiveMessage(applicationId, timeoutSec, emptyBuffer, MANAGER_TCP);
	}

	void NetworkManager::SetupUDPSender(string ip, int port, bool nonBlocking) {
		udpManager.Connect(ip.c_str(), port);
		udpManager.SetNonBlocking(nonBlocking);
	}

	void NetworkManager::SetupUDPReceiver(int port, int bufferSize, bool nonBlocking) {
		udpManager.Bind(port);
		udpManager.SetNonBlocking(nonBlocking);
		udpManager.SetReceiveBufferSize(bufferSize);
		this->udpListenPort = port;
		udpBufferStream = new NetReader(bufferSize);
	}

	void NetworkManager::SendUDPMessage(ABYTE applicationId, spt<NetOutputMessage> msg) {
		NetWriter* writer = PrepareMessage(applicationId, msg);
		auto buffer = writer->GetBuffer();
		udpManager.Send((char*)buffer, writer->GetUsedBites() / 8);
		delete writer;
	}

	void NetworkManager::SendUDPMessage(ABYTE applicationId, NetWriter* writer) {
		NetWriter* writer2 = PrepareMessage(applicationId, writer);
		auto buffer = writer2->GetBuffer();
		udpManager.Send((char*)buffer, writer2->GetUsedBites() / 8);
		delete writer2;
	}

	NetReader* NetworkManager::ReceiveUDPMessage(ABYTE applicationId, int timeoutSec) {
		return ReceiveMessage(applicationId, timeoutSec, MANAGER_UDP);
	}

	spt<NetInputMessage> NetworkManager::ReceiveUDPMessage(ABYTE applicationId, int timeoutSec, bool emptyBuffer) {
		return ReceiveMessage(applicationId, timeoutSec, emptyBuffer, MANAGER_UDP);
	}

	NetWriter* NetworkManager::PrepareMessage(ABYTE applicationId, spt<NetOutputMessage> msg) {
		NetWriter* writer = new NetWriter(msg->GetMessageLength() + 1);
		// write application id and the content
		writer->WriteByte(applicationId);
		msg->SaveToStream(writer);
		return writer;
	}

	NetWriter* NetworkManager::PrepareMessage(ABYTE applicationId, NetWriter* writer) {
		NetWriter* writer2 = new NetWriter(writer->GetUsedBites() * 8 + 1);
		// write application id and the content
		writer2->WriteByte(applicationId);
		writer2->WriteBytes(writer->GetBuffer(), writer->GetUsedBites() * 8);
		return writer2;
	}

	NetReader* NetworkManager::ReceiveMessage(ABYTE applicationId, int timeoutSec, int managerType) {
		auto bufferStream = managerType == MANAGER_TCP ? tcpBufferStream : udpBufferStream;
		
		auto time = ofGetElapsedTimeMillis();
		int timeOutMillis = timeoutSec * 1000;

		NetReader* filledBuffer = nullptr;

		while (true) {
			bufferStream->Reset();
			int bytesBuff = 0;
			if (managerType == MANAGER_TCP) {
				bytesBuff = tcpManager.Receive((char*)bufferStream->GetBuffer(), bufferStream->GetBufferBites() / 8);
			}
			else if (managerType == MANAGER_UDP) {
				bytesBuff = udpManager.Receive((char*)bufferStream->GetBuffer(), bufferStream->GetBufferBites() / 8);
			}
			else throw IllegalOperationException("ManagerType can must be either 0 or 1");

			// check received bytes 
			if (bytesBuff > 0 && bufferStream->ReadByte() == applicationId) {
				unsigned int size = bytesBuff - 1; // minus just received byte
				NetWriter* writer = new NetWriter(size);
				writer->WriteBytes(bufferStream->GetActualPointer(), size);
				filledBuffer = new NetReader(writer->GetBuffer(), size);
				return filledBuffer;
			}

			// check timeout
			if ((ofGetElapsedTimeMillis() - time) > timeOutMillis) {
				return nullptr;
			}
		}
	}

	spt<NetInputMessage> NetworkManager::ReceiveMessage(ABYTE applicationId, int timeoutSec, bool emptyBuffer, int managerType) {
		auto bufferStream = managerType == MANAGER_TCP ? tcpBufferStream : udpBufferStream;

		auto time = ofGetElapsedTimeMillis();
		int timeOutMillis = timeoutSec * 1000;

		spt<NetInputMessage> receivedMsg = spt<NetInputMessage>();

		while (true) {

			bufferStream->Reset();
			int bytesBuff = 0;

			if (managerType == MANAGER_TCP) {
				bytesBuff = tcpManager.Receive((char*)bufferStream->GetBuffer(), bufferStream->GetBufferBites() / 8);
			}
			else if (managerType == MANAGER_UDP) {
				bytesBuff = udpManager.Receive((char*)bufferStream->GetBuffer(), bufferStream->GetBufferBites() / 8);
			}
			else throw IllegalOperationException("ManagerType can must be either 0 or 1");

			if(bytesBuff == -1) {
				// error, return an empty messge
				return spt<NetInputMessage>();
			}

			if (bytesBuff > 0 && bufferStream->ReadByte() == applicationId) {

				// size of content (minus just read byte)
				unsigned int size = bytesBuff - 1;

				receivedMsg = spt<NetInputMessage>(new NetInputMessage(size));
				receivedMsg->LoadFromStream(bufferStream);
				string ipAddress = "";
				int port = 0;
				udpManager.GetRemoteAddr(ipAddress, port);
				receivedMsg->sourceIp = ipAddress;
				receivedMsg->sourcePort = port;

				// if emptyBuffer is set to true, the whole inner buffer will be read and the last message will be returned
				if (!emptyBuffer) return receivedMsg;
				else continue;
			}
			else {
				if (receivedMsg) return receivedMsg;
			}

			if ((ofGetElapsedTimeMillis() - time) > timeOutMillis) {
				return spt<NetInputMessage>();
			}
		}
	}

	ofHttpResponse NetworkManager::HttpGet(string url) {
		ofURLFileLoader loader;
		return loader.get(url);
	}

	ofHttpResponse NetworkManager::HttpGet(ofHttpRequest& request) {
		ofURLFileLoader loader;
		return loader.handleRequest(request);
	}

}// namespace