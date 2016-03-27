
#include "CogEngine.h"
#include "ofxCogCommon.h"
#include "NetworkCommunicator.h"
#include "Network.h"
#include "NetMessage.h"

namespace Cog {


	void NetworkCommunicator::InitBroadcast(int applicationId, int myPort, int peerPort) {
		Close();

		this->applicationId = applicationId;
		this->peerPort = peerPort;
		CogLogInfo("NETWORK", "Initialized broadcasting for application %? on ports %d and %d", applicationId, myPort, peerPort);
		network = new Network();
		network->SetupUDPReceiver(myPort, 10000, true);
		network->GetUDPSender().SetEnableBroadcast(true);
		networkState = NetworkComState::DISCOVERING;
	}

	void NetworkCommunicator::InitListening(int applicationId, int port) {
		Close();

		this->applicationId = applicationId;
		this->myPort = port;
		CogLogInfo("NETWORK", "Initialized listening for application %? on port %d", applicationId, port);
		network = new Network();
		network->SetupUDPReceiver(port, 10000, true);
		networkState = NetworkComState::LISTENING;
	}

	void NetworkCommunicator::PushMessageForSending(spt<NetOutputMessage> msg) {

		// more messages at once only for communicating state
		if (this->networkState != NetworkComState::COMMUNICATING) {
			messagesToSend.clear();
		}

		cout << "pushing msg of type " << StringHash::GetStringValue(msg->GetAction()).c_str() << endl;
		messagesToSend.push_back(msg);
	}

	void NetworkCommunicator::Close() {
		if (network != nullptr) {
			network->CloseUDP();
			delete network;
			network = nullptr;
		}

		lastReceivedMsgId = -1;
		lastSentMsgId = 0;
		messagesToSend.clear();
		lastReceivedMsgTime = 0;
		discoveredPeers.clear();
		unconfirmedMessages.clear();
	}


	void NetworkCommunicator::ConnectToPeer(string ip) {
		this->peerIp = ip;
		network->SetupUDPSender(peerIp, peerPort, true);
		networkState = NetworkComState::CONNECTING;
	}

	void NetworkCommunicator::OnMessage(Msg& msg) {

	}


	void NetworkCommunicator::Update(const uint64 delta, const uint64 absolute) {
		int frame = CogGetFrameCounter();

		switch (networkState) {
		case NetworkComState::LISTENING:
			UpdateListening(frame, absolute);
			break;
		case NetworkComState::DISCOVERING:
			UpdateDiscovering(frame, absolute);
			break;
		case NetworkComState::CONNECTING:
			UpdateConnecting(frame, absolute);
			break;
		case NetworkComState::COMMUNICATING:
			UpdateCommunicating(frame, absolute);
			break;
		}
	}

	void NetworkCommunicator::UpdateListening(int frame, const uint64 absolute) {

		if ((frame - 1) % 100 == 0) {
			auto message = network->ReceiveUDPMessage(applicationId, 0, true);

			if (message) {
				if (message->GetMsgType() == NetMsgType::DISCOVER_REQUEST) {
					// received discover request -> send discover response
					CogLogInfo("NETWORK", "Received discover message from %s:%d", message->GetSourceIp().c_str(), message->GetSourcePort());
					network->SetupUDPSender(message->GetSourceIp(), message->GetSourcePort(), true);
					auto response = spt<NetOutputMessage>();
					if (!messagesToSend.empty()) {
						// if there is a message for sending, send is as a payload, but don't delete it, because the discover process is static
						response = messagesToSend.back();
						response->SetMsgTime(absolute);
						response->SetMsgType(NetMsgType::DISCOVER_RESPONSE);
					}
					else {
						response = spt<NetOutputMessage>(new NetOutputMessage(1, NetMsgType::DISCOVER_RESPONSE));
					}

					network->SendUDPMessage(applicationId, response);
				}
				else if (message->GetMsgType() == NetMsgType::CONNECT_REQUEST) {
					// received connect request -> send connect response and switch to communicating state
					CogLogInfo("NETWORK", "Connected peer %s", message->GetSourceIp().c_str());
					network->SetupUDPSender(message->GetSourceIp(), message->GetSourcePort(), true);
					this->peerIp = message->GetSourceIp();
					this->peerPort = message->GetSourcePort();
					lastReceivedMsgTime = absolute;
					networkState = NetworkComState::COMMUNICATING;

					// send confirmation message
					auto msg = spt<NetOutputMessage>(new NetOutputMessage(1, NetMsgType::CONNECT_RESPONSE));
					SendMessageToListeners(StringHash(ACT_NET_CLIENT_CONNECTED), 0, new NetworkMsgEvent(message), nullptr);
					messagesToSend.clear();

					network->SendUDPMessage(applicationId, msg);
				}
			}
		}
	}

	void NetworkCommunicator::UpdateDiscovering(int frame, const uint64 absolute) {

		if (frame % 100 == 0) {
			// send broadcast every nth frame
			spt<NetOutputMessage> msg = spt<NetOutputMessage>(new NetOutputMessage(lastReceivedMsgId, NetMsgType::DISCOVER_REQUEST));

			// send broadcast messsages to 192, 10 and localhost
			COGLOGDEBUG("NETWORK", "Broadcasting");
			network->SetupUDPSender("192.168.0.255", peerPort, true);
			network->SendUDPMessage(applicationId, msg);
			network->SetupUDPSender("10.16.0.255", peerPort, true);
			network->SendUDPMessage(applicationId, msg);
			network->SetupUDPSender("127.0.0.1", peerPort, true);
			network->SendUDPMessage(applicationId, msg);
		}
		else {
			// check for discover responses
			auto message = network->ReceiveUDPMessage(applicationId, 0, false);
			if (message && (message->GetMsgType() == NetMsgType::DISCOVER_RESPONSE)) {
				CogLogInfo("NETWORK", "Found peer %s", message->GetSourceIp().c_str());

				network->SetupUDPSender(message->GetSourceIp(), peerPort, true);
				SendMessageToListeners(StringHash(ACT_NET_MESSAGE_RECEIVED), 0, new NetworkMsgEvent(message), nullptr);

				// update list of discovered servers
				discoveredPeers[message->GetSourceIp()] = absolute;
			}
		}
	}


	void NetworkCommunicator::UpdateConnecting(int frame, const uint64 absolute) {

		// send connection request
		auto msg = spt<NetOutputMessage>(new NetOutputMessage(0, NetMsgType::CONNECT_REQUEST));
		network->SendUDPMessage(applicationId, msg);

		if (lastReceivedMsgTime == 0) lastReceivedMsgTime = absolute;

		// check connection response
		auto message = network->ReceiveUDPMessage(applicationId, 0, false);
		if (message && message->GetMsgType() == NetMsgType::CONNECT_RESPONSE) {
			CogLogInfo("NETWORK", "Connected to peer %s", message->GetSourceIp().c_str());
			lastReceivedMsgTime = absolute;
			networkState = NetworkComState::COMMUNICATING;
		}
		else if(message &&message->GetMsgType() == NetMsgType::CONNECT_REQUEST) {
			// both peers can connect at the same time
			CogLogInfo("NETWORK", "Connected to peer %s", message->GetSourceIp().c_str());
			lastReceivedMsgTime = absolute;
			auto msg = spt<NetOutputMessage>(new NetOutputMessage(1, NetMsgType::CONNECT_RESPONSE));
			SendMessageToListeners(StringHash(ACT_NET_CLIENT_CONNECTED), 0, new NetworkMsgEvent(message), nullptr);
			network->SendUDPMessage(applicationId, msg);
			networkState = NetworkComState::COMMUNICATING;
		}
		else if ((absolute - lastReceivedMsgTime) > 40000) {
			CogLogInfo("NETWORK", "No message received from peer for 40s, disconnecting...");
			auto msg = spt<NetOutputMessage>(new NetOutputMessage(lastReceivedMsgId, NetMsgType::DISCONNECT));
			network->SendUDPMessage(applicationId, msg);
			networkState = NetworkComState::DISCOVERING;
		}
	}

	void NetworkCommunicator::UpdateCommunicating(int frame, const uint64 absolute) {
		
		auto message = network->ReceiveUDPMessage(applicationId, 0, false);
		
		if (message) {
			
			auto type = message->GetMsgType();

			if ((type == NetMsgType::UPDATE || type == NetMsgType::ACCEPT)) { 

				tBYTE acceptedMsgId = message->GetAcceptedId();
				unconfirmedMessages.erase(acceptedMsgId);
				
				if (type != NetMsgType::ACCEPT) {
					if (find(acceptedMessages.begin(), acceptedMessages.end(), lastReceivedMsgId) == acceptedMessages.end()) {
						acceptedMessages.push_back(lastReceivedMsgId);
					}
				}

				if (message->GetSyncId() > this->lastReceivedMsgId || (this->lastReceivedMsgId - message->GetSyncId()) > 128) {
					lastReceivedMsgTime = absolute;

					if (type != NetMsgType::ACCEPT) {
						lastReceivedMsgId = message->GetSyncId();

						SendMessageToListeners(StringHash(ACT_NET_MESSAGE_RECEIVED), 0, new NetworkMsgEvent(message), nullptr);
					}
				}
			}
			else if (type == NetMsgType::DISCONNECT) {
				CogLogInfo("NETWORK", "Peer %s has disconnected", message->GetSourceIp().c_str());
				Close();
			}
			else if (type == NetMsgType::CONNECT_REQUEST) {
				CogLogInfo("NETWORK", "Peer %s is reconnecting", message->GetSourceIp().c_str());
				lastReceivedMsgTime = absolute;
				auto msg = spt<NetOutputMessage>(new NetOutputMessage(1, NetMsgType::CONNECT_RESPONSE));
				SendMessageToListeners(StringHash(ACT_NET_CLIENT_CONNECTED), 0, new NetworkMsgEvent(message), nullptr);
				network->SendUDPMessage(applicationId, msg);
			}
		}

		// send regular message
		SendUpdateMessage(absolute);

		if ((absolute - lastReceivedMsgTime) > 4000) {
			CogLogInfo("NETWORK", "No message received from peer for 4s, reconnecting...");
			networkState = NetworkComState::CONNECTING;
		}
	}

	void NetworkCommunicator::SendUpdateMessage(uint64 time) {
		if (!messagesToSend.empty()) {

			for (auto& msg : messagesToSend) {
				msg->SetSyncId(lastSentMsgId++);
				
				if (!acceptedMessages.empty()) {
					msg->SetAcceptedId(acceptedMessages.back());
					acceptedMessages.erase(acceptedMessages.end() - 1);
				}
				
				msg->SetMsgType(NetMsgType::UPDATE);
				msg->SetMsgTime(time);
				
				// delta updates don't need to be confirmed
				if (msg->GetAction() != NET_MSG_DELTA_UPDATE) {
					unconfirmedMessages[msg->GetSyncId()] = msg;
				}
				else {
					cout << "sending delta" << endl;
					network->SendUDPMessage(applicationId, msg);
				}
			}

			// send all messages that haven't been confirmed
			for (auto& key : unconfirmedMessages) {
				if ((time - key.second->GetMsgTime()) < 20 || (time - key.second->GetMsgTime()) > 100) {
					network->SendUDPMessage(applicationId, key.second);
				}
			}

			messagesToSend.clear();
		}
		else if(!acceptedMessages.empty()) {
			// send all acceptation messages
			for (auto& acc : acceptedMessages) {
				auto msg = spt<NetOutputMessage>(new NetOutputMessage(1, NetMsgType::ACCEPT));
				msg->SetMsgTime(time);
				msg->SetAcceptedId(acc);
				network->SendUDPMessage(applicationId, msg);
			}

			acceptedMessages.clear();
		}
	}

} // namespace