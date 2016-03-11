
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

		network = new Network();
		network->SetupUDPReceiver(port, 10000, true);
		network->GetUDPManager().SetEnableBroadcast(true);

		if (isServer) {
			CogLogInfo("NETWORK", "Listening on port %d", port);
			networkState = NetworkComState::LISTENING;
		}
		else networkState = NetworkComState::BROADCASTING;
	}

	void NetworkCommunicator::OnMessage(Msg& msg) {

	}

	void NetworkCommunicator::SendNetworkMessage(spt<NetMessage> msg) {
		msgToSent = msg;
	}

	void NetworkCommunicator::Update(const uint64 delta, const uint64 absolute) {

		if (isServer) ServerUpdate(delta, absolute);
		else ClientUpdate(delta, absolute);
	}

	void NetworkCommunicator::ServerUpdate(const uint64 delta, const uint64 absolute) {
		int frame = CogGetFrameCounter();

		if (networkState == NetworkComState::LISTENING) {
			if (frame % 100 == 0) {
				auto message = network->ReceiveUDPMessage(applicationId, 0);
				if (message && message->GetMsgType() == NetMsgType::HANDSHAKE_REQUEST) {
					CogLogInfo("NETWORK", "Connected client %s", message->GetIpAddress().c_str());
					network->SetupUDPSender(message->GetIpAddress(), port, true);
					networkState = NetworkComState::UPDATING_SERVER;
				}
			}
		}
		else if (networkState == NetworkComState::UPDATING_SERVER) {
			if (msgToSent) {
				cout << "sending message" << endl;
				network->SendUDPMessage(applicationId, msgToSent);
				msgToSent = spt<NetMessage>();
			}
		}
	}

	void NetworkCommunicator::ClientUpdate(const uint64 delta, const uint64 absolute) {
		int frame = CogGetFrameCounter();

		if (networkState == NetworkComState::BROADCASTING) {
			if (frame % 100 == 0) {
				spt<NetMessage> msg = spt<NetMessage>(new NetMessage(NetMsgType::HANDSHAKE_REQUEST));
				cout << "broadcasting " << endl;

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
				auto message = network->ReceiveUDPMessage(applicationId, 0);
				if (message && message->GetMsgType() == NetMsgType::UPDATE) {
					CogLogInfo("NETWORK", "Connected to server %s", message->GetIpAddress().c_str());
					network->SetupUDPSender(message->GetIpAddress(), port, true);
					networkState = NetworkComState::UPDATING_CLIENT;
				}
			}
		}
		else if (networkState == NetworkComState::UPDATING_CLIENT) {
			auto message = network->ReceiveUDPMessage(applicationId, 0);
			if (message &&message->GetId() > this->lastReceivedMsgId) {
				// dispose old messages
				this->lastReceivedMsgId = message->GetId();
				SendMessageNoBubbling(StringHash(ACT_NET_MESSAGE_RECEIVED), 0, new NetworkMsgEvent(message), nullptr);
			}
		}
	}

} // namespace