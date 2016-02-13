#pragma once

#include "Behavior.h"
#include "Node.h"

namespace Cog {

	/*! User input action enumerator */
	enum class Act {
		NONE = 0,		/*!< none */
		LEFT = 1,		/*!< move left */
		RIGHT = 2,	/*!< move right */
		UP = 3,		/*!< move up */
		DOWN = 4,		/*!< move down */	/*! User input action enumerator */
		FIRE = 5,		/*!< fire/action */
		SWITCH = 6	/*!< switch/action */
	};


	/**x
	* Behavior for mapping key inputs to ACTIONS
	*/
	class InputKey : public Behavior {
		OBJECT_PROTOTYPE(InputKey)
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
			if (!owner->HasAttr(ATTR_ACTIONS)) {
				owner->AddAttr(ATTR_ACTIONS, Flags());
			}
		}

		virtual void Update(const uint64 delta, const uint64 absolute) {

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

	private:

		// Gets action by key
		Act GetAction(int key) {
			auto find = keyMapping.find(key);
			if (find != keyMapping.end()) return (*find).second;

			return Act::NONE;
		}
	};

}// namespace