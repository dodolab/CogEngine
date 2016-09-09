#pragma once

#include "Behavior.h"

namespace Cog {

	/**x
	* Behavior for mapping key inputs to ACTIONS
	*/
	class InputKey : public Behavior {
	private:
		map<int, Act> keyMapping;

	public:
		/**
		* Creates a new behavior that maps keys to actions
		* @param keyMapping key-to-action map
		*/
		InputKey(map<int, Act> keyMapping) : keyMapping(keyMapping) {

		}

		void Init() {
			if (!owner->HasAttr(Attrs::ACTIONS)) {
				owner->AddAttr(Attrs::ACTIONS, Flags());
			}
		}

		virtual void Update(const uint64 delta, const uint64 absolute) {

			Flags& actions = owner->GetAttr<Flags>(Attrs::ACTIONS);

			for (auto key : GetPressedKeys()) {

				if (!key.IsHandled() || key.handlerId == owner->GetId()) {
					// handle key press
					key.handlerId = owner->GetId();

					Act inAct = GetAction(key.key);
					if (inAct != Act::NONE)
					{
						if (!key.ended) actions.SetState((int)inAct);
						else actions.ResetState((int)inAct);
					}
				}
			}
		}

	private:

		// Gets action by key
		Act GetAction(int key) {
			auto find = keyMapping.find(key);
			if (find != keyMapping.end()) return (*find).second;

			return Act::NONE;
		}
	};

}