
#include "CogEngine.h"
#include "ofxCogCommon.h"
#include "NetworkCommunicator.h"
#include "Network.h"
#include "NetMessage.h"

namespace Cog {

	void NetworkCommunicator::Init(int applicationId, int port, bool isServer) {
		this->isServer = isServer;
		this->port = port;
		this->applicationId = applicationId;

		if (network != nullptr) {
			network->CloseUDP();
			delete network;
		}

		// init values
		clientIp = "";
		lastReceivedMsgId = -1;
		lastSentMsgId = 0;
		msgToSent = spt<NetOutputMessage>();
		clientLastCallBack = 0;
		serverLastUpdate = 0;
		confirmations.clear();
		mode = NetworkComMode::ESTABLISHING;


		network = new Network();
		// client will always have different port than server
		network->SetupUDPReceiver(isServer ? port : port+1, 10000, true);
		if(!isServer) network->GetUDPSender().SetEnableBroadcast(true);

		if (isServer) {
			CogLogInfo("NETWORK", "Listening on port %d", port);
			networkState = NetworkComState::LISTENING;
		}
		else networkState = NetworkComState::BROADCASTING;
	}

	void NetworkCommunicator::OnMessage(Msg& msg) {

	}

	void NetworkCommunicator::SendNetworkMessage(spt<NetOutputMessage> msg) {
		msgToSent = msg;
	}

	void NetworkCommunicator::Update(const uint64 delta, const uint64 absolute) {
		if (network != nullptr) {
			if (isServer) ServerUpdate(delta, absolute);
			else ClientUpdate(delta, absolute);
		}
	}

	void NetworkCommunicator::ServerUpdate(const uint64 delta, const uint64 absolute) {
		int frame = CogGetFrameCounter();

		if (networkState == NetworkComState::LISTENING) {
			if ((frame-1) % 100 == 0) {
				auto message = network->ReceiveUDPMessage(applicationId, 0, true);
				if (message && message->GetMsgType() == NetMsgType::CONNECT_REQUEST) {
					CogLogInfo("NETWORK", "Connected client %s", message->GetSourceIp().c_str());
					network->SetupUDPSender(message->GetSourceIp(), port+1, true);
					this->clientIp = message->GetSourceIp();

					clientLastCallBack = absolute;
					if (mode == NetworkComMode::ESTABLISHING) {
						networkState = NetworkComState::UPDATING_SERVER;
					}

					// send only confirmation message
					auto msg = spt<NetOutputMessage>(new NetOutputMessage(1, NetMsgType::CONNECT_RESPONSE));
					network->SendUDPMessage(applicationId, msg);

					SendMessageToListeners(StringHash(ACT_NET_CLIENT_CONNECTED), 0, new NetworkMsgEvent(message), nullptr);
				}
			}
		}
		else if (networkState == NetworkComState::UPDATING_SERVER) {
			if (msgToSent) {
				msgToSent->SetId(lastSentMsgId++);
				msgToSent->SetMsgType(NetMsgType::UPDATE);
				msgToSent->SetMsgTime(absolute);

				confirmations[msgToSent->GetId()] = msgToSent;

				// send all messages that haven't been confirmed
				for (auto& key : confirmations) {
					network->SendUDPMessage(applicationId, key.second);
				}
				
				msgToSent = spt<NetOutputMessage>();
			}

			auto message = network->ReceiveUDPMessage(applicationId, 0, false);
			if (message) {
				if (message->GetMsgType() == NetMsgType::CLIENT_CALLBACK) {
					BYTE acceptedMsgId = message->GetId();
					confirmations.erase(acceptedMsgId);
					clientLastCallBack = absolute;
					SendMessageToListeners(StringHash(ACT_NET_MESSAGE_RECEIVED), 0, new NetworkMsgEvent(message), nullptr);
				}
				else if (message->GetMsgType() == NetMsgType::DISCONNECT) {
					CogLogInfo("NETWORK", "Disconnected client %s", message->GetSourceIp().c_str());
				}
			}

			if ((absolute - clientLastCallBack) > 4000) {
				// disconnect client
				CogLogInfo("NETWORK", "Disconnecting client %s", this->clientIp.c_str());
				spt<NetOutputMessage> msg = spt<NetOutputMessage>(new NetOutputMessage(lastSentMsgId++,NetMsgType::DISCONNECT));
				network->SendUDPMessage(applicationId, msg);
				networkState = NetworkComState::LISTENING;
			}
		}
	}

	void NetworkCommunicator::ClientUpdate(const uint64 delta, const uint64 absolute) {
		int frame = CogGetFrameCounter();

		if (networkState == NetworkComState::BROADCASTING) {
			if (frame % 100 == 0) {
				spt<NetOutputMessage> msg = spt<NetOutputMessage>(new NetOutputMessage(lastReceivedMsgId, NetMsgType::CONNECT_REQUEST));

				// send broadcast messsages to 192, 10 and localhost
				MLOGDEBUG("NETWORK", "Broadcasting");
				network->SetupUDPSender("192.168.0.255", port, true);
				network->SendUDPMessage(applicationId, msg);
				network->SetupUDPSender("10.16.0.255", port, true);
				network->SendUDPMessage(applicationId, msg);
				network->SetupUDPSender("127.0.0.1", port, true);
				network->SendUDPMessage(applicationId, msg);
			}
			else {
				auto message = network->ReceiveUDPMessage(applicationId, 0, false);
				if (message && (message->GetMsgType() == NetMsgType::CONNECT_RESPONSE || message->GetMsgType() == NetMsgType::UPDATE)) {
					CogLogInfo("NETWORK", "Connected to server %s", message->GetSourceIp().c_str());
					network->SetupUDPSender(message->GetSourceIp(), port, true);

					if (mode == NetworkComMode::ESTABLISHING) {
						networkState = NetworkComState::UPDATING_CLIENT;
					}
					else {
						SendMessageToListeners(StringHash(ACT_NET_SERVER_CONNECTED), 0, new NetworkMsgEvent(message), nullptr);
					}
					
					serverLastUpdate = absolute;
					this->lastReceivedMsgId = message->GetId();
				}
			}
		}
		else if (networkState == NetworkComState::UPDATING_CLIENT) {
			auto message = network->ReceiveUDPMessage(applicationId, 0, false);

			if (message && (message->GetId() > this->lastReceivedMsgId || (this->lastReceivedMsgId - message->GetId()) > 128)) { // check overflow
				// dispose old messages
				this->lastReceivedMsgId = message->GetId();
				SendMessageToListeners(StringHash(ACT_NET_MESSAGE_RECEIVED), 0, new NetworkMsgEvent(message), nullptr);
				serverLastUpdate = absolute;
			}

			if (!msgToSent) {
				msgToSent = spt<NetOutputMessage>(new NetOutputMessage(lastReceivedMsgId,NetMsgType::CLIENT_CALLBACK));
			}

			msgToSent->SetMsgType(NetMsgType::CLIENT_CALLBACK);
			msgToSent->SetId(lastReceivedMsgId);
			msgToSent->SetMsgTime(absolute);
			network->SendUDPMessage(applicationId, msgToSent);
			msgToSent = spt<NetOutputMessage>();


			if ((absolute - serverLastUpdate) > 4000) {
				// disconnect client
				CogLogInfo("NETWORK", "No message received from server for 4s, disconnecting...");
				confirmations.clear();
				networkState = NetworkComState::BROADCASTING;
			}
		}
	}

} // namespace