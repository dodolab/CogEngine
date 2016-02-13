#pragma once

#include "ofMain.h"
#include "ofxCogMain.h"
#include "Network.h"


#ifdef TESTING

using namespace Cog;


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
}

#endif

