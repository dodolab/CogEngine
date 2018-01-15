
#include "Button.h"
#include "ofxCogEngine.h"
#include "Node.h"

namespace Cog {

	void Button::OnInit() {
		SubscribeForMessages(ACT_OBJECT_HIT_STARTED, ACT_OBJECT_HIT_LOST, ACT_OBJECT_HIT_ENDED, ACT_STATE_CHANGED);
	}

	void Button::OnStart() {
		// set default image according to object's state
		if (disabledImg && owner->HasState(stateDisabled)) {
			owner->GetMesh<Image>()->SetImage(disabledImg);
		}
		else {
			owner->GetMesh<Image>()->SetImage(defaultImg);
		}
	}

	void Button::OnMessage(Msg& msg) {
		if (msg.GetContextNode()->GetId() == owner->GetId()) {
			if (!owner->HasState(stateDisabled) && msg.HasAction(ACT_OBJECT_HIT_STARTED)) {
				// button has been pressed
				msg.GetContextNode()->GetMesh<Image>()->SetImage(pressedImg);
			}
			else if (!owner->HasState(stateDisabled) && (msg.HasAction(ACT_OBJECT_HIT_ENDED) || msg.HasAction(ACT_OBJECT_HIT_LOST))) {
				// button has been released
				msg.GetContextNode()->GetMesh<Image>()->SetImage(defaultImg);
				if (msg.HasAction(ACT_OBJECT_HIT_ENDED)) {
					SendMessage(StrId(ACT_BUTTON_CLICKED), owner);
				}
			}
			else if (disabledImg && msg.GetContextNode()->HasState(stateDisabled) && msg.GetAction() == ACT_STATE_CHANGED) {
				// button has been disabled
				msg.GetContextNode()->GetMesh<Image>()->SetImage(disabledImg);
			}
			else if (defaultImg && !msg.GetContextNode()->HasState(stateDisabled) && msg.GetAction() == ACT_STATE_CHANGED) {
				// button has been enabled
				msg.GetContextNode()->GetMesh<Image>()->SetImage(defaultImg);
			}
		}
	}

}// namespace