#pragma once

#include "ofxSmartPointer.h"
#include "Component.h"
#include <unordered_set>


namespace Cog {

	class NetOutputMessage;
	class Network;

	enum class NetworkComState {
		NONE,
		LISTENING,
		DISCOVERING,
		CONNECTING,
		COMMUNICATING
	};

	class NetworkCommunicator : public Component {
		OBJECT(NetworkCommunicator)

	private:
		Network* network = nullptr;
		NetworkComState networkState = NetworkComState::NONE;
		int applicationId;
		int myPort;
		int peerPort;
		string peerIp = "";

		vector<tBYTE> acceptedMessages;
		tBYTE lastReceivedMsgId = -1;
		tBYTE lastSentMsgId = 0;
		vector<spt<NetOutputMessage>> messagesToSend;
		uint64 lastReceivedMsgTime = 0;
		map<string, uint64> discoveredPeers;
		map<tBYTE, spt<NetOutputMessage>> unconfirmedMessages;

		// frequency the broadcast is sent, per second
		float broadcastingFrequency = 1;
		// frequency the discover is made, per second
		float discoveringFrequency = 4;
		// frequency the messages are sent, per second
		float sendingFrequency = 10;
		// frequency the (re)connecting is made, per second
		float connectingFrequency = 2;
		
		uint64 lastBroadcastTime = 0;
		uint64 lastDiscoveringTime = 0;
		uint64 lastSendingTime = 0;
		uint64 lastConnectingTime = 0;

	public:

		void InitBroadcast(int applicationId, int myPort, int peerPort);

		void InitListening(int applicationId, int myPort);

		NetworkComState GetNetworkState() {
			return networkState;
		}


		int GetApplicationId() {
			return applicationId;
		}

		int GetMyPort() {
			return myPort;
		}

		int GetPeerPort() {
			return peerPort;
		}

		string GetPeerIp() {
			return peerIp;
		}

		uint64 GetLastReceivedMsgTime() {
			return lastReceivedMsgTime;
		}

		map<string, uint64>& GetDiscoveredPeers() {
			return discoveredPeers;
		}

		float GetBroadcastingFrequency() {
			return broadcastingFrequency;
		}

		void SetBroadcastingFrequency(float frequency) {
			this->broadcastingFrequency = frequency;
		}

		float GetSendingFrequency() {
			return sendingFrequency;
		}

		void SetSendingFrequency(float frequency) {
			this->sendingFrequency = frequency;
		}

		float GetConnectingFrequency() {
			return connectingFrequency;
		}

		void SetConnectingFrequency(float frequency) {
			this->connectingFrequency = frequency;
		}

		void PushMessageForSending(spt<NetOutputMessage> msg);
		
		void Close();

		void ConnectToPeer(string ip);

		void OnMessage(Msg& msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

	protected:

		void UpdateListening(const uint64 absolute);

		void UpdateDiscovering(const uint64 absolute);

		void UpdateConnecting(const uint64 absolute);

		void UpdateCommunicating(const uint64 absolute);

		void SendUpdateMessage(uint64 time);
	};

}// namespace