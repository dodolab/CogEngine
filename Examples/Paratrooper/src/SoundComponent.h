#pragma once

#include "Component.h"
#include "Behavior.h"
#include "TransformBuilder.h"
#include "ParatrooperModel.h"
#include "GameValues.h"

using namespace Cog;

/**
 * Component that plays sounds based on incoming events
 */
class SoundComponent : public Behavior {
public:

	virtual void OnInit() {
		SubscribeForMessages(PROJECTILE_SHOT, GAME_OVER, UNIT_KILLED);
	}

	virtual void OnMessage(Msg& msg) {
		// can't be simpler
		if (msg.GetAction() == PROJECTILE_SHOT) {
			CogPlaySound(CogGetSound(FILE_SOUND_FIRE));
		}
		else if (msg.GetAction() == GAME_OVER) {
			CogPlaySound(CogGetSound(FILE_SOUND_GAMEOVER));
		}
		else if (msg.GetAction() == UNIT_KILLED) {
			CogPlaySound(CogGetSound(FILE_SOUND_KILL));
		}
	}

	virtual void Update(uint64_t delta, uint64_t absolute) {

	}
};