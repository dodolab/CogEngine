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
	NetworkBehavior(bool receiver): receiver(receiver) {

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

	int param = 0;
	int frame = 0;
	virtual void Update(const uint64 delta, const uint64 absolute) {
		if (frame++ % 10 == 0) {
			if (receiver) {
				param = this->binder->parameter;
				owner->GetShape<spt<Label>>()->SetText(ofToString(param));
			}
			else {
				auto msg = spt<NetMessage>(new NetMessage(NetMsgType::DELTA_UPDATE, StringHash("FOFKA")));
				msg->SetMsgTime(absolute);
				cout << "SENDING " << param << endl;;
				msg->SetParameter(param);
				param += 10;

				netSender->SendNetworkMessage(msg);
				delete msg;
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
		// this example is an alternative for config1.xml
		CogEngine::GetInstance().Init();

		auto resCache = GETCOMPONENT(ResourceCache);

		// <spritesheets>
		spt<SpriteSheet> spriteSheet = spt<SpriteSheet>(new SpriteSheet("bgr", CogGet2DImage("bgr.jpg"), 1, 800, 450));
		resCache->StoreSpriteSheet(spriteSheet);

		// <scenes>
		Scene* main = new Scene("main", false);

		//  <scene_layers>
		LayerEnt layer1 = LayerEnt("bgr", "bgr", 100, 30);
		main->AddLayer(layer1);

		// <node>
		TransformMath math = TransformMath();
		NodeBuilder bld = NodeBuilder();
		Node* node1 = new Node("bgr");
		bld.SetSpriteNode(main, node1, "bgr", 0, 0);
		math.SetSizeToScreen(node1, main->GetSceneNode());
		main->GetSceneNode()->AddChild(node1);
		
		auto font = CogGetFont("cousine.ttf", 25);
		Node* text = new Node("text");
		auto label = spt<Label>(new Label(font, "test", CogGetScreenWidth()));
		label->SetColor(ofColor(255, 255, 0));
		text->SetShape(label);
		text->AddBehavior(new NetworkBehavior(true));
		main->GetSceneNode()->AddChild(text);

		// add scene into stage
		auto stage = GETCOMPONENT(Stage);
		stage->AddScene(main, true);

		// init logging
		auto logger = GETCOMPONENT(Logger);
		//logger->SetLogLevel(LogLevel::LDEBUG);

	}

	void InitStage(Stage* stage) {
	}
};


int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}


