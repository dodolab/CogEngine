
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
		CogLogInfo("NETWORK", "Initialized broadcasting for application %d on ports %d and %d", applicationId, myPort, peerPort);
		network = new Network();
		network->SetupUDPReceiver(myPort, 10000, true);
		network->GetUDPSender().SetEnableBroadcast(true);
		networkState = NetworkComState::DISCOVERING;
	}

	void NetworkCommunicator::InitListening(int applicationId, int port) {
		Close();

		this->applicationId = applicationId;
		this->myPort = port;
		CogLogInfo("NETWORK", "Initialized listening for application %d on port %d", applicationId, port);
		network = new Network();
		network->SetupUDPReceiver(port, 10000, true);
		networkState = NetworkComState::LISTENING;
	}

	void NetworkCommunicator::PushMessageForSending(spt<NetOutputMessage> msg) {

		// more messages at once only for communicating state
		if (this->networkState != NetworkComState::COMMUNICATING) {
			messagesToSend.clear();
		}

		COGLOGDEBUG("NETWORK_SYNC", "pushing message for sending: %s ", msg->GetAction().GetStringValue().c_str());
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
		if (network != nullptr) {
			switch (networkState) {
			case NetworkComState::LISTENING:
				UpdateListening(absolute);
				break;
			case NetworkComState::DISCOVERING:
				UpdateDiscovering(absolute);
				break;
			case NetworkComState::CONNECTING:
				UpdateConnecting(absolute);
				break;
			case NetworkComState::COMMUNICATING:
				UpdateCommunicating(absolute);
				break;
			}
		}
	}

	void NetworkCommunicator::UpdateListening(const uint64 absolute) {

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
				SendMessage(ACT_NET_CLIENT_CONNECTED, spt<NetworkMsgEvent>(new NetworkMsgEvent(message)));
				messagesToSend.clear();

				network->SendUDPMessage(applicationId, msg);
			}
		}
	}

	void NetworkCommunicator::UpdateDiscovering(const uint64 absolute) {

		if (IsProperTime(lastBroadcastTime, absolute, broadcastingFrequency)) {
			lastBroadcastTime = absolute;
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
		else if (IsProperTime(lastDiscoveringTime, absolute, connectingFrequency)) {
			lastDiscoveringTime = absolute;

			// check for discover responses
			auto message = network->ReceiveUDPMessage(applicationId, 0, false);
			if (message && (message->GetMsgType() == NetMsgType::DISCOVER_RESPONSE)) {
				CogLogInfo("NETWORK", "Found peer %s", message->GetSourceIp().c_str());

				network->SetupUDPSender(message->GetSourceIp(), peerPort, true);
				SendMessage(ACT_NET_MESSAGE_RECEIVED, spt<NetworkMsgEvent>(new NetworkMsgEvent(message)));

				// update list of discovered servers
				discoveredPeers[message->GetSourceIp()] = absolute;
			}
		}
	}


	void NetworkCommunicator::UpdateConnecting(const uint64 absolute) {

		if (IsProperTime(lastConnectingTime, absolute, connectingFrequency)) {
			lastConnectingTime = absolute;
			// send connection request
			auto msg = spt<NetOutputMessage>(new NetOutputMessage(0, NetMsgType::CONNECT_REQUEST));
			network->SendUDPMessage(applicationId, msg);
		}

		if (lastReceivedMsgTime == 0) lastReceivedMsgTime = absolute;

		// check connection response
		auto message = network->ReceiveUDPMessage(applicationId, 0, false);
		if (message && message->GetMsgType() == NetMsgType::CONNECT_RESPONSE) {
			CogLogInfo("NETWORK", "Connected to peer %s", message->GetSourceIp().c_str());
			lastReceivedMsgTime = absolute;
			networkState = NetworkComState::COMMUNICATING;
		}
		else if (message &&message->GetMsgType() == NetMsgType::CONNECT_REQUEST) {
			// both peers can connect at the same time
			CogLogInfo("NETWORK", "Connected to peer %s", message->GetSourceIp().c_str());
			lastReceivedMsgTime = absolute;
			auto msg = spt<NetOutputMessage>(new NetOutputMessage(1, NetMsgType::CONNECT_RESPONSE));
			SendMessage(ACT_NET_CLIENT_CONNECTED, spt<NetworkMsgEvent>(new NetworkMsgEvent(message)));
			network->SendUDPMessage(applicationId, msg);
			networkState = NetworkComState::COMMUNICATING;
		}
		else if ((absolute - lastReceivedMsgTime) > 20000) {
			CogLogInfo("NETWORK", "No message received from peer for 20s, disconnecting...");
			auto msg = spt<NetOutputMessage>(new NetOutputMessage(0, NetMsgType::DISCONNECT));
			network->SendUDPMessage(applicationId, msg);
			networkState = NetworkComState::DISCOVERING;
		}
	}

	void NetworkCommunicator::UpdateCommunicating(const uint64 absolute) {


		while (true) {
			auto message = network->ReceiveUDPMessage(applicationId, 0, false);

			if (!message) break;

			auto type = message->GetMsgType();

			if ((type == NetMsgType::UPDATE || type == NetMsgType::ACCEPT)) {

				tBYTE acceptedMsgId = message->GetAcceptedId();
				if (acceptedMsgId != 0) {
					COGLOGDEBUG("NETWORK_SYNC", "received acceptation of %d ", (int)acceptedMsgId);
					unconfirmedMessages.erase(acceptedMsgId);
				}

				lastReceivedMsgTime = absolute;

				if (type != NetMsgType::ACCEPT) {
					if (acceptedMessageIds.count(message->GetSyncId()) == 0) {
						COGLOGDEBUG("NETWORK_SYNC", "received %d ", (int)message->GetSyncId());
						acceptedMessageIds.insert(message->GetSyncId());
					}

					if (acceptedMessageTimes.count(message->GetMsgTime()) == 0) {
						
						if (message->GetSyncId() > this->lastReceivedMsgId || (this->lastReceivedMsgId - message->GetSyncId()) > 128
							|| (message->GetAction() != NET_MSG_DELTA_UPDATE)) { // accept only newest delta messages; but for other messages, we can accept old one
							

							if(message->GetSyncId() > this->lastReceivedMsgId || this->lastReceivedMsgId - message->GetSyncId() > 128) lastReceivedMsgId = message->GetSyncId();
							
							COGLOGDEBUG("NETWORK_SYNC", "informing listeners");
							SendMessage(ACT_NET_MESSAGE_RECEIVED, spt<NetworkMsgEvent>(new NetworkMsgEvent(message)));
						}

						acceptedMessageTimes.insert(message->GetMsgTime());

						if (acceptedMessageTimes.size() > 128) {

							// remove half of collection
							for (auto it = acceptedMessageTimes.begin(); it != acceptedMessageTimes.end(); ) {
								if (acceptedMessageTimes.size() < 64) break;
								if (*it % 2 == 0) {
									acceptedMessageTimes.erase(it++);
								}
								else {
									++it;
								}
							}
						}
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
				SendMessage(ACT_NET_CLIENT_CONNECTED, spt<NetworkMsgEvent>(new NetworkMsgEvent(message)));
				network->SendUDPMessage(applicationId, msg);
			}
		}

		if (IsProperTime(lastSendingTime, absolute, sendingFrequency)) {
			lastSendingTime = absolute;
			// send regular message
			SendUpdateMessage(absolute);
		}

		if ((absolute - lastReceivedMsgTime) > 4000) {
			CogLogInfo("NETWORK", "No message received from peer for 4s, reconnecting...");
			networkState = NetworkComState::CONNECTING;
		}

	}

	void NetworkCommunicator::SendUpdateMessage(uint64 time) {

		int counter = 0;
		if (!messagesToSend.empty()) {

			for (auto& msg : messagesToSend) {
				lastSentMsgId++;
				if (lastSentMsgId == 0) lastSentMsgId++; // zero is for undefined sync messages

				msg->SetSyncId(lastSentMsgId);

				if (!acceptedMessageIds.empty()) {
					auto lastItem = prev(acceptedMessageIds.end());

					msg->SetAcceptedId(*lastItem);
					acceptedMessageIds.erase(lastItem);
				}

				msg->SetMsgType(NetMsgType::UPDATE);
				msg->SetMsgTime(time + (counter++));

				// delta updates don't need to be confirmed
				if (msg->GetAction() != NET_MSG_DELTA_UPDATE) {
					unconfirmedMessages[msg->GetSyncId()] = msg;
				}
				else {
					COGLOGDEBUG("NETWORK_SYNC", "sending delta %d ",(int)msg->GetSyncId());
					network->SendUDPMessage(applicationId, msg);
				}
			}


			// send all messages that haven't been confirmed
			for (auto& key : unconfirmedMessages) {
				COGLOGDEBUG("NETWORK_SYNC", "sending msg %d ",(int)key.second->GetSyncId());
				network->SendUDPMessage(applicationId, key.second);
			}

			messagesToSend.clear();
		}
		else if (!acceptedMessageIds.empty()) {
			// send all acceptation messages
			for (auto& acc : acceptedMessageIds) {
				auto msg = spt<NetOutputMessage>(new NetOutputMessage(1, NetMsgType::ACCEPT));
				msg->SetMsgTime(time);
				msg->SetAcceptedId(acc);
				COGLOGDEBUG("NETWORK_SYNC", "confirmating %d ", (int)msg->GetAcceptedId());
				network->SendUDPMessage(applicationId, msg);
			}

			acceptedMessageIds.clear();
		}

	}

} // namespace