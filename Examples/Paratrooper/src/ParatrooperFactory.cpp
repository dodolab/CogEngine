#include "Component.h"
#include "TransformBuilder.h"
#include "ParatrooperModel.h"
#include "GameValues.h"
#include "GameManager.h"
#include "CollisionManager.h"
#include "CannonController.h"
#include "ScoreComponent.h"
#include "DeathChecker.h"
#include "SoundComponent.h"
#include "ProjectileComponent.h"
#include "CopterAnimator.h"
#include "GameManager.h"
#include "CopterComponent.h"
#include "CopterSpawner.h"
#include "jsonxx.h"
#include "Node.h"
#include "Stage.h"

using namespace jsonxx;
using namespace Cog;

ParatrooperModel* ParatrooperFactory::LoadGameModel() {
	ifstream fin; 
	fin.open(ofToDataPath(FILE_CONFIG).c_str());

	Object o;
	o.parse(fin);
	fin.close();

	auto model = new ParatrooperModel();
	model->maxLandedUnits = o.get<Number>("max_landed_units");
	model->minCannonAngle = o.get<Number>("min_cannon_angle");
	model->maxCannonAngle = o.get<Number>("max_cannon_angle");
	model->cannonFireRate = o.get<Number>("cannon_fire_rate");
	model->paratrooperSpawnFrequency = o.get<Number>("paratrooper_spawn_frequency");
	model->copterSpawnFrequency = o.get<Number>("copter_spawn_frequency");
	model->copterReward = o.get<Number>("copter_reward");
	model->paratrooperFallingReward = o.get<Number>("paratrooper_falling_reward");
	model->paratrooperShotReward = o.get<Number>("paratrooper_shot_reward");
	model->shootPenalty = o.get<Number>("shoot_penalty");
	model->projectileVelocity = o.get<Number>("projectile_velocity");
	model->gravity = o.get<Number>("gravity");
	model->parachuteDecceleration = o.get<Number>("parachute_decceleration");
	model->copterSpawnMinY = o.get<Number>("copter_spawn_min_y");
	model->copterSpawnMaxY = o.get<Number>("copter_spawn_max_y");
	model->copterMinVelocity = o.get<Number>("copter_min_velocity");
	model->copterMaxVelocity = o.get<Number>("copter_max_velocity");
	model->parachuteOpenAltitude = o.get<Number>("parachute_open_altitude");
	model->parachuteOpenVelocityThreshold = o.get<Number>("parachute_open_velocity_threshold");

	return model;
}

void ParatrooperFactory::InitializeGame(Node* rootObject, ParatrooperModel* model) {
	
	auto scene = rootObject->GetScene();
	scene->SetCustomWidth(100);

	CogSetGameSpeed(0.1f);

	TransformBuilder transBld;

	// create all visible objects
	auto tower = new Node(OBJECT_TOWER);
	auto turret = new Node(OBJECT_TURRET);
	auto cannon = new Node(OBJECT_CANNON);
	auto ground = new Node(OBJECT_GROUND);

	tower->SetMesh(spt<Image>(new Image(CogGet2DImage(FILE_TOWER))));
	turret->SetMesh(spt<Image>(new Image(CogGet2DImage(FILE_TURRET))));
	cannon->SetMesh(spt<Image>(new Image(CogGet2DImage(FILE_CANNON))));
	ground->SetMesh(spt<FRect>(new FRect(100, 0.3f, ofColor(0, 255, 255))));

	// add game model
	rootObject->AddAttr(MODEL, model);
	
	// create labels
	auto font = spt<ofTrueTypeFont>(new ofTrueTypeFont());
	font->load(FILE_FONT, 20);
	
	// score
	auto score = new Node("score");
	score->SetMesh(spt<Text>(new Text(font, "")));
	// game over label
	string text = "GAME OVER";
	auto gameOver = new Node(OBJECT_GAMEOVER);
	gameOver->SetMesh(spt<Text>(new Text(font, text)));

	gameOver->GetMesh()->SetIsVisible(false);
	// number of lives
	auto lives = new Node("lives");
	lives->SetMesh(spt<Text>(new Text(font, "")));
	
	// create scene graph
	rootObject->AddChild(tower);
	rootObject->AddChild(score);
	rootObject->AddChild(lives);
	rootObject->AddChild(gameOver);
	rootObject->AddChild(ground);
	tower->AddChild(turret);
	turret->AddChild(cannon);
	
	// add root components -> managers
	rootObject->AddBehavior(new GameManager());
	rootObject->AddBehavior(new DeathChecker());
	rootObject->AddBehavior(new SoundComponent());
	rootObject->AddBehavior(new CopterSpawner());
	rootObject->AddBehavior(new CollisionManager());

	// add other components
	score->AddBehavior(new ScoreComponent());
	lives->AddBehavior(new LivesComponent());
	cannon->AddBehavior(new CannonInputController());
	
	// update all transformations
	rootObject->SubmitChanges(true);
	rootObject->UpdateTransform(true);

	// use magic builder to set positions of all children
	transBld.RelativePosition(0.80f, 1.01f).Anchor(1, 1).ZIndex(2).BuildAndReset(score);														// score
	transBld.RelativePosition(0.10f, 1.01f).Anchor(1, 1).ZIndex(2).BuildAndReset(lives);														// lives
	transBld.RelativePosition(0.5f, 0.5f).Anchor(0.5f, 0.5f).ZIndex(2).BuildAndReset(gameOver);													// game over label
	transBld.RelativePosition(0.5f, 0.94f).Anchor(0.5f, 1).ZIndex(2).BuildAndReset(tower);														// tower
	transBld.RelativePosition(0.5f, 0).Anchor(0.5f, 1).ZIndex(2).BuildAndReset(turret);															// turret
	transBld.RelativePosition(0.5f, 0.35f).Anchor(0.5f, 1).ZIndex(1).RotationCenter(0.5f, 1).BuildAndReset(cannon);								// cannon
	transBld.RelativePosition(0, 0.94f).Anchor(0, 1).ZIndex(1).BuildAndReset(ground);															// ground
}

void ParatrooperFactory::CreateProjectile(Node* canon, ParatrooperModel* model) {
	auto projectileImg = CogGet2DImage(FILE_PROJECTILE);
	Node* projectile = new Node("projectile");
	projectile->SetMesh(spt<Image>(new Image(projectileImg)));
	projectile->SetState(FLAG_PROJECTILE);

	auto rootObject = canon->GetSceneRoot();
	auto canonTrans = canon->GetTransform();

	TransformBuilder transBld;

	rootObject->AddChild(projectile);
	rootObject->SubmitChanges(true);

	float rotation = ofDegToRad(canon->GetTransform().rotation);
	auto absWidth = canonTrans.absScale.x * canon->GetMesh()->GetWidth();
	auto absHeight = canonTrans.absScale.y * canon->GetMesh()->GetHeight();
	
	// we need the projectile to be at the same location as the cannon with current rotation
	transBld.AbsolutePosition(canonTrans.absPos.x + absWidth * 0.5f + absHeight * sin(rotation), 
		canonTrans.absPos.y + absHeight - absHeight * cos(rotation)).Build(projectile);

	float velocityX = model->projectileVelocity * cos(rotation - PI / 2);
	float velocityY = model->projectileVelocity * sin(rotation - PI / 2);

	auto dynamics = new Dynamics();
	dynamics->SetVelocity(ofVec2f(velocityX, velocityY));
	dynamics->SetAcceleration(ofVec2f(0, model->gravity)); // add gravity

	projectile->AddAttr(ATTR_MOVEMENT, dynamics);
	projectile->AddBehavior(new ProjectileComponent());
	
	COGLOGDEBUG("Factory", "Projectile created");
}

void ParatrooperFactory::CreateParatrooper(Node* owner, ParatrooperModel* model) {
	auto paratrooperMesh = spt<Image>(new Image(CogGet2DImage(FILE_PARATROOPER)));
	Node* paratrooper = new Node(OBJECT_PARATROOPER);
	paratrooper->SetMesh(paratrooperMesh);

	paratrooper->SetState(FLAG_COLLIDABLE);
	paratrooper->SetTransform(owner->GetTransform());

	auto dynamics = new Dynamics();
	dynamics->SetAcceleration(ofVec2f(0, model->gravity));
	paratrooper->AddAttr(ATTR_MOVEMENT, dynamics);

	auto rootObject = owner->GetSceneRoot();
	rootObject->AddChild(paratrooper);
	rootObject->SubmitChanges(true);
	paratrooper->AddAttr(PARA_STATE, ParaState::FALLING);
	paratrooper->AddBehavior(new ParatrooperComponent());
	COGLOGDEBUG("Factory", "Paratrooper created");

}

void ParatrooperFactory::CreateCopter(Node* owner, ParatrooperModel* model) {
	auto copterLeft = CogGet2DImage(FILE_COPTER_LEFT);
	auto copterImage = spt<Image>(new Image(copterLeft));
	Node* copter = new Node(OBJECT_COPTER);
	copter->SetMesh(copterImage);
	copter->SetState(FLAG_COLLIDABLE);

	auto rootObject = owner->GetSceneRoot();

	rootObject->AddChild(copter);
	rootObject->SubmitChanges(true);
	TransformBuilder transBld;

	// 50% probability that the copter will be spawned on the left side
	bool spawnLeft = ofRandom(0, 1) > 0.5f;

	float posY = ofRandom(model->copterSpawnMinY, model->copterSpawnMaxY);
	float posX = spawnLeft ? -0.2f : 1.2f;

	transBld.RelativePosition(posX, posY)
	.Anchor(0.5f, 0.5f).Build(copter);

	float velocity = (spawnLeft ? 1 : -1) * ofRandom(model->copterMinVelocity, model->copterMaxVelocity);
	auto dynamics = new Dynamics();
	dynamics->SetVelocity(ofVec2f(velocity, 0));
	copter->AddAttr(ATTR_MOVEMENT, dynamics);

	copter->AddBehavior(new CopterComponent());
	copter->AddBehavior(new CopterAnimator());

	COGLOGDEBUG("Factory", "Copter created");
}

void ParatrooperFactory::Update(const uint64 delta, const uint64 absolute) {
	if (resetGamePending) {
		resetGamePending = false;
		auto stage = GETCOMPONENT(Stage);
		auto scene = stage->GetActualScene();
		scene->Finish();
		scene->Init();
		auto model = this->LoadGameModel();
		InitializeGame(stage->GetActualScene()->GetSceneNode(), model);
	}
}
