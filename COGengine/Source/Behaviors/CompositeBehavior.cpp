#include "CompositeBehavior.h"

namespace Cog {

	void CompositeBehavior::AddBehavior(Behavior* beh) {
		behaviors.push_back(beh);
		if (this->initialized) {
			SetOwner(beh, owner);
			beh->Init();
		}
	}

	bool CompositeBehavior::RemoveBehavior(Behavior* beh) {
		for (auto it = behaviors.begin(); it != behaviors.end(); ++it) {
			if ((*it)->GetId() == beh->GetId()) {
				behaviors.erase(it);
				break;
				return true;
			}
		}
		return false;
	}

	void CompositeBehavior::OnInit() {
		for (auto beh : behaviors) {
			SetOwner(beh, owner);
			beh->Init();
		}
	}

	void CompositeBehavior::OnStart() {
		for (auto beh : behaviors) {
			beh->Start();
		}
	}

	void CompositeBehavior::OnFinish() {
		for (auto beh : behaviors) {
			if (!beh->HasFinished()) beh->Finish();
		}
	}

	void CompositeBehavior::OnMessage(Msg& msg) {
		for (auto beh : behaviors) {
			beh->OnMessage(msg);
		}
	}

	void CompositeBehavior::Update(const uint64 delta, const uint64 absolute) {
		
		if (this->behaviors.empty()) this->Finish();

		for (auto it = behaviors.begin(); it != behaviors.end();) {
			auto beh = (*it);
			if (beh->HasFinished()) {
				it = behaviors.erase(it);
				continue;
			}
			else if(beh->GetListenerState() == ListenerState::ACTIVE_UPDATES || 
				beh->GetListenerState() == ListenerState::ACTIVE_ALL) {
				beh->Update(delta, absolute);
			}
			++it;
		}
	}


} // namespace