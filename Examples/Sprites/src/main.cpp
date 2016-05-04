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
#include "Move.h"
#include "Movement.h"
#include "SteeringBehavior.h"
#include "SpriteSheet.h"


class SpriteBehavior : public Behavior {
private:
	// view
	Node* viewNode = nullptr;
	// models
	vector<Node*> cars;
	// sprite storage
	vector<Sprite> spriteStorage;
	// sprite set
	spt<SpriteSet> spriteSet;
	Node* infoNode;
	int carsSize;
public:
	SpriteBehavior() {

	}

	void OnInit() {
		SubscribeForMessages(ACT_BUTTON_CLICKED);
		
		carsSize = GETCOMPONENT(ResourceCache)->GetProjectSettings().GetSettingValFloat("scene_settings", "cars_size");

		infoNode = owner->GetScene()->FindNodeByTag("info");
		viewNode = owner->GetScene()->FindNodeByTag("cars");
		// set sprite mesh
		spt<MultiSpriteMesh> mesh = spt<MultiSpriteMesh>(new MultiSpriteMesh("cars"));
		viewNode->SetMesh(mesh);

		// fill sprite storage
		auto spriteSheet = GETCOMPONENT(ResourceCache)->GetSpriteSheet("cars");
		spriteSet = spriteSheet->GetDefaultSpriteSet();

		for (int i = 0; i < 8; i++) {
			Sprite spr = Sprite(spriteSet, i);
			this->spriteStorage.push_back(spr);
		}

		AddCars(10);
	}

	void OnMessage(Msg& msg) {
		if (msg.HasAction(ACT_BUTTON_CLICKED)) {
			if (msg.GetContextNode()->GetTag().compare("increase_but") == 0) {
				AddCars(30);
			}
			else {
				DeleteCars(30);
			}
		}
	}

	virtual void Update(const uint64 delta, const uint64 absolute) {

		// when user presses 't', report will be written into log
		auto pressedKeys = CogGetPressedKeys();
		if (!pressedKeys.empty() && !pressedKeys[0]->IsHandled() && pressedKeys[0]->key == 't') {
			pressedKeys[0]->handlerNodeId = this->id;
			TimeMeasure::GetInstance().Report(true);

		}
		// copy transformations from model to view 
		auto& sprites = viewNode->GetMesh<MultiSpriteMesh>()->GetSprites();

		for (int i = 0; i < cars.size(); i++) {
			auto& model = cars[i];
			auto& view = sprites[i];
			view->transform = model->GetTransform();

			// each car has two frames -> with lights turned on and off,
			// switch the sprite cca every 1s
			if (ofRandom(0,1) < 0.005f) {
				if (view->sprite.GetFrame() % 2 == 0) {
					view->sprite = spriteStorage[view->sprite.GetFrame() + 1];
				}
				else {
					view->sprite = spriteStorage[view->sprite.GetFrame() - 1];
				}
			}
		}

		WriteInfo(absolute);
	}

	void AddCars(int num) {
		TransformMath math;
		auto spriteSheet = GETCOMPONENT(ResourceCache)->GetSpriteSheet("cars");
		auto spriteSet = spriteSheet->GetDefaultSpriteSet();


		for (int i = 0; i < num; i++) {
			Node* car = new Node("car");
			owner->AddChild(car);
			this->cars.push_back(car);

			// rectangles are not rendered (instead of planes) and can be used for size calculations
			auto rectangle = spt<Cog::Rectangle>(new Cog::Rectangle(spriteSet->GetSpriteWidth(), spriteSet->GetSpriteHeight()));
			car->SetMesh(rectangle);

			// top left corner of the screen
			TransformEnt transEntity = TransformEnt();
			transEntity.pos = ofVec2f(0);
			transEntity.anchor = ofVec2f(0.5f);
			transEntity.pType = CalcType::ABS_PER;
			transEntity.zIndex = 100;
			transEntity.sType = CalcType::ABS_PER;
			transEntity.size = ofVec2f(0.01f*carsSize,0); // size of screen %

			Trans trans;
			math.CalcTransform(trans, car, owner, transEntity);

			// add sprite into view with random car color
			int randomCar = ofRandom(0, 4) * 2;
			auto sprite = spriteStorage[randomCar];
			// sprite instance contains sprite and transformation
			auto spriteInstance = spt<SpriteInst>(new SpriteInst(sprite, trans));
			this->viewNode->GetMesh<MultiSpriteMesh>()->AddSprite(spriteInstance);
			
			// set transformation of the model node
			car->SetTransform(trans);

			// insert steering behavior for random movement
			auto movement = Movement();
			car->AddAttr(ATTR_MOVEMENT, movement);
			car->AddBehavior(new Move(true));
			car->AddBehavior(new WanderBehavior(300 * (ofRandomf() + 1), 50 * (ofRandomf() + 1), 1000000));
		}
	}

	void DeleteCars(int num) {
		for (int i = 0; i < num; i++) {
			if (!cars.empty()) {
				auto first = (*cars.begin());
				cars.erase(cars.begin());
				owner->RemoveChild(first,true);
				auto sprites = this->viewNode->GetMesh<MultiSpriteMesh>();
				sprites->RemoveSprite(sprites->GetSprites()[0]);
			}
		}
	}

	uint64 lastAbsolute;
	uint64 lastFrequencyRefresh;
	int lastFrequency;
	int refreshCalls = 1;

	void WriteInfo(uint64 absolute) {
		int carsNum = cars.size();
		int frequency;

		if ((absolute - lastFrequencyRefresh) > 1000) {
			frequency = (int)(1000.0f / (absolute - lastFrequencyRefresh) * refreshCalls);
			lastFrequencyRefresh = absolute;
			refreshCalls = 0;
		}
		else {
			refreshCalls++;
			frequency = lastFrequency;
		}

		lastFrequency = frequency;
		lastAbsolute = absolute;

		string info = string_format("Total cars: %d; FPS: %d",carsNum, frequency);
		this->infoNode->GetMesh<Text>()->SetText(info);
	}
};


class ExampleApp : public ofxCogApp {

	void RegisterComponents() {
		REGISTER_BEHAVIOR(SpriteBehavior);
	}

	void InitEngine() {
		ofxCogEngine::GetInstance().Init("config.xml");
		ofxCogEngine::GetInstance().LoadStageFromXml(spt<ofxXml>(new ofxXml("config.xml")));
	}

	void InitStage(Stage* stage) {
	}
};


int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}