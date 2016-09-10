
#include "Button.h"
#include "CogEngine.h"

namespace Cog {

	void Button::OnInit() {
		Button::RegisterListening(ACT_OBJECT_HIT_STARTED, ACT_OBJECT_HIT_LOST, ACT_OBJECT_HIT_ENDED, ACT_STATE_CHANGED);
	}

	void Button::OnStart() {
		if (disabledImg && owner->HasState(stateDisabled)) {
			owner->GetShape<spt<Image>>()->SetImage(disabledImg);
		}
		else {
			owner->GetShape<spt<Image>>()->SetImage(defaultImg);
		}
	}

	void Button::OnMessage(Msg& msg) {
		if (msg.GetSourceObject()->GetId() == owner->GetId()) {
			if (!owner->HasState(stateDisabled) && msg.HasAction(ACT_OBJECT_HIT_STARTED)) {
				msg.GetSourceObject()->GetShape<spt<Image>>()->SetImage(pressedImg);
			}
			else if (!owner->HasState(stateDisabled) && (msg.HasAction(ACT_OBJECT_HIT_ENDED) || msg.HasAction(ACT_OBJECT_HIT_LOST))) {
				msg.GetSourceObject()->GetShape<spt<Image>>()->SetImage(defaultImg);
				if (msg.HasAction(ACT_OBJECT_HIT_ENDED)) {
					SendMessageToListeners(StringHash(ACT_BUTTON_CLICKED), 0, nullptr, owner);
				}
			}
			else if (disabledImg && msg.GetSourceObject()->HasState(stateDisabled) && msg.GetAction() == ACT_STATE_CHANGED) {
				msg.GetSourceObject()->GetShape<spt<Image>>()->SetImage(disabledImg);
			}
			else if (defaultImg && !msg.GetSourceObject()->HasState(stateDisabled) && msg.GetAction() == ACT_STATE_CHANGED) {
				msg.GetSourceObject()->GetShape<spt<Image>>()->SetImage(defaultImg);
			}
		}
	}

}// namespace