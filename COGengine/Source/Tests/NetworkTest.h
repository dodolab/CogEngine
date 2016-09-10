#pragma once

#include "ofMain.h"
#include "ofxCogMain.h"
#include "Network.h"
#include "NetReader.h"
#include "NetWriter.h"

#ifdef TESTING

using namespace Cog;

class DummyMessage {
public:

	DummyMessage() {

	}

	DummyMessage(int num1, int num2, int num3, bool flag) 
		: num1(num1), num2(num2), num3(num3), flag(flag) {

	}

	int num1;
	int num2;
	int num3;
	bool flag;

	void LoadFromStream(NetReader* reader) {
		num1 = reader->ReadDWord();
		num2 = reader->ReadDWord();
		num3 = reader->ReadDWord();
		flag = reader->ReadBit();
	}

	void SaveToStream(NetWriter* writer) {
		writer->WriteDWord(num1);
		writer->WriteDWord(num2);
		writer->WriteDWord(num3);
		writer->WriteBit(flag);
	}
};

#include "catch.hpp"

TEST_CASE("Network test", "[class]")
{

	SECTION("TcpClient test message")
	{
		CogEngine::GetInstance().Init();
		auto network = GETCOMPONENT(Network);

		// setup server and client (server must go first)
		network->SetupTCPServer(11999,"_");
		network->SetupTCPClient("127.0.0.1", 11999, "_");
		
		auto& server = network->GetTCPServer();
		auto& client = network->GetTCPClient();

		// send message directly (server runs in a separate thread)
		client.send("TEST");

		string msg = "";

		// wait for message from client
		for (int i = 0; i < 10; i++) {
			if (server.isClientConnected(0)) {		
				msg = server.receive(0);
				if(msg.length() > 0) break;
			}
			ofSleepMillis(1000);
		}

		server.close();
		client.close();

		REQUIRE(msg.compare("TEST") == 0);
	}

	SECTION("TcpClient test bytes")
	{
		CogEngine::GetInstance().Init();
		auto network = GETCOMPONENT(Network);

		// setup server and client (server must go first)
		network->SetupTCPServer(11999, "_");
		network->SetupTCPClient("127.0.0.1", 11999, "_");

		auto& server = network->GetTCPServer();
		auto& client = network->GetTCPClient();

		char* bytes = new char[4]{'a','b','c','d'};
		
		client.sendRawBytes(bytes,4);
		
		delete bytes;
		bytes = new char[4];

		// wait for message from client
		for (int i = 0; i < 10; i++) {
			if (server.isClientConnected(0)) {
				if (server.receiveRawBytes(0, bytes, 4) == 4) break;
			}
			ofSleepMillis(1000);
		}

		REQUIRE(bytes[0] == 'a');
		REQUIRE(bytes[1] == 'b');
		REQUIRE(bytes[2] == 'c');
		REQUIRE(bytes[3] == 'd');

		server.close();
		client.close();
		delete bytes;
	}


	SECTION("Udp test message")
	{
		CogEngine::GetInstance().Init();
		auto network = GETCOMPONENT(Network);

		// setup server and client (server must go first)
		network->SetupUDPReceiver(11999, false);
		network->SetupUDPSender("127.0.0.1", 11999, false);

		auto& receiver = network->GetUDPReceiver();
		auto& sender = network->GetUDPSender();

		char* bytes = new char[4]{ 'a','b','c','d' };

		sender.Send(bytes,4);
		delete bytes;
		bytes = new char[4];

		// wait for message from client
		for (int i = 0; i < 10; i++) {
			receiver.Receive(bytes, 4);
			if (bytes[0] == 'a') break;
			ofSleepMillis(1000);
		}

		REQUIRE(bytes[0] == 'a');
		REQUIRE(bytes[1] == 'b');
		REQUIRE(bytes[2] == 'c');
		REQUIRE(bytes[3] == 'd');

		receiver.Close();
		sender.Close();
		delete bytes;
	}

	SECTION("NetworkStream")
	{
		NetWriter* writer = new NetWriter(1000);
		writer->WriteByte(12);
		writer->WriteByte(13);
		writer->WriteByte(14);
		writer->WriteBit(1);
		writer->WriteBit(0);

		unsigned char* data = new unsigned char[3];
		data[0] = 1;
		data[1] = 2;
		data[2] = 3;

		writer->WriteData(data, 3);
		writer->WriteDWord(987654321);
		writer->WriteBit(1);
		writer->WriteBit(0);
		writer->WriteBit(1);
		writer->WriteBit(1);
		writer->WriteDWord(123456789);
		writer->WriteByte(100);
		writer->WriteByte(128);
		writer->WriteByte(255);

		unsigned size=0;
		data = writer->CopyData(size);
		
		NetReader* reader = new NetReader(data, size);

		unsigned char b1 = reader->ReadByte();
		unsigned char b2 = reader->ReadByte();
		unsigned char b3 = reader->ReadByte();
		unsigned char b4 = reader->ReadBit();
		unsigned char b5 = reader->ReadBit();
		data = reader->ReadData(3);

		unsigned int dw1 = reader->ReadDWord();
		unsigned char b6 = reader->ReadBit();
		unsigned char b7 = reader->ReadBit();
		unsigned char b8 = reader->ReadBit();
		unsigned char b9 = reader->ReadBit();
		unsigned int dw2 = reader->ReadDWord();
		unsigned char b10 = reader->ReadByte();
		unsigned char b11 = reader->ReadByte();
		unsigned char b12 = reader->ReadByte();

		REQUIRE(b1 == 12);
		REQUIRE(b2 == 13);
		REQUIRE(b3 == 14);
		REQUIRE(b4 == 1);
		REQUIRE(b5 == 0);
		REQUIRE(data[0] == 1);
		REQUIRE(data[1] == 2);
		REQUIRE(data[2] == 3);
		REQUIRE(dw1 == 987654321);
		REQUIRE(b6 == 1);
		REQUIRE(b7 == 0);
		REQUIRE(b8 == 1);
		REQUIRE(b9 == 1);
		REQUIRE(dw2 == 123456789);
		REQUIRE(b10 == 100);
		REQUIRE(b11 == 128);
		REQUIRE(b12 == 255);
	}

	SECTION("Message sending")
	{
		CogEngine::GetInstance().Init();
		auto network = GETCOMPONENT(Network);

		// setup server and client (server must go first)
		network->SetupUDPReceiver(11999, false);
		network->SetupUDPSender("127.0.0.1", 11999, false);

		auto& receiver = network->GetUDPReceiver();
		auto& sender = network->GetUDPSender();

		// create message
		DummyMessage msg1(12345, 11137, 680, true);
		auto writer = new NetWriter(200);
		msg1.SaveToStream(writer);

		unsigned int size = 0;
		auto data =writer->CopyData(size);

		// send message
		network->SendUDPMessage(1000, 1001, data, size);
		// receive messge
		auto reader = network->ReceiveUDPMessage(1000, 1001, 10000);

		REQUIRE(reader != nullptr);

		// check message data
		if (reader != nullptr) {
			DummyMessage msg2 = DummyMessage();
			msg2.LoadFromStream(reader);

			REQUIRE(msg2.num1 == 12345);
			REQUIRE(msg2.num2 == 11137);
			REQUIRE(msg2.num3 == 680);
			REQUIRE(msg2.flag == true);
		}

		receiver.Close();
		sender.Close();
	}
}

#endif

