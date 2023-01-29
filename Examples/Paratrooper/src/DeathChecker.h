#pragma once

#include "Component.h"
#include "Behavior.h"
#include "TransformBuilder.h"
#include "GameManager.h"
#include "DeathAnimation.h"

using namespace Cog;

/**
 * Component that watches killed units, executes flickering animation and removes
 * them from the scene
 */
class DeathChecker : public Behavior {

public:
	map<int, Behavior*> pendingAnimations;

	virtual void OnInit() {
		SubscribeForMessages(UNIT_KILLED, ANIMATION_ENDED);
	}

	virtual void OnMessage(Msg& msg) {
		if(msg.GetAction() == UNIT_KILLED) {
			// create a flickering animation
			auto node = msg.GetContextNode();
			auto anim = new DeathAnimation();
			node->AddBehavior(anim);
			pendingAnimations[anim->GetId()] = anim;
			COGLOGDEBUG("DeathChecker", "%s killed", node->GetTag().c_str());
		} else if(msg.GetAction() == ANIMATION_ENDED) {
			// flickering animation ended -> remove the unit from the scene
			auto found = pendingAnimations.find(msg.GetSenderId());
			if(found != pendingAnimations.end()) {
				msg.GetContextNode()->RemoveFromParent(true);
			}
		}
	}

	virtual void Update(uint64_t delta, uint64_t absolute) {
		
	}
};
