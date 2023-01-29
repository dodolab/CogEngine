#pragma once

#include "StrId.h"
#include "ArkanoidConstants.h"

/**
 * Component that only plays sounds according to the incoming events
 */
class ArkanoidSoundComponent : public Behavior {
public:

	virtual void OnInit() {
		SubscribeForMessages(ARK_EVENT_GAME_STARTED);
		SubscribeForMessages(ARK_EVENT_ROUND_STARTED);
		SubscribeForMessages(ARK_EVENT_OBJECT_HIT);
		SubscribeForMessages(ARK_EVENT_GAME_OVER);
		SubscribeForMessages(ARK_EVENT_LEVEL_COMPLETED);
		SubscribeForMessages(ARK_EVENT_LEVEL_STARTED);
		SubscribeForMessages(ARK_EVENT_GAME_COMPLETED);
	}

	virtual void OnMessage(Msg& msg) {
		if(msg.GetAction() == ARK_EVENT_ROUND_STARTED) {
			CogPlaySound(CogGetSound(FILE_SOUND_ROUND));
		}else if(msg.GetAction() == ARK_EVENT_OBJECT_HIT) {
			CogPlaySound(CogGetSound(FILE_SOUND_HIT));
		}else if(msg.GetAction() == ARK_EVENT_GAME_OVER) {
			CogPlaySound(CogGetSound(FILE_SOUND_GAME_OVER));
		}else if(msg.GetAction() == ARK_EVENT_LEVEL_COMPLETED) {
			CogPlaySound(CogGetSound(FILE_SOUND_GAME_OVER));
		} else if (msg.GetAction() == ARK_EVENT_LEVEL_STARTED) {
			CogPlaySound(CogGetSound(FILE_SOUND_ROUND));
		} else if (msg.GetAction() == ARK_EVENT_GAME_STARTED) {
			CogPlaySound(CogGetSound(FILE_SOUND_GAME_START));
		} else if (msg.GetAction() == ARK_EVENT_GAME_COMPLETED) {
			CogPlaySound(CogGetSound(FILE_SOUND_GAME_OVER));
		}
	}

	virtual void Update(uint64 delta, uint64 absolute) {
		
	}
};