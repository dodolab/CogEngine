
#include "CogEngine.h"
#include "ofxCogCommon.h"
#include "NetworkCommunicator.h"
#include "Network.h"
#include "NetMessage.h"

namespace Cog {

	// ==================================== CLIENT ==========================================

	void ClientCommunicator::SetMessageForSending(spt<NetOutputMessage> msg) {
		msgToSent = msg;
	}

	void ClientCommunicator::ConnectToServer() {
		networkState = ClientComState::CONNECTING;
	}

	void ClientCommunicator::ConnectToServer(string ip) {
		this->serverIp = ip;
		network->SetupUDPSender(serverIp, serverPort, true);
		networkState = ClientComState::CONNECTING;
	}

	void ClientCommunicator::Update(const uint64 delta, const uint64 absolute) {
		int frame = CogGetFrameCounter();

		if (networkState == ClientComState::DISCOVERING) {
			if (frame % 100 == 0) {
				Broadcast();
			}
			else {
				auto message = network->ReceiveUDPMessage(applicationId, 0, false);
				if (message && (message->GetMsgType() == NetMsgType::DISCOVER_RESPONSE)) {
					CogLogInfo("NETWORK", "Found server %s", message->GetSourceIp().c_str());
					network->SetupUDPSender(message->GetSourceIp(), serverPort, true);
					netcom->SendMessageToListeners(StringHash(ACT_NET_SERVER_FOUND), 0, new NetworkMsgEvent(message), nullptr);
					// update list of discovered servers
					discoveredServers[message->GetSourceIp()] = absolute;
					lastReceivedMsgTime = absolute;
					this->lastReceivedMsgId = message->GetId();
				}
			}
		}
		else if (networkState == ClientComState::CONNECTING) {
			// send connection request
			auto msg = spt<NetOutputMessage>(new NetOutputMessage(lastReceivedMsgId, NetMsgType::CONNECT_REQUEST));
			network->SendUDPMessage(applicationId, msg);

			// check connection response
			auto message = network->ReceiveUDPMessage(applicationId, 0, false);
			if (message && message->GetMsgType() == NetMsgType::CONNECT_RESPONSE) {
				CogLogInfo("NETWORK", "Connected to server %s", message->GetSourceIp().c_str());
				networkState = ClientComState::COMMUNICATING;
			}
			else if ((absolute - lastReceivedMsgTime) > 4000) {
				CogLogInfo("NETWORK", "No message received from server for 4s, disconnecting...");
				auto msg = spt<NetOutputMessage>(new NetOutputMessage(lastReceivedMsgId, NetMsgType::DISCONNECT));
				network->SendUDPMessage(applicationId, msg);
				networkState = ClientComState::DISCOVERING;
			}
		}
		else if (networkState == ClientComState::COMMUNICATING) {
			
			auto message = network->ReceiveUDPMessage(applicationId, 0, false);

			if (message && (message->GetId() > this->lastReceivedMsgId || (this->lastReceivedMsgId - message->GetId()) > 128)) { // check overflow																									 // dispose old messages
				this->lastReceivedMsgId = message->GetId();
				netcom->SendMessageToListeners(StringHash(ACT_NET_MESSAGE_RECEIVED), 0, new NetworkMsgEvent(message), nullptr);
				lastReceivedMsgTime = absolute;
			}

			// send regular message
			SendCallbackMessage(absolute);
			
			if ((absolute - lastReceivedMsgId) > 4000) {
				CogLogInfo("NETWORK", "No message received from server for 4s, reconnecting...");
				networkState = ClientComState::CONNECTING;
			}
		}
	}

	void ClientCommunicator::Broadcast() {
		spt<NetOutputMessage> msg = spt<NetOutputMessage>(new NetOutputMessage(lastReceivedMsgId, NetMsgType::DISCOVER_REQUEST));

		// send broadcast messsages to 192, 10 and localhost
		COGLOGDEBUG("NETWORK", "Broadcasting");
		network->SetupUDPSender("192.168.0.255", serverPort, true);
		network->SendUDPMessage(applicationId, msg);
		network->SetupUDPSender("10.16.0.255", serverPort, true);
		network->SendUDPMessage(applicationId, msg);
		network->SetupUDPSender("127.0.0.1", serverPort, true);
		network->SendUDPMessage(applicationId, msg);
	}

	void ClientCommunicator::SendCallbackMessage(uint64 time) {
		if (!msgToSent) {
			msgToSent = spt<NetOutputMessage>(new NetOutputMessage(lastReceivedMsgId, NetMsgType::CALLBACK_UPDATE));
		}
		msgToSent->SetMsgType(NetMsgType::CALLBACK_UPDATE);
		msgToSent->SetId(lastReceivedMsgId);
		msgToSent->SetMsgTime(time);
		network->SendUDPMessage(applicationId, msgToSent);
		msgToSent = spt<NetOutputMessage>();
	}

	// ==================================== SERVER ==========================================

	void ServerCommunicator::SetMessageForSending(spt<NetOutputMessage> msg) {
		msgToSent = msg;
	}

	void ServerCommunicator::Update(const uint64 delta, const uint64 absolute) {
		int frame = CogGetFrameCounter();

		if (networkState == ServerComState::LISTENING) {
			if ((frame - 1) % 100 == 0) {
				auto message = network->ReceiveUDPMessage(applicationId, 0, true);

				if (message) {
					if (message->GetMsgType() == NetMsgType::DISCOVER_REQUEST) {
						CogLogInfo("NETWORK", "Received discover message from %s:%d", message->GetSourceIp().c_str(), message->GetSourcePort());
						network->SetupUDPSender(message->GetSourceIp(), message->GetSourcePort(), true);
						auto msg = spt<NetOutputMessage>(new NetOutputMessage(1, NetMsgType::DISCOVER_RESPONSE));
						network->SendUDPMessage(applicationId, msg);
					}
					else if (message->GetMsgType() == NetMsgType::CONNECT_REQUEST) {
						CogLogInfo("NETWORK", "Connected client %s", message->GetSourceIp().c_str());
						network->SetupUDPSender(message->GetSourceIp(), message->GetSourcePort(), true);
						this->clientIp = message->GetSourceIp();
						this->clientPort = message->GetSourcePort();
						lastReceivedMsgTime = absolute;
						networkState = ServerComState::COMMUNICATING;

						// send confirmation message
						auto msg = spt<NetOutputMessage>(new NetOutputMessage(1, NetMsgType::CONNECT_RESPONSE));
						network->SendUDPMessage(applicationId, msg);
						netcom->SendMessageToListeners(StringHash(ACT_NET_CLIENT_CONNECTED), 0, new NetworkMsgEvent(message), nullptr);
					}
				}
			}
		}
		else if (networkState == ServerComState::COMMUNICATING) {
			SendUpdateMessage(absolute);

			auto message = network->ReceiveUDPMessage(applicationId, 0, false);
			if (message) {
				if (message->GetMsgType() == NetMsgType::CALLBACK_UPDATE) {
					tBYTE acceptedMsgId = message->GetId();
					// update confirmation table
					confirmations.erase(acceptedMsgId);
					lastReceivedMsgTime = absolute;
					netcom->SendMessageToListeners(StringHash(ACT_NET_MESSAGE_RECEIVED), 0, new NetworkMsgEvent(message), nullptr);
				}
				else if (message->GetMsgType() == NetMsgType::DISCONNECT) {
					CogLogInfo("NETWORK", "Client %s has disconnected", message->GetSourceIp().c_str());
					networkState = ServerComState::LISTENING;
				}
			}

			if ((absolute - lastReceivedMsgTime) > 4000) {
				// disconnect client
				CogLogInfo("NETWORK", "No message received from client for 4s, disconnecting ");
				spt<NetOutputMessage> msg = spt<NetOutputMessage>(new NetOutputMessage(lastSentMsgId++, NetMsgType::DISCONNECT));
				network->SendUDPMessage(applicationId, msg);
				networkState = ServerComState::LISTENING;
			}
		}
	}

	void ServerCommunicator::SendUpdateMessage(uint64 time) {
		if (msgToSent) {
			msgToSent->SetId(lastSentMsgId++);
			msgToSent->SetMsgType(NetMsgType::UPDATE);
			msgToSent->SetMsgTime(time);

			confirmations[msgToSent->GetId()] = msgToSent;

			// send all messages that haven't been confirmed
			for (auto& key : confirmations) {
				network->SendUDPMessage(applicationId, key.second);
			}

			msgToSent = spt<NetOutputMessage>();
		}
	}

	// ==================================== NETWORK ==========================================


	void NetworkCommunicator::InitClient(int applicationId, int clientPort, int serverPort) {
		if (client != nullptr) CloseClient();

		network = new Network();
		network->SetupUDPReceiver(clientPort, 10000, true);
		network->GetUDPSender().SetEnableBroadcast(true);
		client = new ClientCommunicator(applicationId, clientPort, serverPort, network, this);
	}

	void NetworkCommunicator::InitServer(int applicationId, int port) {
		if(server != nullptr) CloseServer();

		network = new Network();
		network->SetupUDPReceiver(port, 10000, true);
		server = new ServerCommunicator(applicationId, port, network, this);
	}

	void NetworkCommunicator::CloseClient() {
		if (network != nullptr) {
			delete client;
			client = nullptr;
			network->CloseUDP();
			delete network;
			network = nullptr;
		}
	}

	void NetworkCommunicator::CloseServer() {
		if (network != nullptr) {
			delete server;
			server = nullptr;
			network->CloseUDP();
			delete network;
			network = nullptr;
		}
	}


	void NetworkCommunicator::OnMessage(Msg& msg) {

	}

	void NetworkCommunicator::Update(const uint64 delta, const uint64 absolute) {
		if (client != nullptr) client->Update(delta, absolute);

		if (server != nullptr) server->Update(delta, absolute);
	}


} // namespace