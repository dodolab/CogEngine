#pragma once

#include "Behavior.h"

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

		void OnStart();

		virtual void Update(const uint64 delta, const uint64 absolute);

	private:

		// Gets action by key
		Act GetAction(int key);
	};

}// namespace