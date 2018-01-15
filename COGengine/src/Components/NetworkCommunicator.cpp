
#include "ofxCogEngine.h"
#include "NetworkCommunicator.h"
#include "NetworkManager.h"
#include "NetMessage.h"
#include "Facade.h"
#include "Utils.h"
#include "ofxNetworkUtils.h"

namespace Cog {

	void NetworkCommunicator::InitBroadcast(ABYTE applicationId, int myPort, int peerPort) {
		Close();

		this->applicationId = applicationId;
		this->peerPort = peerPort;
		this->isHost = false;
		CogLogInfo("Network", "Initialized broadcasting for application %d on ports %d and %d", applicationId, myPort, peerPort);
		network = new NetworkManager();
		network->SetupUDPReceiver(myPort, 10000, true);
		network->GetUDPSender().SetEnableBroadcast(true);
		networkState = NetworkComState::DISCOVERING;
	}

	void NetworkCommunicator::InitListening(ABYTE applicationId, int port) {
		Close();

		this->applicationId = applicationId;
		this->myPort = port;
		this->isHost = true;
		CogLogInfo("Network", "Initialized listening for application %d on port %d", applicationId, port);
		network = new NetworkManager();
		network->SetupUDPReceiver(port, 10000, true);
		networkState = NetworkComState::LISTENING;
	}

	void NetworkCommunicator::PushMessageForSending(spt<NetOutputMessage> msg) {

		if (msg->GetAction() == NET_MSG_UPDATE && msg->GetMsgTime() == 0) {
			throw IllegalArgumentException("Update messages must have time configured");
		}

		// only when the communicator is in COMMUNICATING state there
		// could be sent more than one message at a time
		if (this->networkState != NetworkComState::COMMUNICATING) {
			messagesToSend.clear();
		}

		COGLOGDEBUG("Network_sync", "pushing message for sending: %s ", msg->GetAction().GetStringValue().c_str());
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
				CogLogInfo("Network", "Received discover message from %s:%d", message->GetSourceIp().c_str(), message->GetSourcePort());
				network->SetupUDPSender(message->GetSourceIp(), message->GetSourcePort(), true);
				auto response = spt<NetOutputMessage>();
				if (!messagesToSend.empty()) {
					// if there is a message for sending, send is as a payload, but don't delete it, because during the discovery process 
					// same messages are being sent repeatedly
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
				CogLogInfo("Network", "Connected peer %s", message->GetSourceIp().c_str());
				network->SetupUDPSender(message->GetSourceIp(), message->GetSourcePort(), true);
				this->peerIp = message->GetSourceIp();
				this->peerPort = message->GetSourcePort();
				lastReceivedMsgTime = absolute;
				networkState = NetworkComState::COMMUNICATING;

				// notify other components
				SendMessage(ACT_NET_CONNECTED, spt<NetworkMsgEvent>(new NetworkMsgEvent(message)));

				// send confirmation message
				auto msg = spt<NetOutputMessage>(new NetOutputMessage(1, NetMsgType::CONNECT_RESPONSE));
				messagesToSend.clear();
				network->SendUDPMessage(applicationId, msg);
			}
		}
	}

	void NetworkCommunicator::UpdateDiscovering(const uint64 absolute) {
		// check if it is time to broadcast
		if (CheckTime(lastBroadcastTime, absolute, broadcastingFrequency)) {
			lastBroadcastTime = absolute;

			spt<NetOutputMessage> msg = spt<NetOutputMessage>(new NetOutputMessage(lastReceivedMsgId, NetMsgType::DISCOVER_REQUEST));

			// send broadcast messsages to subnets 192.168, 10.16 and localhost
			COGLOGDEBUG("Network", "Broadcasting");
			network->SetupUDPSender("192.168.0.255", peerPort, true);
			network->SendUDPMessage(applicationId, msg);
			network->SetupUDPSender("10.16.0.255", peerPort, true);
			network->SendUDPMessage(applicationId, msg);
			network->SetupUDPSender("127.0.0.1", peerPort, true);
			network->SendUDPMessage(applicationId, msg);
		}
		else if (CheckTime(lastDiscoveringTime, absolute, connectingFrequency)) {

			lastDiscoveringTime = absolute;

			// check for discover responses
			auto message = network->ReceiveUDPMessage(applicationId, 0, true);
			if (message && (message->GetMsgType() == NetMsgType::DISCOVER_RESPONSE)) {
				CogLogInfo("Network", "Found peer %s", message->GetSourceIp().c_str());

				network->SetupUDPSender(message->GetSourceIp(), peerPort, true);
				// notify other components
				SendMessage(ACT_NET_MESSAGE_RECEIVED, spt<NetworkMsgEvent>(new NetworkMsgEvent(message)));

				// update list of discovered servers
				discoveredPeers[message->GetSourceIp()] = absolute;
				lastReceivedMsgTime = absolute;
				if (autoConnect) ConnectToPeer(message->GetSourceIp());
			}
		}
	}


	void NetworkCommunicator::UpdateConnecting(const uint64 absolute) {

		if (CheckTime(lastConnectingTime, absolute, connectingFrequency)) {
			lastConnectingTime = absolute;
			// send connection request
			auto msg = spt<NetOutputMessage>(new NetOutputMessage(0, NetMsgType::CONNECT_REQUEST));
			network->SendUDPMessage(applicationId, msg);
		}

		if (lastReceivedMsgTime == 0) lastReceivedMsgTime = absolute;

		// check connection response
		auto message = network->ReceiveUDPMessage(applicationId, 0, false);
		if (message && message->GetMsgType() == NetMsgType::CONNECT_RESPONSE) {
			CogLogInfo("Network", "Connected to host %s", message->GetSourceIp().c_str());
			lastReceivedMsgTime = absolute;
			networkState = NetworkComState::COMMUNICATING;
			// notify other components
			SendMessage(ACT_NET_CONNECTED, spt<NetworkMsgEvent>(new NetworkMsgEvent(message)));
		}
		else if (message && message->GetMsgType() == NetMsgType::CONNECT_REQUEST) {
			// both peers can connect at the same time
			CogLogInfo("Network", "Connected peer %s", message->GetSourceIp().c_str());
			lastReceivedMsgTime = absolute;
			auto msg = spt<NetOutputMessage>(new NetOutputMessage(1, NetMsgType::CONNECT_RESPONSE));
			network->SendUDPMessage(applicationId, msg);
			networkState = NetworkComState::COMMUNICATING;
			// notify other components
			SendMessage(ACT_NET_CONNECTED, spt<NetworkMsgEvent>(new NetworkMsgEvent(message)));
		}
		else if ((absolute - lastReceivedMsgTime) > disconnectTimeout * 1000) {
			CogLogInfo("Network", "No message received from peer for %d s, disconnecting...", disconnectTimeout);
			auto msg = spt<NetOutputMessage>(new NetOutputMessage(0, NetMsgType::DISCONNECT));
			network->SendUDPMessage(applicationId, msg);
			networkState = NetworkComState::DISCOVERING;
		}
	}

	void NetworkCommunicator::UpdateCommunicating(const uint64 absolute) {

		// process until there are no received messages 
		while (network != nullptr) {

			auto message = network->ReceiveUDPMessage(applicationId, 0, false);
			
			int err = ofxNetworkCheckError();
			if(err == OFXNETWORK_ERROR(CONNRESET)) {
				COGLOGDEBUG("Network", "Closing port...");
				if(isHost) {
					InitListening(applicationId, myPort);
				}else {
					InitBroadcast(applicationId, myPort, peerPort);
				}
			}
			
			if (!message) break;

			auto type = message->GetMsgType();

			if ((type == NetMsgType::UPDATE || type == NetMsgType::ACCEPT)) {

				ABYTE acceptedMsgId = message->GetAcceptedId();
				if (acceptedMsgId != 0) {
					// got id of accepted message
					COGLOGDEBUG("Network_sync", "received acceptation of %d ", (int)acceptedMsgId);
					unconfirmedMessages.erase(acceptedMsgId);
				}


				lastReceivedMsgTime = absolute;
				bool isUpdateMsg = message->GetAction() == NET_MSG_UPDATE;

				if (type != NetMsgType::ACCEPT) {

					if (!isUpdateMsg && forAcceptationMessageIds.count(message->GetSyncId()) == 0) {
						// got message with synchronization id that must be accepted -> update 
						// collection of messages for acceptation
						COGLOGDEBUG("Network_sync", "received %d ", (int)message->GetSyncId());
						forAcceptationMessageIds.insert(message->GetSyncId());
					}

					if (forAcceptationMessageTimes.count(message->GetMsgTime()) == 0) {

						if (message->GetSyncId() > this->lastReceivedMsgId || (this->lastReceivedMsgId - message->GetSyncId()) > 128
							|| (message->GetAction() != NET_MSG_UPDATE)) {
							// old messages can be still processed but no update messages, because old updates are not important anymore

							// synchronization ids may be in range 0-255 so if the difference is greater than 128 it means that
							// the numbering goes from zero again
							if (message->GetSyncId() > this->lastReceivedMsgId || this->lastReceivedMsgId - message->GetSyncId() > 128)
								lastReceivedMsgId = message->GetSyncId();

							COGLOGDEBUG("Network_sync", "informing listeners");
							SendMessage(ACT_NET_MESSAGE_RECEIVED, spt<NetworkMsgEvent>(new NetworkMsgEvent(message)));
						}

						if (!isUpdateMsg) {
							forAcceptationMessageTimes.insert(message->GetMsgTime());

							if (forAcceptationMessageTimes.size() > 128) {
								// if there is more than 128 messages in the buffer, remove half of the buffer, 
								//because it shouldn't be important anymore

								for (auto it = forAcceptationMessageTimes.begin(); it != forAcceptationMessageTimes.end(); ) {
									if (forAcceptationMessageTimes.size() < 64) break;
									if (*it % 2 == 0) {
										forAcceptationMessageTimes.erase(it++);
									}
									else {
										++it;
									}
								}
							}
						}
					}
				}
			}
			else if (type == NetMsgType::DISCONNECT) {
				CogLogInfo("Network", "Peer %s has disconnected", message->GetSourceIp().c_str());
				networkState = NetworkComState::LISTENING;
				SendMessage(ACT_NET_DISCONNECTED);
			}
			else if (type == NetMsgType::CONNECT_REQUEST) {
				CogLogInfo("Network", "Peer %s is reconnecting", message->GetSourceIp().c_str());
				lastReceivedMsgTime = absolute;
				auto msg = spt<NetOutputMessage>(new NetOutputMessage(1, NetMsgType::CONNECT_RESPONSE));
				// notify other components
				SendMessage(ACT_NET_CONNECTED, spt<NetworkMsgEvent>(new NetworkMsgEvent(message)));
				network->SendUDPMessage(applicationId, msg);
			}
		}

		if (CheckTime(lastSendingTime, absolute, sendingFrequency)) {
			lastSendingTime = absolute;

			// send all messages
			SendMessages(absolute);
		}

		if (!isHost && ((absolute - lastReceivedMsgTime) > reconnectTimeout * 1000)) {
			CogLogInfo("Network", "No message received from host for %d s, reconnecting...", reconnectTimeout);
			networkState = NetworkComState::CONNECTING;
			SendMessage(ACT_NET_CONNECTION_LOST);
		}
	}

	void NetworkCommunicator::SendMessages(uint64 time) {

		int counter = 0;
		if (!messagesToSend.empty()) {

			for (auto& msg : messagesToSend) {
				lastSentMsgId++;
				if (lastSentMsgId == 0) lastSentMsgId++; // zero is for undefined sync messages

				msg->SetSyncId(lastSentMsgId);

				if (!forAcceptationMessageIds.empty()) {
					// if there is at least one id that hasn't been accepted
					// yet, send it with the message

					auto lastItem = prev(forAcceptationMessageIds.end());
					msg->SetAcceptedId(*lastItem);
					forAcceptationMessageIds.erase(lastItem);
				}

				msg->SetMsgType(NetMsgType::UPDATE);

				// updates don't need to be confirmed
				if (msg->GetAction() != NET_MSG_UPDATE) {
					// set time only for action messages
					msg->SetMsgTime(time + (counter++));
					unconfirmedMessages[msg->GetSyncId()] = msg;
				}
				else {
					COGLOGDEBUG("Network_sync", "sending delta %d ", (int)msg->GetSyncId());
					network->SendUDPMessage(applicationId, msg);
				}
			}


			// send all messages that haven't been confirmed
			for (auto& key : unconfirmedMessages) {
				COGLOGDEBUG("Network_sync", "sending msg %d ", (int)key.second->GetSyncId());
				network->SendUDPMessage(applicationId, key.second);
			}

			messagesToSend.clear();
		}
		else if (!forAcceptationMessageIds.empty()) {
			// send the rest acceptation messages separately
			for (auto& acc : forAcceptationMessageIds) {
				auto msg = spt<NetOutputMessage>(new NetOutputMessage(1, NetMsgType::ACCEPT));
				msg->SetMsgTime(time);
				msg->SetAcceptedId(acc);
				COGLOGDEBUG("Network_sync", "confirmating %d ", (int)msg->GetAcceptedId());
				network->SendUDPMessage(applicationId, msg);
			}

			forAcceptationMessageIds.clear();
		}
	}

} // namespace