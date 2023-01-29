#pragma once
#include "Component.h"

namespace luabridge {
	struct lua_State;
}


// forward decl
class ArkanoidModel;

namespace Cog {
	class Behavior;
	class Node;
}

class ArkanoidIntroComponent;
class ArkanoidLifeComponent;
class ArkanoidSoundComponent;
class BallCollisionComponent;
class BrickCollisionComponent;
class GameComponent;
class PaddleComponent;

class ArkanoidFactory : public Cog::Component {
private:
	ArkanoidModel* model;
	jsonxx::Object gameConfig;
	bool resetPending = false;
public:
	ArkanoidFactory() {
		this->initPriority = Cog::InitPriority::LOW; // must be initialized after all other components
	}

	virtual void OnInit();

	/**
	 * Initializes level that is set in the ArkanoidModel
	 */
	void InitializeLevel(Cog::Node* rootObject, ArkanoidModel* model, jsonxx::Object& gameConfig);

	void ResetGame() {
		this->resetPending = true;
	}

	virtual void Update(const uint64 delta, const uint64 absolute);

protected:

	void LoadGameConfig(Cog::Node* rootObject, ArkanoidModel* model, jsonxx::Object& gameConfig);

	void AddIntro(Cog::Node* root, ArkanoidModel* model);

	void AddBricks(Cog::Node* rootObject, ArkanoidModel* model);

	void AddPanels(Cog::Node* root, ArkanoidModel* model);

	void AddPaddle(Cog::Node* root, ArkanoidModel* model);

	void AddLives(Cog::Node* root, ArkanoidModel* model);

	void AddStatus(Cog::Node* root, ArkanoidModel* model);

public:
	void InitLuaMapping(luabridge::lua_State* L);

	
private:
	Cog::Behavior* CreateArkanoidIntroComponent();
	Cog::Behavior* CreateArkanoidLifeComponent();
	Cog::Behavior* CreateArkanoidSoundComponent();
	Cog::Behavior* CreateArkanoidStatusComponent();
	Cog::Behavior* CreateBallCollisionComponent();
	Cog::Behavior* CreatePaddleComponent();
	Cog::Behavior* CreateBrickCollisionComponent();
	Cog::Behavior* CreateGameComponent();
};
