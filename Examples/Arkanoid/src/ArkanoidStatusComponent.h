#pragma once

#include "StrId.h"
#include "Component.h"
#include "SteeringMath.h"
#include "ArkanoidConstants.h"
#include "Scene.h"
#include "ArkanoidModel.h"

using namespace Cog;

/**
 * Component that shows game status such as GAME OVER, ROUND 2 etc.
 */
class ArkanoidStatusComponent : public Behavior {
private:
	// show all texts for 3 seconds
	int statusShowDelay = 3000;
	int statusShowTime = 0;
	bool isShowing = 0;
	ArkanoidModel* model;
public:

	virtual void OnInit() {
		model = owner->GetSceneRoot()->GetAttr<ArkanoidModel*>(ARKANOID_MODEL);
		SubscribeForMessages(ARK_EVENT_LEVEL_STARTED);
		SubscribeForMessages(ARK_EVENT_ROUND_STARTED);
		SubscribeForMessages(ARK_EVENT_GAME_OVER);
		SubscribeForMessages(ARK_EVENT_LEVEL_COMPLETED);
		SubscribeForMessages(ARK_EVENT_GAME_COMPLETED);

	}

	virtual void OnMessage(Msg& msg) {
		if (msg.GetAction() == ARK_EVENT_LEVEL_STARTED) {
			ShowText(string_format("LEVEL %d", model->currentLevel));
		}
		else if (msg.GetAction() == ARK_EVENT_ROUND_STARTED) {
			ShowText(string_format("ROUND %d", model->currentRound));
		}
		else if (msg.GetAction() == ARK_EVENT_GAME_OVER) {
			ShowText(string_format("GAME OVER"));
		}
		else if (msg.GetAction() == ARK_EVENT_LEVEL_COMPLETED) {
			ShowText(string_format("LEVEL COMPLETED"));
		}
		else if (msg.GetAction() == ARK_EVENT_GAME_COMPLETED) {
			ShowText(string_format("!!!YOU WIN THE GAME!!!"));
		}
	}

	void ShowText(string text) {
		auto mesh = owner->GetMesh<Text>();
		mesh->SetIsVisible(true);
		mesh->SetText(text);
		isShowing = true;
		statusShowTime = 0;
	}

	virtual void Update(uint64 delta, uint64 absolute) {
		if (isShowing) {
			statusShowTime += delta;

			if (statusShowTime > statusShowDelay) {
				statusShowTime = 0;
				owner->GetMesh<Text>()->SetIsVisible(false);
				isShowing = false;
			}
		}
	}
};