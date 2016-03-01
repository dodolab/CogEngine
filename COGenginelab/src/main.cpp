#include "ofxCogMain.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"
#include "MultiAnim.h"
#include "Movement.h"
#include "Move.h"
#include "Path.h"
#include "SteeringBehavior.h"
#include "ofxTextLabel.h"

class PointerBehavior : public Behavior {
protected:

public:

	void Init() {
		RegisterListening(owner->GetScene(), ACT_OBJECT_HIT_STARTED);
	}

	void OnMessage(Msg& msg) {

		if (msg.GetAction() == ACT_OBJECT_HIT_STARTED) {

			InputEvent* evt = static_cast<InputEvent*>(msg.GetData());
			
			auto pointer = this->owner->GetScene()->FindNodeByTag("pointer");

			ofVec2f& dest = pointer->GetAttr<ofVec2f>(ATTR_STEERING_BEH_SEEK_DEST);
			dest.x = evt->input->position.x;
			dest.y = evt->input->position.y;
		}
	}

	void Update(const uint64 delta, const uint64 absolute) {
	
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

		for (int i = 0; i < 20; i++) {
			Node* pointer = new Node("pointer");
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
			auto movement = Movement();
			pointer->AddAttr(ATTR_MOVEMENT, movement);
			pointer->AddBehavior(new Move(true));
			pointer->AddBehavior(new WanderBehavior(300*(ofRandomf()+1), 50* (ofRandomf() + 1), 1000000));
		}

		
		Node* wanderer = new Node("pointer2");
		bld.SetImageNode(wanderer, "pawn2.png");

		TransformEnt node2trans = TransformEnt();
		node2trans.pos = ofVec2f(0.5f,0.5f);
		node2trans.anchor = ofVec2f(0.5f, 0.5f);
		node2trans.pType = CalcType::PER;
		node2trans.sType = CalcType::GRID;
		node2trans.size = ofVec2f(5, 5);
		node2trans.zIndex = 10;

		math = TransformMath();
		math.SetTransform(wanderer, main->GetSceneNode(), node2trans,100,50);

		main->GetSceneNode()->AddChild(wanderer);
		wanderer->GetTransform().SetRotationToPosition(ofVec2f(500,500));

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
		wanderer->AddAttr(ATTR_STEERING_BEH_SEEK_DEST, ofVec2f(200,200));


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



#ifdef WIN32

int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}

#else
#include <jni.h>

int main() {

	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}


//========================================================================
extern "C" {
	void Java_cc_openframeworks_OFAndroid_init(JNIEnv*  env, jobject  thiz) {
		main();
	}
}
#endif



