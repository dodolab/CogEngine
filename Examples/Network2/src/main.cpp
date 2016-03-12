#include "ofxCogMain.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"
#include "ofxTextLabel.h"
#include "Network.h"
#include "Shape.h"
#include "NetworkBinder.h"
#include "NetworkCommunicator.h"
#include "NetMessage.h"

class NetworkBehavior : public Behavior {
private:
	bool server;
	NetworkCommunicator* communicator;
	
public:
	NetworkBehavior(bool server) : server(server) {

	}

	ofVec2f actualPosition = ofVec2f(0, 0);

	void Init() {
		communicator = new NetworkCommunicator();
		REGISTER_COMPONENT(communicator);
		communicator->Init(1234, 11987, server);

		RegisterGlobalListening(ACT_NET_MESSAGE_RECEIVED, ACT_NET_CALLBACK_RECEIVED);
	}

	void OnMessage(Msg& msg) {
		if (msg.GetAction() == ACT_NET_MESSAGE_RECEIVED || msg.GetAction() == ACT_NET_CALLBACK_RECEIVED) {
			NetworkMsgEvent* msgEvent = msg.GetDataS<NetworkMsgEvent>();
			spt<NetMessage> netMsg = msgEvent->msg;
			
			if (netMsg->GetAction() == StringHash("COM")) {
				NetReader* reader = new NetReader(netMsg->GetData(), netMsg->GetDataLength());
				string strMessage = reader->ReadString();
				cout << "Received " << strMessage << endl;
				ProcessMessage(strMessage);
			}

		}
	}



	void ProcessMessage(string msg) {
		if (server) {
			cout << "RECEIVED: " << msg << endl;
			int random = ofRandom(0, 5);
			switch (random) {
			case 0:
				SendAnswer("Where are you?");
				break;
			case 1:
				SendAnswer("Go up");
				break;
			case 2:
				SendAnswer("Go down");
				break;
			case 3:
				SendAnswer("Go left");
				break;
			case 4:
				SendAnswer("Go right");
				break;
			}
		}
		else {
			if (msg.compare("Where are you?") == 0) {
				string answer = string_format("I am at [%d,%d]", (int)actualPosition.x, (int)actualPosition.y);
				SendAnswer(answer);
			}
			else if (msg.compare("Go up") == 0) {
				actualPosition.y--;
				SendAnswer(string_format("Going up to [%d,%d]", (int)actualPosition.x, (int)actualPosition.y));
			}
			else if (msg.compare("Go down") == 0) {
				actualPosition.y++;
				SendAnswer(string_format("Going down to [%d,%d]", (int)actualPosition.x, (int)actualPosition.y));
			}
			else if (msg.compare("Go left") == 0) {
				actualPosition.x--;
				SendAnswer(string_format("Going left to [%d,%d]", (int)actualPosition.x, (int)actualPosition.y));
			}
			else if (msg.compare("Go right") == 0) {
				actualPosition.x++;
				SendAnswer(string_format("Going right to [%d,%d]", (int)actualPosition.x, (int)actualPosition.y));
			}
		}
	}

	void SendAnswer(string msg) {
		cout << "ANSWER: " << msg << endl;
		spt<NetMessage> netMsg = spt<NetMessage>(new NetMessage(server ? NetMsgType::UPDATE : NetMsgType::CLIENT_CALLBACK));
		netMsg->SetAction(StringHash("COM"));
		NetWriter* wr = new NetWriter(1000);
		wr->WriteString(msg);
		netMsg->SaveToStream(wr);
		unsigned size = 0;
		auto data = wr->CopyData(size);
		netMsg->SetData(data, size);

		communicator->SendNetworkMessage(netMsg);
	}

	bool comInit = false;

	virtual void Update(const uint64 delta, const uint64 absolute) {
		if (communicator->ClientConnected() && (!comInit || (absolute - communicator->GetClientLastCallBack() > 500))) {
			SendAnswer("Where are you?");
			comInit = true;
		}
	}
};


class ExampleApp : public CogApp {

	void RegisterComponents() {
		auto binder = new NetworkBinder();
		REGISTER_COMPONENT(binder);
	}

	void InitEngine() {
		CogEngine::GetInstance().Init("config.xml");
		CogEngine::GetInstance().LoadStageFromXml(spt<ofxXml>(new ofxXml("config.xml")));

		GETCOMPONENT(Stage)->GetActualScene()->FindNodeByTag("bgr")->AddBehavior(new NetworkBehavior(true));

	}

	void InitStage(Stage* stage) {
	}
};


int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}


