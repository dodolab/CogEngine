#include "ofxCogMain.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"
#include "ofxTextLabel.h"
#include "NetworkManager.h"
#include "Mesh.h"
#include "NetworkClient.h"
#include "NetworkHost.h"
#include "NetMessage.h"
#include "Interpolator.h"
#include "AttribAnimator.h"
#include "UpdateMessage.h"

enum class NetworkType{NONE, CLIENT,SERVER};

class NetworkBehavior : public Behavior {
private:
	NetworkClient* client;
	NetworkHost* host;
	Interpolator* deltaUpdate;
	NetworkType netType = NetworkType::NONE;
public:
	NetworkBehavior()  {

	}

	void OnInit() {
		SubscribeForMessages(ACT_NET_MESSAGE_RECEIVED, ACT_BUTTON_CLICKED);
		client = new NetworkClient();
		REGISTER_COMPONENT(client);
		host = new NetworkHost();
		REGISTER_COMPONENT(host);
	}

	void InitNetwork(NetworkType netType) {
		this->netType = netType;

		if (netType == NetworkType::CLIENT) {
			deltaUpdate = GETCOMPONENT(Interpolator);
		}

		if (netType == NetworkType::SERVER) {
			host->InitHost(1234, 11987);
		}
		else {
			client->InitClient(1234, 11986, 11987);
			client->SetAutoConnect(true);
		}

		if (netType == NetworkType::CLIENT) {
			// remove animator, because animation will be synchronized
			owner->RemoveBehavior(owner->GetBehavior<AttribAnimator>(), true);
		}
	}

	void OnMessage(Msg& msg) {
		if (msg.HasAction(ACT_BUTTON_CLICKED)) {
			if (msg.GetContextNode()->GetTag().compare("server_but") == 0) {
				// click on server button
				InitNetwork(NetworkType::SERVER);
			}
			else {
				// click on client button
				InitNetwork(NetworkType::CLIENT);
			}

			// disable buttons
			this->owner->GetScene()->FindNodeByTag("server_but")->SetState(StrId(STATE_DISABLED));
			this->owner->GetScene()->FindNodeByTag("client_but")->SetState(StrId(STATE_DISABLED));
		}

		if (msg.HasAction(ACT_NET_MESSAGE_RECEIVED) && netType == NetworkType::CLIENT) {
			// push received message to Interpolator
			auto netMsg = msg.GetDataPtr<NetInputMessage>();
			if (netMsg->GetAction() == NET_MSG_UPDATE) {

				spt<UpdateMessage> updateMsg = netMsg->GetData<UpdateMessage>();
				spt<UpdateInfo> deltaInfo = spt<UpdateInfo>(new UpdateInfo(netMsg->GetMsgTime(), updateMsg->GetContinuousValues(), updateMsg->GetDiscreteValues()));
				deltaUpdate->AcceptUpdateMessage(deltaInfo);
			}
		}
	}

	int frame = 0;
	int period = 3;

	virtual void Update(const uint64 delta, const uint64 absolute) {
		if (netType == NetworkType::CLIENT) {
			// set position and rotation according to the message
			auto delta = this->deltaUpdate->GetActualUpdate();
			owner->GetTransform().rotation = delta->GetVal(StrId("ROTATION"));
			owner->GetTransform().localPos.x = delta->GetVal(StrId("POS_X"));
			owner->GetTransform().localPos.y = delta->GetVal(StrId("POS_Y"));
		}
		else if(netType == NetworkType::SERVER){
			if (frame++ % period == 0) {
				// send values to the client
				auto updateInfo = spt<UpdateInfo>(new UpdateInfo());
				updateInfo->GetContinuousValues()[StrId("ROTATION")] = owner->GetTransform().rotation;
				updateInfo->GetContinuousValues()[StrId("POS_X")] = owner->GetTransform().localPos.x;
				updateInfo->GetContinuousValues()[StrId("POS_Y")] = owner->GetTransform().localPos.y;

				UpdateMessage* msg = new UpdateMessage(updateInfo);
				spt<NetOutputMessage> netMsg = spt<NetOutputMessage>(new NetOutputMessage(1, 1));
				netMsg->SetData(msg);
				netMsg->SetAction(StrId(NET_MSG_UPDATE));

				// ===================== HANDLE KEYS ==========================
				// if R is pressed, sending is stopped
				auto pressedKeys = CogGetPressedKeys();
				if (pressedKeys.size() == 0 || pressedKeys[0]->key != 'r') {
					netMsg->SetMsgTime(absolute);
					host->PushMessageForSending(netMsg);
				}

				if (pressedKeys.size() != 0) {
					if (pressedKeys[0]->key == 'm' && period < 20) period++;
					else if (pressedKeys[0]->key == 'n' && period > 2) period--;
					cout << "period changed to " << period << endl;
				}
			}
		}
	}
};

class ExampleApp : public ofxCogApp {

	void RegisterComponents() {
		auto binder = new Interpolator();
		REGISTER_COMPONENT(binder);
		// this behavior is defined in config.xml
		REGISTER_BEHAVIOR(NetworkBehavior);
	}

	void InitEngine() {
		ofxCogEngine::GetInstance().Init();
		ofxCogEngine::GetInstance().LoadStage();
	}
};

int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}


