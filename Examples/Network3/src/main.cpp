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
#include "Move.h"
#include "Path.h"
#include "SteeringBehavior.h"

#define IS_SERVER false
#define POINTERS 50

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
	DeltaMessage* previous = nullptr;

	virtual void Update(const uint64 delta, const uint64 absolute) {
		if (!server) {
			auto delta = this->deltaUpdate->actual;
			
			/*
			auto pointer2 = owner->GetScene()->FindNodeByTag("pointer2");
			pointer2->GetTransform().rotation = delta->GetVal(StringHash(string_format("%s_rot","wanderer")));
			pointer2->GetTransform().localPos.x = delta->GetVal(StringHash(string_format("%s_posx", "wanderer")));
			pointer2->GetTransform().localPos.y = delta->GetVal(StringHash(string_format("%s_posy", "wanderer")));
			*/

			for (int i = 0; i < POINTERS; i++) {
				auto pointer = owner->GetScene()->FindNodeByTag(string_format("pointer_%d", i));
				pointer->GetTransform().rotation = delta->GetVal(StringHash(string_format("pointer_%d_rot", i)));
				pointer->GetTransform().localPos.x = delta->GetVal(StringHash(string_format("pointer_%d_posx", i)));
				pointer->GetTransform().localPos.y = delta->GetVal(StringHash(string_format("pointer_%d_posy", i)));
			}
		}
		else {
			if (frame++ % 3 == 0) {
				
				spt<DeltaInfo> delta = spt<DeltaInfo>(new DeltaInfo());

			/*	auto pointer2 = owner->GetScene()->FindNodeByTag("pointer2");
				delta->deltas[StringHash(string_format("%s_rot", "wanderer"))] = pointer2->GetTransform().rotation;
				delta->deltas[StringHash(string_format("%s_posx", "wanderer"))] = pointer2->GetTransform().localPos.x;
				delta->deltas[StringHash(string_format("%s_posy", "wanderer"))] = pointer2->GetTransform().localPos.y;
				*/

				for (int i = 0; i < POINTERS; i++) {
					auto pointer = owner->GetScene()->FindNodeByTag(string_format("pointer_%d", i));

					delta->deltas[StringHash(string_format("pointer_%d_rot", i))] = pointer->GetTransform().rotation;

					if (previous != nullptr) {
						string posXStr = string_format("pointer_%d_posx", i);
						string posYStr = string_format("pointer_%d_posy", i);
						
						if (previous->deltas.find(posXStr) != previous->deltas.end() &&previous->deltas.find(posYStr) != previous->deltas.end()) {
							float previousPosX = previous->deltas.find(posXStr)->second;
							float previousPosY = previous->deltas.find(posYStr)->second;
							float actualPosX = pointer->GetTransform().localPos.x;
							float actualPosY = pointer->GetTransform().localPos.y;

							if (abs(actualPosY - previousPosY) > 100 || abs(actualPosX-previousPosX) >100) {
								// use teleport
								delta->teleports[posXStr] = actualPosX;
								delta->teleports[posYStr] = actualPosY;
								continue;
							}
						}
					}

					delta->deltas[StringHash(string_format("pointer_%d_posx", i))] = pointer->GetTransform().localPos.x;
					delta->deltas[StringHash(string_format("pointer_%d_posy", i))] = pointer->GetTransform().localPos.y;
				}

				DeltaMessage* msg = new DeltaMessage(delta);
				spt<NetOutputMessage> netMsg = spt<NetOutputMessage>(new NetOutputMessage(1));
				netMsg->SetData(msg);
				netMsg->SetAction(StringHash(NET_MSG_DELTA_UPDATE));
				communicator->SendNetworkMessage(netMsg);
				delete previous;
				previous = msg;
			}
		}
	}
};




class ExampleApp : public CogApp {


	void RegisterComponents() {
		if (!IS_SERVER) {
			auto binder = new DeltaUpdate();
			REGISTER_COMPONENT(binder);
		}
	}

	void InitEngine() {

		// this example is an alternative for config1.xml
		CogEngine::GetInstance().Init();

		auto resCache = GETCOMPONENT(ResourceCache);

		// <spritesheets>
		spt<SpriteSheet> spriteSheet = spt<SpriteSheet>(new SpriteSheet("bgr", CogGet2DImage("bgr2.jpg"), 1, 800, 450));
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
		node1->GetStates().SetState(StringHash(STATES_HITTABLE));
		node1->AddBehavior(new HitEvent());
		//node1->AddBehavior(new PointerBehavior());

		for (int i = 0; i < POINTERS; i++) {
			Node* pointer = new Node(string_format("pointer_%d", i));
			bld.SetImageNode(pointer, "pawn.png");

			TransformEnt node2trans = TransformEnt();
			node2trans.pos = ofVec2f(0.5f, 0.5f);
			node2trans.anchor = ofVec2f(0.5f, 0.5f);
			node2trans.pType = CalcType::PER;
			node2trans.sType = CalcType::GRID;
			node2trans.size = ofVec2f(3, 3);
			node2trans.zIndex = 10;


			math.SetTransform(pointer, main->GetSceneNode(), node2trans, 100, 50);
			main->GetSceneNode()->AddChild(pointer);
			pointer->GetTransform().SetRotationToPosition(ofVec2f(500, 500));

			if (IS_SERVER) {
				auto movement = Movement();
				pointer->AddAttr(ATTR_MOVEMENT, movement);
				pointer->AddBehavior(new Move(true));
				pointer->AddBehavior(new WanderBehavior(300 * (ofRandomf() + 1), 50 * (ofRandomf() + 1), 1000000));
			}
		}


	/*	Node* wanderer = new Node("pointer2");
		bld.SetImageNode(wanderer, "pawn2.png");

		TransformEnt node2trans = TransformEnt();
		node2trans.pos = ofVec2f(0.5f, 0.5f);
		node2trans.anchor = ofVec2f(0.5f, 0.5f);
		node2trans.pType = CalcType::PER;
		node2trans.sType = CalcType::GRID;
		node2trans.size = ofVec2f(5, 5);
		node2trans.zIndex = 10;

		math = TransformMath();
		math.SetTransform(wanderer, main->GetSceneNode(), node2trans, 100, 50);

		main->GetSceneNode()->AddChild(wanderer);
		wanderer->GetTransform().SetRotationToPosition(ofVec2f(500, 500));

		if (IS_SERVER) {
			auto movement = Movement();

			wanderer->AddAttr(ATTR_MOVEMENT, movement);
			wanderer->AddBehavior(new Move(true));

			Path* path = new Path(ofVec2f(0, 0), ofVec2f(10, 10));
			path->AddSegment(ofVec2f(50, 50));
			path->AddSegment(ofVec2f(100, 50));
			path->AddSegment(ofVec2f(200, 50));
			path->AddSegment(ofVec2f(200, 100));
			path->AddSegment(ofVec2f(200, 150));
			path->AddSegment(ofVec2f(100, 50));
			path->AddSegment(ofVec2f(50, 50));

			//pointer->AddBehavior(new ArriveBehavior(10,15));
			wanderer->AddBehavior(new FollowBehavior(path, 10, 10));
			wanderer->AddAttr(ATTR_STEERING_BEH_SEEK_DEST, ofVec2f(200, 200));
		}*/

		// add scene into stage
		auto stage = GETCOMPONENT(Stage);
		stage->AddScene(main, true);

		// init logging
		auto logger = GETCOMPONENT(Logger);
		logger->SetLogLevel(LogLevel::LDEBUG);

		
		GETCOMPONENT(Stage)->GetActualScene()->GetSceneNode()->AddBehavior(new NetworkBehavior(IS_SERVER));

	}

	void InitStage(Stage* stage) {
	}
};


int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}


