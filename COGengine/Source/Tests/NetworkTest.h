#pragma once

#include "ofMain.h"
#include "ofxCogMain.h"
#include "Network.h"
#include "NetReader.h"
#include "NetWriter.h"
#include "NetMessage.h"
#include "DeltaUpdate.h"
#include "NetworkCommunicator.h"

#define TESTING

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
		auto network = new Network();

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
		delete network;
	}

	SECTION("TcpClient test bytes")
	{
		auto network = new Network();

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
		delete network;
	}

	
	SECTION("Udp test message")
	{
		auto network = new Network();

		// setup server and client (server must go first)
		network->SetupUDPReceiver(11999, 1000,true);
		network->SetupUDPSender("127.0.0.1", 11999, true);

		auto& receiver = network->GetUDPReceiver();
		auto& sender = network->GetUDPSender();

		char* bytes = new char[4]{ 'a','b','c','d' };

		sender.Send(bytes,4);
		//delete bytes;
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
		delete network;
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

		writer->WriteBytes(data, 3);
		writer->WriteString("mojo_message");
		writer->WriteDWord(987654321);
		writer->WriteBit(1);
		writer->WriteBit(0);
		writer->WriteBit(1);
		writer->WriteBit(1);
		writer->WriteDWord(123456789);
		writer->WriteByte(100);
		writer->WriteByte(128);
		writer->WriteFloat(12.1234f);
		writer->WriteByte(255);
		writer->WriteDWord(7800); writer->WriteDWord(10300);

		unsigned size=0;
		data = writer->CopyData(size);
		
		NetReader* reader = new NetReader(data, size);

		unsigned char b1 = reader->ReadByte();
		unsigned char b2 = reader->ReadByte();
		unsigned char b3 = reader->ReadByte();
		unsigned char b4 = reader->ReadBit();
		unsigned char b5 = reader->ReadBit();
		
		data = reader->ReadBytes(3);
		string msg = reader->ReadString();

		unsigned int dw1 = reader->ReadDWord();
		unsigned char b6 = reader->ReadBit();
		unsigned char b7 = reader->ReadBit();
		unsigned char b8 = reader->ReadBit();
		unsigned char b9 = reader->ReadBit();
		unsigned int dw2 = reader->ReadDWord();
		unsigned char b10 = reader->ReadByte();
		unsigned char b11 = reader->ReadByte();
		float f1 = reader->ReadFloat();
		unsigned char b12 = reader->ReadByte();

		unsigned int ints[2];
		reader->ReadDWords(ints, 2);

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
		REQUIRE(isEqual(f1,12.1234f));
		REQUIRE(b12 == 255);
		REQUIRE(ints[0] == 7800);
		REQUIRE(ints[1] == 10300);
	}

	SECTION("Custom message sending")
	{
		auto network = new Network();
		// setup server and client (server must go first)
		network->SetupUDPReceiver(11999, 10000, false);
		network->SetupUDPSender("127.0.0.1", 11999, false);

		auto& receiver = network->GetUDPReceiver();
		auto& sender = network->GetUDPSender();

		// create message
		DummyMessage msg1(12345, 11137, 680, true);
		auto writer = new NetWriter(200);
		msg1.SaveToStream(writer);

		// send message
		network->SendUDPMessage(1000, writer);
		// receive messge
		auto reader = network->ReceiveUDPMessage(1000, 10000);

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
		delete network;
	}

	SECTION("Net message sending")
	{
		auto network = new Network();
		// setup server and client (server must go first)
		network->SetupUDPReceiver(11999, 10000, false);
		network->SetupUDPSender("127.0.0.1", 11999, false);

		auto& receiver = network->GetUDPReceiver();
		auto& sender = network->GetUDPSender();

		// create message
		DummyMessage msg1(12345, 11137, 680, true);
		auto writer = new NetWriter(200);
		msg1.SaveToStream(writer);

		auto msg = spt<NetOutputMessage>(new NetOutputMessage(12));
		msg->SetAction(StringHash("MOJO"));
		msg->SetMsgTime(12345);
		msg->SetMsgType(NetMsgType::CLIENT_CALLBACK);

		// send message
		network->SendUDPMessage(1000,msg);
		// receive messge
		auto inputMsg = network->ReceiveUDPMessage(1000, 10000, false);

		REQUIRE(inputMsg);

		// check message data
		if (inputMsg) {
			REQUIRE(inputMsg->GetAction() == StringHash("MOJO"));
			REQUIRE(inputMsg->GetMsgTime() == 12345);
			REQUIRE(inputMsg->GetMsgType() == NetMsgType::CLIENT_CALLBACK);
			REQUIRE(inputMsg->GetId() == 12);
		}

		receiver.Close();
		sender.Close();
		delete network;
	}

	SECTION("DeltaUpdate test without networking")
	{
		DeltaUpdate* delta = new DeltaUpdate();
		delta->actual = spt<DeltaInfo>(new DeltaInfo());

		// time[10] = 10
		spt<DeltaInfo> deltaInf = spt<DeltaInfo>(new DeltaInfo(10));
		deltaInf->deltas[StringHash("MOJO")] = 10;
		delta->AcceptDeltaUpdate(deltaInf);
		
		// time[20] = 20
		deltaInf = spt<DeltaInfo>(new DeltaInfo(20));
		deltaInf->deltas[StringHash("MOJO")] = 20;
		delta->AcceptDeltaUpdate(deltaInf);

		// check that time[15] = 15
		delta->Update(5, 0);
		REQUIRE(((int)delta->actual->deltas[StringHash("MOJO")]) == 15);

		// time[30] = 20
		deltaInf = spt<DeltaInfo>(new DeltaInfo(30));
		deltaInf->deltas[StringHash("MOJO")] = 20;
		delta->AcceptDeltaUpdate(deltaInf);

		// check that time[30] = 20
		delta->Update(15, 0);
		REQUIRE(((int)delta->actual->deltas[StringHash("MOJO")]) == 20);

		// time[60] = 70
		deltaInf = spt<DeltaInfo>(new DeltaInfo(60));
		deltaInf->deltas[StringHash("MOJO")] = 70;
		deltaInf->time = 60;
		delta->AcceptDeltaUpdate(deltaInf);

		// check that time[40] >= 36 && time[40] <= 37
		delta->Update(10, 0);
		REQUIRE(((int)delta->actual->deltas[StringHash("MOJO")]) >= 36);
		REQUIRE(((int)delta->actual->deltas[StringHash("MOJO")]) <= 37);
	}

	SECTION("Lost packet test")
	{
		auto netCom = new NetworkCommunicator();
		REGISTER_COMPONENT(netCom);
		// init engine
		CogEngine::GetInstance().SetFps(20);
		CogEngine::GetInstance().Init();
		Scene* scene = new Scene("main", false);
		CogEngine::GetInstance().stage->AddScene(scene, true);
		CogEngine::GetInstance().stage->GetRootObject()->SubmitChanges(true);

		// start server
		netCom->Init(1000, 12345, true);
		
		// start client
		Network* network = new Network();
		network->SetupUDPSender("127.0.0.1", 12345, true);
		network->SetupUDPReceiver(12345, 1000, true);

		// send message to server
		auto msg = spt<NetOutputMessage>(new NetOutputMessage(1));
		msg->SetMsgType(NetMsgType::CONNECT_REQUEST);
		network->SendUDPMessage(1000, msg);
		// update
		CogEngine::GetInstance().ResetFrameCounter();
		CogEngine::GetInstance().Update(10, 10);
		// client should be connected
		REQUIRE(netCom->ClientConnected());

	/*	// send message to client
		auto clMsg = spt<NetOutputMessage>(new NetOutputMessage(2));
		clMsg->SetMsgType(NetMsgType::UPDATE);
		clMsg->SetMsgTime(98798);
		netCom->SendNetworkMessage(clMsg);
		CogEngine::GetInstance().Update(10, 10);

		auto msgFromServer = network->ReceiveUDPMessage(1000, 5000, false);
		REQUIRE(msgFromServer);

		if (msgFromServer) {
			REQUIRE(msgFromServer->GetMsgTime() == 98798);
		}*/
	}
}

#endif

