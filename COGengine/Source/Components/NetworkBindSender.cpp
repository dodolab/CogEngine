
#include "CogEngine.h"
#include "ofxCogCommon.h"
#include "NetworkBindSender.h"
#include "Network.h"
#include "NetMessage.h"

namespace Cog {

	void NetworkBindSender::Init(int param1, int param2, string ip, int port) {
		this->port = port;
		this->param1 = param1;
		this->param2 = param2;
		this->ip = ip;

		network = GETCOMPONENT(Network);
		network->SetupUDPSender(ip, port, true);
	}

	void NetworkBindSender::OnMessage(Msg& msg) {

	}

	void NetworkBindSender::SendNetworkMessage(spt<NetMessage> msg) {
		auto writer = new NetWriter(msg->GetMessageLength());
		msg->SaveToStream(writer);
		unsigned int size = 0;
		auto data = writer->CopyData(size);

		// send message
		network->SendUDPMessage(param1, param2, data, size);
		delete writer;
	}

	void NetworkBindSender::Update(const uint64 delta, const uint64 absolute) {

	}

} // namespace