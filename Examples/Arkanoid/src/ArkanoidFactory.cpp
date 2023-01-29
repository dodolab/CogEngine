#include "TransformBuilder.h"
#include "jsonxx.h"
#include "ArkanoidFactory.h"
#include "SpriteSheetBuilder.h"
#undef None // LuaBridge is polluting Ubuntu's global namespace
#include <LuaBridge.h>
#include "Node.h"
#include "BehaviorLua.h"
#include "List.h"
#include "ArkanoidModel.h"
#include "ArkanoidConstants.h"
#include "PaddleComponent.h"
#include "BallCollisionComponent.h"
#include "ArkanoidSoundComponent.h"
#include "GameComponent.h"
#include "ArkanoidStatusComponent.h"
#include "ArkanoidIntroComponent.h"
#include "ArkanoidLifeComponent.h"
#include "BrickCollisionComponent.h"
#include "Move.h"
#include "Renderer.h"
#include "LuaScripting.h"
#include "Stage.h"
#include "FacadeLua.h"

using namespace jsonxx;
using namespace Cog;

#define ARKANOID_LUA

void ArkanoidFactory::OnInit() {
	ifstream fin;
	fin.open(ofToDataPath("data.json").c_str());
	gameConfig.parse(fin);
	fin.close();

	// create model
	model = new ArkanoidModel();

	// load sprite atlas
	auto spritesImage = CogGet2DImage("sprites.png");
	auto renderer = GETCOMPONENT(Renderer);
	renderer->AddTileLayer(spritesImage, "spriteLayer", 5000, 1);

#ifdef ARKANOID_LUA
	auto scripts = GETCOMPONENT(LuaScripting);
	scripts->LoadScript(ofFile("Arkanoid.lua"));
	InitLuaMapping(scripts->GetLua());
#endif

	ResetGame();
}


void ArkanoidFactory::Update(const uint64 delta, const uint64 absolute) {
	if(resetPending) {
		resetPending = false;
		auto stage = GETCOMPONENT(Stage);
		auto scene = stage->GetActualScene();
		scene->Finish();
		scene->Init();
		InitializeLevel(scene->GetSceneNode(), model, gameConfig);
	}
}

void ArkanoidFactory::InitializeLevel(Node* rootObject, ArkanoidModel* model, jsonxx::Object& gameConfig) {
	LoadGameConfig(rootObject, model, gameConfig);

	auto scene = rootObject->GetScene();
	scene->SetCustomHeight(25);

	if (model->currentLevel == 0) {
		AddIntro(rootObject, model);
	}
	else {
		AddPanels(rootObject, model);
		AddBricks(rootObject, model);
		AddPaddle(rootObject, model);
		AddLives(rootObject, model);
		AddStatus(rootObject, model);

		rootObject->AddBehavior(CreateArkanoidSoundComponent());
		rootObject->AddBehavior(CreateBrickCollisionComponent());
		rootObject->AddBehavior(CreateGameComponent());
	}
}

void ArkanoidFactory::AddIntro(Node* root, ArkanoidModel* model) {
	TransformBuilder trBld;

	auto scene = root->GetScene();
	
	// add title
	auto titleSpr = Sprite(CogGetSpriteSheet("title"), 0);
	auto titleMesh = spt<SpriteMesh>(new SpriteMesh(titleSpr, "spriteLayer"));
	auto titleObj = new Node("title", titleMesh);
	root->AddChild(titleObj);
	root->SubmitChanges(false);
	trBld.RelativePosition(0.5, 0.25).Anchor(0.5f, 0.5f).BuildAndReset(titleObj);

	// add ship sprite
	auto shipSpr = Sprite(CogGetSpriteSheet("ship"), 0);
	auto shipMesh = spt<SpriteMesh>(new SpriteMesh(shipSpr, "spriteLayer"));
	auto shipObj = new Node("ship", shipMesh);
	root->AddChild(shipObj);
	root->SubmitChanges(false);
	trBld.RelativePosition(0.5f, 0.65f).Anchor(0.5f, 0.5f).BuildAndReset(shipObj);

	root->AddBehavior(CreateArkanoidSoundComponent());
	root->AddBehavior(CreateArkanoidIntroComponent());
}

void ArkanoidFactory::LoadGameConfig(Node* rootObject, ArkanoidModel* model, jsonxx::Object& gameConfig) {

	auto sprites = gameConfig.get<Array>("sprites");
	model->currentRound = 0;
	model->maxLevels = gameConfig.get<Number>("levels_total");
	model->maxLives = model->currentLives = gameConfig.get<Number>("max_lives");
	model->ballSpeed = gameConfig.get<Number>("ball_speed");
	model->ballSpeedMultiplier = gameConfig.get<Number>("ball_speed_multiplier");

	SpriteSheetBuilder bld;

	// add the main game model
	rootObject->AddAttr(ARKANOID_MODEL, model, true); // model should survive scene remove

	for (int i = 0; i < sprites.size(); i++) {
		// create sprite
		auto& spr = sprites.get<Object>(i);
		bld.LoadFromJson(spr, "sprites.png", "sprites");
		SpriteSheet* sheet = bld.BuildAndReset();
		CogStoreSpriteSheet(spt<SpriteSheet>(sheet));
	}

	if (model->currentLevel != 0) { // level 0 is intro

		// load level map
		auto level = gameConfig.get<Array>("levels_maps").get<Array>(model->currentLevel - 1);

		for (int i = 0; i < level.size(); i++) {
			auto& row = level.get<Array>(i);

			for (int j = 0; j < row.size(); j++) {
				int brickIndex = row.get<Number>(j);

				if (brickIndex != ARK_BRICK_NONE) { // 0 is for empty space
					// add a new brick
					Brick brick;
					brick.position = Vec2i(j, i);
					brick.type = brickIndex;
					model->bricks[brick.position] = brick;

					if (brickIndex != ARK_BRICK_INDESTRUCTIBLE) { // skip indestructible bricks
						model->remainingBricks++;
					}
				}
			}
		}
	}
}

void ArkanoidFactory::AddBricks(Node* rootObject, ArkanoidModel* model) {
	
	auto brickMesh = spt<MultiSpriteMesh>(new MultiSpriteMesh("spriteLayer"));
	auto bricks = new Node("bricks", brickMesh);

	for (auto& brick : model->bricks) {
		auto spriteIndex = brick.second.type - 1;

		Sprite* brickSpr = new Sprite(CogGetSpriteSheet("blocks"), spriteIndex);
		brickSpr->GetTransform().localPos.x = brick.second.position.x * 2 + 1;
		brickSpr->GetTransform().localPos.y = brick.second.position.y + 1;
		brickMesh->AddSprite(brickSpr);
		brickSpr->GetTransform().scale *= (CogGetNativeScale() / rootObject->GetScene()->GetCustomScale());
		// keep relation between Brick and its sprite, we will need it
		model->brickMap[brickSpr] = brick.second;
	}

	rootObject->AddChild(bricks);
}


void ArkanoidFactory::AddPanels(Node* rootObject, ArkanoidModel* model) {

	TransformBuilder trBld;

	auto scene = rootObject->GetScene();
	
	// left panel
	auto leftPanelSpr = Sprite(CogGetSpriteSheet("left_panel"), 0);
	auto leftPanel = spt<SpriteMesh>(new SpriteMesh(leftPanelSpr, "spriteLayer"));
	auto leftPanelObj = new Node("left_panel", leftPanel);
	rootObject->AddChild(leftPanelObj);
	rootObject->SubmitChanges(false);
	trBld.LocalPosition(0, 0).BuildAndReset(leftPanelObj);

	// right panel
	auto rightPanelSpr = Sprite(CogGetSpriteSheet("right_panel"), 0);
	auto rightPanel = spt<SpriteMesh>(new SpriteMesh(rightPanelSpr, "spriteLayer"));
	auto rightPanelObj = new Node("right_panel", rightPanel);
	rootObject->AddChild(rightPanelObj);
	rootObject->SubmitChanges(false);
	trBld.LocalPosition(23, 0).BuildAndReset(rightPanelObj);

	// top panel
	auto topPanelSpr = Sprite(CogGetSpriteSheet("top_panel"), 0);
	auto topPanel = spt<SpriteMesh>(new SpriteMesh(topPanelSpr, "spriteLayer"));
	auto topPanelObj = new Node("top_panel", topPanel);
	rootObject->AddChild(topPanelObj);
	rootObject->SubmitChanges(false);
	trBld.LocalPosition(0, 0).BuildAndReset(topPanelObj);
}

void ArkanoidFactory::AddPaddle(Node* root, ArkanoidModel* model) {

	TransformBuilder trBld;

	auto scene = root->GetScene();
	
	auto paddleSpr = Sprite(CogGetSpriteSheet("paddle"), 0);
	auto paddleMesh = spt<SpriteMesh>(new SpriteMesh(paddleSpr, "spriteLayer"));
	auto paddleObj = new Node("paddle", paddleMesh);
	root->AddChild(paddleObj);
	root->SubmitChanges(false);
	trBld.LocalPosition(10, 23).BuildAndReset(paddleObj);

	// add ball
	auto ballSpr = Sprite(CogGetSpriteSheet("ball"), 0);
	auto ballMesh = spt<SpriteMesh>(new SpriteMesh(ballSpr, "spriteLayer"));
	auto ballObj = new Node("ball", ballMesh);
	root->AddChild(ballObj);
	root->SubmitChanges(false);

	// remember, the scene is scaled to 25 units of height
	trBld.LocalPosition(10.0 + model->ballOffset, 22.4f).BuildAndReset(ballObj);

	ballObj->AddAttr(ATTR_MOVEMENT, new Dynamics());
	ballObj->AddBehavior(new Move(false));			// ball movement
	ballObj->AddBehavior(CreateBallCollisionComponent());
	paddleObj->AddBehavior(CreatePaddleComponent());
}

void ArkanoidFactory::AddLives(Node* rootObject, ArkanoidModel* model) {
	TransformBuilder trBld;
	auto scene = rootObject->GetScene();
	
	// for each life, create a small paddle icon
	for (int i = 1; i <= model->currentLives; i++) {
		auto lifeSpr = Sprite(CogGetSpriteSheet("life"), 0);
		auto lifeMesh = spt<SpriteMesh>(new SpriteMesh(lifeSpr, "spriteLayer"));

		auto lifeObj = new Node(string_format("life_%d", i));
		lifeObj->SetMesh(lifeMesh);
		rootObject->AddChild(lifeObj);
		rootObject->SubmitChanges(false);
		trBld.LocalPosition(1 + 2 * (i - 1), 24).BuildAndReset(lifeObj);
	}

	rootObject->AddBehavior(CreateArkanoidLifeComponent()); 
}

void ArkanoidFactory::AddStatus(Node* rootObject, ArkanoidModel* model) {
	TransformBuilder trBld;
	
	auto status = new Node("status");
	status->SetMesh(spt<Text>(new Text(CogGetFont("comfont.TTF", 20), "")));
	rootObject->AddChild(status);
	rootObject->SubmitChanges(false);
	trBld.LocalPosition(8, 15).BuildAndReset(status);
	status->AddBehavior(CreateArkanoidStatusComponent());
}

template<>
static ArkanoidFactory* FacadeLua::CogGetComponent() {
	return GETCOMPONENT(ArkanoidFactory);
}

void ArkanoidFactory::InitLuaMapping(luabridge::lua_State* L) {

	luabridge::getGlobalNamespace(L)
		.beginClass<Brick>("Brick")
		.addData("position", &Brick::position)
		.addData("type", &Brick::type)
		.endClass();

	luabridge::getGlobalNamespace(L)
		.beginClass<ArkanoidModel>("ArkanoidModel")
		.addData("ballReleased", &ArkanoidModel::ballReleased)
		.addData("currentLevel", &ArkanoidModel::currentLevel)
		.addData("remainingBricks", &ArkanoidModel::remainingBricks)
		.addData("currentRound", &ArkanoidModel::currentRound)
		.addData("ballSpeed", &ArkanoidModel::ballSpeed)
		.addData("ballSpeedMultiplier", &ArkanoidModel::ballSpeedMultiplier)
		.addData("paddleSpeed", &ArkanoidModel::paddleSpeed)
		.addData("ballOffset", &ArkanoidModel::ballOffset)
		.addData("currentLives", &ArkanoidModel::currentLives)
		.addData("maxLives", &ArkanoidModel::maxLives)
		.addData("maxLevels", &ArkanoidModel::maxLevels)
		.addFunction("GetBrick", &ArkanoidModel::GetBrick)
		.addFunction("RemoveBrick", &ArkanoidModel::RemoveBrick)
		.endClass();

	luabridge::getGlobalNamespace(L)
		.beginClass<Node>("Node")
		.addFunction("GetAttr_ARKANOID_MODEL", reinterpret_cast<ArkanoidModel*(Node::*)()>(&Node::GetAttrPtrStatic<ARKANOID_MODEL>))
		.addFunction("GetAttr_DYNAMICS", reinterpret_cast<Dynamics*(Node::*)()>(&Node::GetAttrPtrStatic<ATTR_MOVEMENT_STR>))
		.endClass();


	luabridge::getGlobalNamespace(L)
		.deriveClass<HitInfo, MsgPayload>("HitInfo")
		.addConstructor<void(*)(), RefCountedObjectPtr<HitInfo>>()
		.addData("hitIndex", &HitInfo::hitIndex)
		.addData("hitType", &HitInfo::hitType)
		.endClass();


	luabridge::getGlobalNamespace(L)
		.addFunction("CogGetComponent_ArkanoidFactory", &FacadeLua::CogGetComponent<ArkanoidFactory>)
		.beginClass<ArkanoidFactory>("ArkanoidFactory")
		.addFunction("ResetGame", &ArkanoidFactory::ResetGame);


	luabridge::getGlobalNamespace(L)
		.beginClass<Msg>("Msg")
		.addFunction("GetData_HITINFO", reinterpret_cast<RefCountedObjectPtr<HitInfo>(Msg::*)()>(&Msg::GetDataPtr<HitInfo>))
		.endClass();
}


// ========================================================================================

Behavior* ArkanoidFactory::CreateArkanoidIntroComponent() {
#ifdef ARKANOID_LUA
	return GETCOMPONENT(LuaScripting)->CreateLuaBehavior("ArkanoidIntroComponent");
#else
	return new ArkanoidIntroComponent();
#endif
}

Behavior* ArkanoidFactory::CreateArkanoidLifeComponent() {
#ifdef ARKANOID_LUA
	return GETCOMPONENT(LuaScripting)->CreateLuaBehavior("ArkanoidLifeComponent");
#else
	return new ArkanoidLifeComponent();
#endif
}

Behavior* ArkanoidFactory::CreateArkanoidSoundComponent() {
#ifdef ARKANOID_LUA
	return GETCOMPONENT(LuaScripting)->CreateLuaBehavior("ArkanoidSoundComponent");
#else
	return new ArkanoidSoundComponent();
#endif
}

Behavior* ArkanoidFactory::CreateArkanoidStatusComponent() {
#ifdef ARKANOID_LUA
	return GETCOMPONENT(LuaScripting)->CreateLuaBehavior("ArkanoidStatusComponent");
#else
	return new ArkanoidStatusComponent();
#endif
}

Behavior* ArkanoidFactory::CreateBallCollisionComponent() {
#ifdef ARKANOID_LUA
	return GETCOMPONENT(LuaScripting)->CreateLuaBehavior("BallCollisionComponent");
#else
	return new BallCollisionComponent();
#endif
}

Behavior* ArkanoidFactory::CreatePaddleComponent() {
#ifdef ARKANOID_LUA
	return GETCOMPONENT(LuaScripting)->CreateLuaBehavior("PaddleInputController");
#else
	return new PaddleInputController();
#endif
}

Behavior* ArkanoidFactory::CreateBrickCollisionComponent() {
#ifdef ARKANOID_LUA
	return GETCOMPONENT(LuaScripting)->CreateLuaBehavior("BrickCollisionComponent");
#else
	return new BrickCollisionComponent();
#endif
}

Behavior* ArkanoidFactory::CreateGameComponent() {
#ifdef ARKANOID_LUA
	return GETCOMPONENT(LuaScripting)->CreateLuaBehavior("GameComponent");
#else
	return new GameComponent();
#endif
}
