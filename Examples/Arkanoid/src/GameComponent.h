#pragma once

#include "StrId.h"
#include "Component.h"
#include "SteeringMath.h"
#include "Scene.h"
#include "ArkanoidFactory.h"
#include "ComponentStorage.h"

/**
 * Global game component that orchestrates the whole game
 */
class GameComponent : public Behavior {
public:
	ArkanoidModel* model;

	// attributes for delayed game reset
	bool resetPending = false;
	uint64_t resetTime = 0;
	int resetDuration = 3000;
	ArkanoidFactory* factory;

	Node* paddle;
	Node* ball;

	virtual void OnInit() {
		SubscribeForMessages(ARK_EVENT_BALL_OUTSIDE_AREA);
		SubscribeForMessages(ARK_COMMAND_FINISH_LEVEL);

		factory = GETCOMPONENT(ArkanoidFactory);

		model = owner->GetSceneRoot()->GetAttr<ArkanoidModel*>(ARKANOID_MODEL);
		ball = owner->GetScene()->FindNodeByTag("ball");
		paddle = owner->GetScene()->FindNodeByTag("paddle");

		if (model->currentRound == 0) {
			// init the first round
			ResolveNextRound();
		}
	}

	virtual void OnMessage(Msg& msg) {
		if (msg.GetAction() == ARK_EVENT_BALL_OUTSIDE_AREA) {
			ResolveLifeLost();
		}
		else if (msg.GetAction() == ARK_COMMAND_FINISH_LEVEL) {
			ResolveLevelCompleted();
		}
	}


	void ResolveLevelCompleted() {
		// go to the next level
		resetPending = true;
		if (model->currentLevel == model->maxLevels) {
			model->currentLevel = 0; // back to intro scene
			SendMessage(ARK_EVENT_GAME_COMPLETED);
		}
		else {
			model->currentLevel++;
			// slightly increase ball speed
			model->ballSpeed *= model->ballSpeedMultiplier;
			SendMessage(ARK_EVENT_LEVEL_COMPLETED);
		}
		ball->RemoveFromParent(true);
	}

	void ResolveLifeLost() {
		model->currentLives--;
		SendMessage(ARK_EVENT_LIFE_LOST);

		if (model->currentLives == 0) {
			resetPending = true;
			// game over -> go back to intro scene
			model->currentLevel = 0;
			SendMessage(ARK_EVENT_GAME_OVER);
			ball->RemoveFromParent(true);
		}
		else {
			ResolveNextRound();
		}
	}

	void ResolveNextRound() {
		auto dynamics = ball->GetAttr<Dynamics*>(ATTR_MOVEMENT);
		model->currentRound++;
		dynamics->SetVelocity(ofVec2f(0, 0));
		model->ballReleased = false;
		
		// set the position of the ball to touch the paddle
		TransformBuilder trbld;
		trbld.LocalPosition(ofVec2f(paddle->GetTransform().localPos.x + model->ballOffset, 22.4f)).BuildAndReset(ball);

		if (model->currentRound == 1) {
			// first round -> LEVEL started
			SendMessage(ARK_EVENT_LEVEL_STARTED);
		}
		else {
			SendMessage(ARK_EVENT_ROUND_STARTED);
		}
	}

	virtual void Update(uint64 delta, uint64 absolute) {

		// reset game with delay (this could be done much more efficiently in Lua!) 
		if (resetPending) {
			if (resetTime >= resetDuration) {
				resetTime = 0;
				factory->ResetGame();
			}
			else {
				resetTime += delta;
			}
		}
	}
};