#include "ofxCogMain.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"
#include "ofxTextLabel.h"
#include "Network.h"
#include "Shape.h"

class UDPMessage {
public:

	UDPMessage() {

	}

	UDPMessage(int number)
		: number(number) {

	}

	int number;

	void LoadFromStream(NetReader* reader) {
		number = reader->ReadDWord();
	}

	void SaveToStream(NetWriter* writer) {
		writer->WriteDWord(number);
	}
};

class NetworkBehavior : public Behavior {

	Network* network;
	bool receiver;

public:
	NetworkBehavior(bool receiver) {
		this->receiver = receiver;
	}

	void Init() {
		network = GETCOMPONENT(Network);
		if (receiver) {
			network->SetupUDPReceiver(11999, true);
		}
		else {
			network->SetupUDPSender("10.16.0.122", 11999, true);
		}
	}

	int counter = 0;
	virtual void Update(const uint64 delta, const uint64 absolute) {
		if (receiver) {
			auto reader = network->ReceiveUDPMessage(1000, 1001, 1);

			if (reader != nullptr) {
				UDPMessage msg = UDPMessage();
				msg.LoadFromStream(reader);
				auto label = owner->GetShape<spt<Label>>();
				label->AppendLine(ofToString(msg.number));
			}
		}
		else {
			if ((counter++) % 10 == 0) {
				UDPMessage msg(ofRandom(0, 100));
				auto writer = new NetWriter(200);
				msg.SaveToStream(writer);
				unsigned int size = 0;
				auto data = writer->CopyData(size);

				// send message
				network->SendUDPMessage(1000, 1001, data, size);
			}
		}
	}
};


class ExampleApp : public CogApp {

	void InitComponents() {

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
		text->AddBehavior(new NetworkBehavior(false));
		main->GetSceneNode()->AddChild(text);

		// add scene into stage
		auto stage = GETCOMPONENT(Stage);
		stage->AddScene(main, true);

		// init logging
		auto logger = GETCOMPONENT(Logger);
		logger->SetLogLevel(LogLevel::LDEBUG);

	}

	void InitStage(Stage* stage) {
	}
};


int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}


