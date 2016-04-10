
#include "InputKey.h"
#include "CogEngine.h"

namespace Cog {


	void InputKey::OnStart() {
		if (!owner->HasAttr(ATTR_ACTIONS)) {
			owner->AddAttr(ATTR_ACTIONS, Flags());
		}
	}

	void InputKey::Update(const uint64 delta, const uint64 absolute) {

		Flags& actions = owner->GetAttr<Flags>(ATTR_ACTIONS);

		for (auto key : CogGetPressedKeys()) {

			if (!key->IsHandled()) {


				// todo...
				if (key->key == (int)('l')) {
					// handle key press
					key->handlerNodeId = owner->GetId();
					// write log
					CogWriteLogActualScene();
				}
				else if (key->key == (int)('r')) {
					key->handlerNodeId = owner->GetId();
					CogWriteTimeReport(true);
				}

				Act inAct = GetAction(key->key);
				if (inAct != Act::NONE)
				{
					if (!key->ended) actions.SetState((int)inAct);
					else actions.ResetState((int)inAct);
				}
			}
		}
	}

	Act InputKey::GetAction(int key) {
		auto found = keyMapping.find(key);
		if (found != keyMapping.end()) return (*found).second;

		return Act::NONE;
	}


}// namespace