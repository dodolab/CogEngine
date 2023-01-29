#pragma once

#include "Component.h"

using namespace Cog;

// forward decl
class Cog::Node;
class ParatrooperModel;

/**
 * Game object factory
 */
class ParatrooperFactory : public Component {
public:
	bool resetGamePending = false;
	ParatrooperModel* LoadGameModel();

	void InitializeGame(Cog::Node* rootObject, ParatrooperModel* model);

	void CreateProjectile(Cog::Node* canon, ParatrooperModel* model);

	void CreateParatrooper(Cog::Node* owner, ParatrooperModel* model);

	void CreateCopter(Cog::Node* owner, ParatrooperModel* model);

	void ResetGame() {
		resetGamePending = true;
	}

	virtual void Update(const uint64 delta, const uint64 absolute);
};
