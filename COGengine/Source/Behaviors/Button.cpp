
#include "Button.h"
#include "CogEngine.h"
#include "Node.h"

namespace Cog {

	void Button::OnInit() {
		SubscribeForMessages(ACT_OBJECT_HIT_STARTED, ACT_OBJECT_HIT_LOST, ACT_OBJECT_HIT_ENDED, ACT_STATE_CHANGED);
	}

	void Button::OnStart() {
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
				msg.GetContextNode()->GetMesh<Image>()->SetImage(pressedImg);
			}
			else if (!owner->HasState(stateDisabled) && (msg.HasAction(ACT_OBJECT_HIT_ENDED) || msg.HasAction(ACT_OBJECT_HIT_LOST))) {
				msg.GetContextNode()->GetMesh<Image>()->SetImage(defaultImg);
				if (msg.HasAction(ACT_OBJECT_HIT_ENDED)) {
					SendMessage(StrId(ACT_BUTTON_CLICKED), owner);
				}
			}
			else if (disabledImg && msg.GetContextNode()->HasState(stateDisabled) && msg.GetAction() == ACT_STATE_CHANGED) {
				msg.GetContextNode()->GetMesh<Image>()->SetImage(disabledImg);
			}
			else if (defaultImg && !msg.GetContextNode()->HasState(stateDisabled) && msg.GetAction() == ACT_STATE_CHANGED) {
				msg.GetContextNode()->GetMesh<Image>()->SetImage(defaultImg);
			}
		}
	}

}// namespace