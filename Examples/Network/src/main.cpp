#include "ofxCogMain.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"
#include "ofxTextLabel.h"
#include "Network.h"
#include "Shape.h"
#include "DeltaUpdate.h"
#include "NetworkCommunicator.h"
#include "NetMessage.h"
#include "DeltaMessage.h"

class NetworkBehavior : public Behavior {
private:
	bool server;
	NetworkCommunicator* communicator;
	DeltaUpdate* deltaUpdate;

public:
	NetworkBehavior(bool server) : server(server) {

	}

	void Init() {
		communicator = new NetworkCommunicator();
		REGISTER_COMPONENT(communicator);
		if (!server) {
			deltaUpdate = GETCOMPONENT(DeltaUpdate);
		}
		
		communicator->Init(1234, 11987, server);

		if (!server) {
			for (auto& beh : owner->GetBehaviors()) {
				if (beh->GetClassNameW().compare("AttribAnimator") == 0) {
					owner->RemoveBehavior(beh, true);
					break;
				}
			}
		}
	}


	int frame = 0;

	virtual void Update(const uint64 delta, const uint64 absolute) {
		if (!server) {
			auto delta = this->deltaUpdate->actual;
			
			owner->GetTransform().rotation = delta->GetVal(StringHash("ROTATION"));
			owner->GetTransform().localPos.x = delta->GetVal(StringHash("POS_X"));
			owner->GetTransform().localPos.y = delta->GetVal(StringHash("POS_Y"));
		}
		else {
			if (frame++ % 3 == 0) {
				
				spt<DeltaInfo> delta = spt<DeltaInfo>(new DeltaInfo());
				delta->deltas[StringHash("ROTATION")] = owner->GetTransform().rotation;
				delta->deltas[StringHash("POS_X")] = owner->GetTransform().localPos.x;
				delta->deltas[StringHash("POS_Y")] = owner->GetTransform().localPos.y;

				DeltaMessage* msg = new DeltaMessage(delta);
				spt<NetOutputMessage> netMsg = spt<NetOutputMessage>(new NetOutputMessage(1));
				netMsg->SetData(msg);
				netMsg->SetAction(StringHash(NET_MSG_DELTA_UPDATE));
				communicator->SendNetworkMessage(netMsg);
			}
		}
	}
};




class ExampleApp : public CogApp {

	bool isServer = false;

	void RegisterComponents() {
		if (!isServer) {
			auto binder = new DeltaUpdate();
			REGISTER_COMPONENT(binder);
		}
	}

	void InitEngine() {
		CogEngine::GetInstance().Init("config.xml");
		CogEngine::GetInstance().LoadStageFromXml(spt<ofxXml>(new ofxXml("config.xml")));

		GETCOMPONENT(Stage)->GetActualScene()->FindNodeByTag("anim")->AddBehavior(new NetworkBehavior(isServer));

	}

	void InitStage(Stage* stage) {
	}
};


int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}


