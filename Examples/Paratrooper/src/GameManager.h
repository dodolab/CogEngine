#pragma once

#include "Component.h"
#include "ofImage.h"
#include "Behavior.h"
#include "GameValues.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "ParatrooperModel.h"
#include "ParatrooperComponent.h"
#include "CopterComponent.h"
#include "CopterAnimator.h"

using namespace Cog;

/**
 * Game manager and collision handler
 */
class GameManager : public Behavior {
	ParatrooperModel* model;
	ParatrooperFactory* factory;
	uint64_t gameOverTime = 0;
	uint64_t lastAbsolute = 0;
public:


	virtual void OnInit() {
		SubscribeForMessages(COLLISION, PROJECTILE_SHOT, UNIT_KILLED, UNIT_LANDED);
		model = owner->GetSceneRoot()->GetAttr<ParatrooperModel*>(MODEL);
		factory = GETCOMPONENT(ParatrooperFactory);
	}

	virtual void OnMessage(Msg& msg) {
		if (model->isGameOver) {
			return;
		}

		if (msg.GetAction() == COLLISION) {
			HandleCollision(msg);
		}
		else if (msg.GetAction() == PROJECTILE_SHOT) {
			// decrease score with each shot
			model->score = max(0, model->score - model->shootPenalty);
		}
		else if (msg.GetAction() == UNIT_LANDED) {

			model->landedUnits++;

			if (model->landedUnits >= model->maxLandedUnits) {
				// GAME OVER
				GameOver(lastAbsolute);
			}
		}
	}

	virtual void Update(uint64_t delta, uint64_t absolute) {
		this->lastAbsolute = absolute;

		// just wait 5 seconds after game over and reset the game
		if (model->isGameOver && (absolute - gameOverTime) > 5000) {
			factory->ResetGame();
			Finish();
		}
	}

protected:
	void HandleCollision(Msg& msg) {
		auto trigger = msg.GetDataPtr<CollisionTrigger>();

		if (trigger->unit->GetTag() == OBJECT_COPTER) {
			// copter hit
			model->score += model->copterReward;
			trigger->unit->SetState(FLAG_DEAD);
			SendMessage(UNIT_KILLED, trigger->unit);
		}
		else if (trigger->unit->GetTag() == OBJECT_PARATROOPER) {
			// we can either kill the paratrooper or blow up his parachute

			if (trigger->unit->GetAttr<ParaState>(PARA_STATE) == ParaState::FALLING) {
				// paratrooper hit while falling
				model->score += model->paratrooperShotReward;
				trigger->unit->SetState(FLAG_DEAD);
				SendMessage(UNIT_KILLED, trigger->unit);
			}
			else {
				// paratrooper hit while landing
				auto& unitBB = trigger->unit->GetMesh()->GetBoundingBox();
				auto& projectileBB = trigger->projectile->GetMesh()->GetBoundingBox();
				auto state = trigger->unit->GetAttr<ParaState>(PARA_STATE);

				if (state == ParaState::FALLING_PARACHUTE && projectileBB.topLeft.y <= (unitBB.bottomLeft.y - unitBB.GetSize().y / 2)) {
					// remove parachute -> paratrooper is gonna be killed by gravity :-D
					trigger->unit->ChangeAttr<ParaState>(PARA_STATE, ParaState::FALLING_WIHTOUT_PARACHUTE);
				}
				else {
					// we hit the paratrooper's body
					trigger->unit->SetState(FLAG_DEAD);
					SendMessage(UNIT_KILLED, trigger->unit);
				}

				// reward is different -> we hit the paratrooper too late
				model->score += model->paratrooperFallingReward;
			}
		}

		trigger->projectile->GetParent()->RemoveChild(trigger->projectile, true);
	}

	void GameOver(uint64_t absolute) {
		ofLogNotice("GameManager", "GAME OVER");

		// change visibility of the game over text 
		auto gameOver = owner->GetScene()->FindNodeByTag(OBJECT_GAMEOVER);
		gameOver->GetMesh()->SetIsVisible(true);
		model->isGameOver = true;
		gameOverTime = absolute;

		// find all paratroopers and set their state to CAPTURING. This will execute
		// the capturing animation
		vector<Node*> allParatroopers;
		owner->GetScene()->FindNodesByTag(OBJECT_PARATROOPER, allParatroopers);

		for (auto para : allParatroopers) {
			if (para->GetAttr<ParaState>(PARA_STATE) == ParaState::ON_GROUND) {
				para->ChangeAttr(PARA_STATE, ParaState::CAPTURING);
			}
		}

		// notify everyone interested
		SendMessage(GAME_OVER);
	}
};
