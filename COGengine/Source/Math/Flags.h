#pragma once

#include <map>
#include <vector>

using std::map;
using std::vector;

namespace Cog {

	class StringHash;

	/**
	* State machine class that holds collection of states
	*
	*/
	class Flags {
	private:
		// first flag bit array
		unsigned flags1 = 0;
		// second flag bit array
		unsigned flags2 = 0;
		// third flag bit array
		unsigned flags3 = 0;
		// fourth flag bit array
		unsigned flags4 = 0;
		// map for other flags (if used)
		map<unsigned, unsigned>* otherFlags = nullptr;


	public:
		/**
		* Creates a new state machine
		*/
		Flags();

		/**
		* Creates a new state machine, initialized with list of state
		*/
		Flags(vector<unsigned> states);

		/**
		* Creates a new state machine, initialized with any number of states
		*/
		Flags(unsigned states, ...);

		/**
		* Creates a new state machine, initialized with one state
		*/
		Flags(StringHash state);


		Flags(const Flags& obj);

		~Flags();

		/**
		* Gets list of all states (flags)
		*/
		vector<unsigned> GetAllStates() const;

		/**
		* Returns true, if a state (flag) is set
		*/
		bool HasState(unsigned state) const;

		/**
		* Sets the state
		* @param state state to set
		*/
		void SetState(unsigned state) {
			return DoStateOperation(true, state);
		}

		/**
		* Switches values of two states
		*/
		void SwitchState(unsigned state1, unsigned state2);

		/**
		* Resets the state
		* @param state state to reset
		*/
		void ResetState(unsigned state) {
			return DoStateOperation(false, state);
		}

		/**
		* Resets all states and sets one given state
		* @param st1 state to set
		*/
		Flags& operator=(const unsigned& st1);

		/**
		* Resets all states and sets all states from given EnFlag object
		* @param st1 EnFlag object to set
		*/
		Flags& operator=(const Flags& st1);

		/**
		* Compares state machine with one state
		*/
		bool operator==(unsigned st1);

		/**
		* Compares state machine with another
		*/
		bool operator==(const Flags& st1);

		/**
		* Compares state machine with one state
		*/
		bool operator!=(unsigned st1);

		/**
		* Compares state machine with another
		*/
		bool operator!=(const Flags& st1);

		/**
		* Adds a state to the state machine and returns a new object
		* @param st1 state to set
		*/
		Flags operator+(unsigned st1);

		/**
		* Removes a state from the state machine and returns a new object
		* @param st1 state to reset
		*/
		Flags operator-(unsigned st2);

		/**
		* Adds a state to the state machine
		* @param st1 state to set
		*/
		Flags& operator+=(unsigned st1);

		/**
		* Renoves a state from the state machine
		* @param st1 state to reset
		*/
		Flags& operator-=(unsigned st1);

		/**
		* Returns true, if all states from the given state machine are set in this state machine
		*/
		bool Contains(Flags& other) const;

		/**
		* Returns true, if at least one state from the given state machine is set in this state machine
		*/
		bool ContainsAtLeastOne(Flags& other) const;

	private:
		/**
		* Gets index of a state
		* @param state number of state
		*/
		int GetStateIndex(unsigned state) const {
			return state / sizeof(unsigned);
		}

		/**
		* Gets offset of a state
		* @param state number of state
		*/
		int GetStateOffset(unsigned state) const {
			return state%sizeof(unsigned);
		}

		/**
		* Does state operation (sets or resets the state)
		* @param set - if true, state will be set; reset otherwise
		* @param state - state to set/reset
		*/
		void DoStateOperation(bool set, unsigned state);
	};

}// namespace