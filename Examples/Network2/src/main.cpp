#include "ofxCogMain.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"
#include "ofxTextLabel.h"
#include "NetworkManager.h"
#include "Mesh.h"
#include "NetworkCommunicator.h"
#include "NetMessage.h"
#include "Interpolator.h"
#include "AttribAnimator.h"
#include "UpdateMessage.h"

enum class NetworkType{NONE, CLIENT,SERVER};


/**
* Network message payload
*/
class ValueMsg : public NetData {
public:

	int value;

	ValueMsg()  {

	}

	ValueMsg(int value) : value(value) {

	}

	void LoadFromStream(NetReader* reader) {
		this->value = reader->ReadDWord();
	}

	void SaveToStream(NetWriter* writer) {
		writer->WriteDWord(value);
	}

	int GetDataLength() {
		return sizeof(tDWORD);
	}

	spt<NetOutputMessage> CreateMessage() {
		auto outputMsg = spt<NetOutputMessage>(new NetOutputMessage(0));
		outputMsg->SetAction(StrId("VALUE_MESSAGE")); // this identifies the message payload
		outputMsg->SetData(this);
		return outputMsg;
	}
};

class NetworkBehavior : public Behavior {
private:
	NetworkCommunicator* communicator;
	NetworkType netType = NetworkType::NONE;
public:
	NetworkBehavior()  {

	}

	void OnInit() {
		SubscribeForMessages(ACT_NET_MESSAGE_RECEIVED, ACT_BUTTON_CLICKED);
		communicator = new NetworkCommunicator();
		REGISTER_COMPONENT(communicator);
	}

	void InitNetwork(NetworkType netType) {
		this->netType = netType;

		if (netType == NetworkType::SERVER) {
			communicator->InitListening(1234, 11987);
		}
		else {
			communicator->InitBroadcast(1234, 11986, 11987);
			communicator->SetAutoConnect(true);
		}
	}


	void OnMessage(Msg& msg) {
		if (msg.HasAction(ACT_BUTTON_CLICKED)) {
			if (msg.GetContextNode()->GetTag().compare("server_but") == 0) {
				// click on server button
				InitNetwork(NetworkType::SERVER);
				// disable buttons
				this->owner->GetScene()->FindNodeByTag("server_but")->SetState(StrId(STATES_DISABLED));
				this->owner->GetScene()->FindNodeByTag("client_but")->SetState(StrId(STATES_DISABLED));
			}
			else if (msg.GetContextNode()->GetTag().compare("client_but") == 0) {
				// click on client button
				InitNetwork(NetworkType::CLIENT);
				// disable buttons
				this->owner->GetScene()->FindNodeByTag("server_but")->SetState(StrId(STATES_DISABLED));
				this->owner->GetScene()->FindNodeByTag("client_but")->SetState(StrId(STATES_DISABLED));
			}
			else if (msg.GetContextNode()->GetTag().compare("incr_but") == 0) {
				// increase value
				auto text = owner->GetScene()->FindNodeByTag("value_to_send")->GetMesh<Text>();
				int textNum = ofToInt(text->GetText());
				textNum++;
				text->SetText(ofToString(textNum));
			}
			else if (msg.GetContextNode()->GetTag().compare("decr_but") == 0) {
				// decrease value
				auto text = owner->GetScene()->FindNodeByTag("value_to_send")->GetMesh<Text>();
				int textNum = ofToInt(text->GetText());
				textNum--;
				text->SetText(ofToString(textNum));
			}
			else if (msg.GetContextNode()->GetTag().compare("send_but") == 0) {
				// push message for sending
				auto text = owner->GetScene()->FindNodeByTag("value_to_send")->GetMesh<Text>();
				int textNum = ofToInt(text->GetText());
				auto valueMsg = new ValueMsg(textNum);
				auto netMsg = valueMsg->CreateMessage();
				communicator->PushMessageForSending(netMsg);
			}
		}

		if (msg.HasAction(ACT_NET_MESSAGE_RECEIVED)) {
			// set text according to the received value
			auto msgEvent = msg.GetData<NetworkMsgEvent>();
			auto netMsg = msgEvent->msg;
			if (netMsg->GetAction() == StrId("VALUE_MESSAGE")) {
				auto payload = netMsg->GetData<ValueMsg>();
				int value = payload->value;
				auto text = owner->GetScene()->FindNodeByTag("value_to_receive")->GetMesh<Text>();
				text->SetText(ofToString(value));
			}
		}
	}


	virtual void Update(const uint64 delta, const uint64 absolute) {
		if (netType == NetworkType::CLIENT || netType == NetworkType::SERVER) {
			// send just regular message, otherwise the other station will disconnect
			auto outputMsg = spt<NetOutputMessage>(new NetOutputMessage(0));
			outputMsg->SetAction(StrId(12345));
			communicator->PushMessageForSending(outputMsg);
		}
	}
};

class ExampleApp : public ofxCogApp {

	void RegisterComponents() {
		auto binder = new Interpolator();
		REGISTER_COMPONENT(binder);
		REGISTER_BEHAVIOR(NetworkBehavior);
	}

	void InitEngine() {
		ofxCogEngine::GetInstance().Init("config.xml");
		ofxCogEngine::GetInstance().LoadStageFromXml(spt<ofxXml>(new ofxXml("config.xml")));
	}
};


int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}


