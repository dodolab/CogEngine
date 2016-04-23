
#include "Selection.h"
#include "ofxCogEngine.h"
#include "Node.h"

namespace Cog {


	void Selection::OnInit() {
		SubscribeForMessages(ACT_STATE_CHANGED);

	}

	void Selection::OnStart() {
		if (owner->HasState(selectedState)) {
			owner->GetMesh<Image>()->SetImage(selectedImg);
		}
		else {
			owner->GetMesh<Image>()->SetImage(defaultImg);
		}
	}


	void Selection::OnMessage(Msg& msg) {
	}


	void Selection::Update(const uint64 delta, const uint64 absolute) {
		// check if the object has selected state; if so, update mesh
		if (!hasSelectedState && owner->HasState(selectedState)) {
			owner->GetMesh<Image>()->SetImage(selectedImg);
			hasSelectedState = true;
		}
		else if (hasSelectedState && !owner->HasState(selectedState)) {
			owner->GetMesh<Image>()->SetImage(defaultImg);
			hasSelectedState = false;
		}
	}

}// namespace