
#include "Selection.h"
#include "CogEngine.h"

namespace Cog {


	void Selection::OnInit() {
		RegisterListening(ACT_STATE_CHANGED);

	}

	void Selection::OnStart() {
		if (owner->HasState(selectedState)) {
			owner->GetShape<spt<Image>>()->SetImage(selectedImg);
		}
		else {
			owner->GetShape<spt<Image>>()->SetImage(defaultImg);
		}
	}


	void Selection::OnMessage(Msg& msg) {
		/*			if (msg.GetAction() == ACT_STATE_CHANGED && msg.GetSourceObject()->GetId() == owner->GetId()) {
						if (owner->HasState(selectedState)) {
							owner->GetShape<spt<Image>>()->SetImage(selectedImg);
						}
						else {
							owner->GetShape<spt<Image>>()->SetImage(defaultImg);
						}
					} moved to the update loop */
	}


	void Selection::Update(const uint64 delta, const uint64 absolute) {
		if (!hasSelectedState && owner->HasState(selectedState)) {
			owner->GetShape<spt<Image>>()->SetImage(selectedImg);
			hasSelectedState = true;
		}
		else if (hasSelectedState && !owner->HasState(selectedState)) {
			owner->GetShape<spt<Image>>()->SetImage(defaultImg);
			hasSelectedState = false;
		}
	}

}// namespace