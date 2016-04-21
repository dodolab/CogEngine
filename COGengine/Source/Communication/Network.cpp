#include "Network.h"
#include "Error.h"

namespace Cog {


	void Network::SetupTCPSender(string ip, int port, bool nonBlocking) {
		// TcpManager somehow accepts char* instead of const char*
		char* cstr = new char[ip.length() + 1];
		strcpy(cstr, ip.c_str());
		tcpManager.Connect(cstr, port);
		tcpManager.SetNonBlocking(nonBlocking);
	}

	void Network::SetupTCPReceiver(int port, int bufferSize, bool nonBlocking) {
		tcpManager.Bind(port);
		tcpManager.SetNonBlocking(nonBlocking);
		tcpManager.SetReceiveBufferSize(bufferSize);
		this->tcpListenPort = port;
		tcpBufferStream = new NetReader(bufferSize);
	}

	void Network::SendTCPMessage(unsigned int applicationId, spt<NetOutputMessage> msg) {
		NetWriter* writer = PrepareMessage(applicationId, msg);
		auto buffer = writer->GetBuffer();
		tcpManager.Send((char*)buffer, writer->GetUsedBites() / 8);
		delete writer;
	}

	void Network::SendTCPMessage(unsigned int applicationId, NetWriter* writer) {
		NetWriter* writer2 = PrepareMessage(applicationId, writer);
		auto buffer = writer2->GetBuffer();
		tcpManager.Send((char*)buffer, writer2->GetUsedBites() / 8);
		delete writer2;
	}

	NetReader* Network::ReceiveTCPMessage(unsigned int applicationId, int timeoutSec) {
		return ReceiveMessage(applicationId, timeoutSec, MANAGER_TCP);
	}

	spt<NetInputMessage> Network::ReceiveTCPMessage(unsigned int applicationId, int timeoutSec, bool emptyBuffer) {
		return ReceiveMessage(applicationId, timeoutSec, emptyBuffer, MANAGER_TCP);
	}

	void Network::SetupUDPSender(string ip, int port, bool nonBlocking) {
		udpManager.Connect(ip.c_str(), port);
		udpManager.SetNonBlocking(nonBlocking);
	}

	void Network::SetupUDPReceiver(int port, int bufferSize, bool nonBlocking) {
		udpManager.Bind(port);
		udpManager.SetNonBlocking(nonBlocking);
		udpManager.SetReceiveBufferSize(bufferSize);
		this->udpListenPort = port;
		udpBufferStream = new NetReader(bufferSize);
	}

	void Network::SendUDPMessage(unsigned int applicationId, spt<NetOutputMessage> msg) {
		NetWriter* writer = PrepareMessage(applicationId, msg);
		auto buffer = writer->GetBuffer();
		udpManager.Send((char*)buffer, writer->GetUsedBites() / 8);
		delete writer;
	}

	void Network::SendUDPMessage(unsigned int applicationId, NetWriter* writer) {
		NetWriter* writer2 = PrepareMessage(applicationId, writer);
		auto buffer = writer2->GetBuffer();
		udpManager.Send((char*)buffer, writer2->GetUsedBites() / 8);
		delete writer2;
	}

	NetReader* Network::ReceiveUDPMessage(unsigned int applicationId, int timeoutSec) {
		return ReceiveMessage(applicationId, timeoutSec, MANAGER_UDP);
	}

	spt<NetInputMessage> Network::ReceiveUDPMessage(unsigned int applicationId, int timeoutSec, bool emptyBuffer) {
		return ReceiveMessage(applicationId, timeoutSec, emptyBuffer, MANAGER_UDP);
	}

	NetWriter* Network::PrepareMessage(unsigned int applicationId, spt<NetOutputMessage> msg) {
		NetWriter* writer = new NetWriter(msg->GetMessageLength() + 2);
		// write application id and the content
		writer->WriteWord(applicationId);
		msg->SaveToStream(writer);
		return writer;
	}

	NetWriter* Network::PrepareMessage(unsigned int applicationId, NetWriter* writer) {
		NetWriter* writer2 = new NetWriter(writer->GetUsedBites() * 8 + 2);
		// write application id and the content
		writer2->WriteWord(applicationId);
		writer2->WriteBytes(writer->GetBuffer(), writer->GetUsedBites() * 8);
		return writer2;
	}

	NetReader* Network::ReceiveMessage(unsigned int applicationId, int timeoutSec, int managerType) {
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
			if (bytesBuff > 0 && bufferStream->ReadWord() == applicationId) {
				unsigned int size = bytesBuff - 2; // minus just received word
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

	spt<NetInputMessage> Network::ReceiveMessage(unsigned int applicationId, int timeoutSec, bool emptyBuffer, int managerType) {
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


			if (bytesBuff > 0 && bufferStream->ReadWord() == applicationId) {

				// size of content (minus just read word)
				unsigned int size = bytesBuff - 2;

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

	ofHttpResponse Network::HttpGet(string url) {
		ofURLFileLoader loader;
		return loader.get(url);
	}

	ofHttpResponse Network::HttpGet(ofHttpRequest& request) {
		ofURLFileLoader loader;
		return loader.handleRequest(request);
	}

}// namespace