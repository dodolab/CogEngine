#include "ofxCogMain.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"
#include "ofxTextLabel.h"
#include "Network.h"
#include "Shape.h"
#include "NetworkBinder.h"
#include "NetworkBindReceiver.h"
#include "NetworkBindSender.h"
#include "NetMessage.h"

class NetworkBehavior : public Behavior {
private:
	bool receiver;
	NetworkBindReceiver* netReceiver;
	NetworkBindSender* netSender;
	NetworkBinder* binder;

public:
	NetworkBehavior(bool receiver) : receiver(receiver) {

	}

	void Init() {
		if (receiver) {
			netReceiver = new NetworkBindReceiver();
			REGISTER_COMPONENT(netReceiver);
			netReceiver->Init(1000, 1001, 11987);

			binder = GETCOMPONENT(NetworkBinder);
		}
		else {
			netSender = new NetworkBindSender();
			REGISTER_COMPONENT(netSender);
			netSender->Init(1000, 1001, "127.0.0.1", 11987);
		}
	}

	float param = 0;
	int frame = 0;
	virtual void Update(const uint64 delta, const uint64 absolute) {

		if (receiver) {
			param = this->binder->parameter1;
			cout << this->param << endl;
			owner->GetTransform().rotation = binder->parameter1;
			owner->GetTransform().localPos.x = binder->parameter2;
			owner->GetTransform().localPos.y = binder->parameter3;
		}
		else {
			if (frame++ % 35 == 0) {
				auto msg = spt<NetMessage>(new NetMessage(NetMsgType::DELTA_UPDATE, StringHash("FOFKA")));
				msg->SetMsgTime(absolute);


				float sendParam = owner->GetTransform().rotation;
				cout << "SENDING " << sendParam << endl;

				msg->SetFloatParameter1(owner->GetTransform().rotation);
				msg->SetFloatParameter2(owner->GetTransform().localPos.x);
				msg->SetFloatParameter3(owner->GetTransform().localPos.y);
				netSender->SendNetworkMessage(msg);
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


