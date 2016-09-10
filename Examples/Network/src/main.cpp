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
	NetworkBinder* binder;

public:
	NetworkBehavior(bool server) : server(server) {

	}

	void Init() {
		communicator = new NetworkCommunicator();
		REGISTER_COMPONENT(communicator);
		binder = GETCOMPONENT(NetworkBinder);
		communicator->Init(1234, 11987, server);
	}

	float param = 0;
	int frame = 0;

	virtual void Update(const uint64 delta, const uint64 absolute) {
		if (!server) {
			param = this->binder->parameter1;
			owner->GetTransform().rotation = binder->parameter1;
			owner->GetTransform().localPos.x = binder->parameter2;
			owner->GetTransform().localPos.y = binder->parameter3;
		}
		else {
			if (frame++ % 3 == 0) {
				auto msg = spt<NetMessage>(new NetMessage(NetMsgType::UPDATE, StringHash("ffs")));
				msg->SetMsgTime(absolute);


				float sendParam = owner->GetTransform().rotation;
				
				msg->SetFloatParameter1(owner->GetTransform().rotation);
				msg->SetFloatParameter2(owner->GetTransform().localPos.x);
				msg->SetFloatParameter3(owner->GetTransform().localPos.y);
				communicator->SendNetworkMessage(msg);
			}
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

		GETCOMPONENT(Stage)->GetActualScene()->FindNodeByTag("anim")->AddBehavior(new NetworkBehavior(true));

	}

	void InitStage(Stage* stage) {
	}
};


int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}


