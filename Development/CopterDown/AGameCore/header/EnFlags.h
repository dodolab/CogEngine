#ifndef ENSTATE_H
#define ENSTATE_H

#include <map>
#include "IwArray.h"
#include <vector>
using namespace std;

/**
* EnFlags - state machine class that can combined
* any number of states
*
*/
class EnFlags{
private:
	// first flag bit array
	int flags1 = 0;
	// second flag bit array
	int flags2 = 0;
	// third flag bit array
	int flags3 = 0;
	// fourth flag bit array
	int flags4 = 0;
	// map for other flags (if used)
	map<int, int>* otherFlags = nullptr;

	/**
	* Gets index of a state
	* @param state number of state
	*/
	int GetStateIndex(int state) const{
		return state / sizeof(int);
	}
	
	/**
	* Gets offset of a state
	* @param state number of state
	*/
	int GetStateOffset(int state) const{
		return state%sizeof(int);
	}

	/**
	* Does state operation (sets or resets the state)
	* @param set - if true, state will be set; reset otherwise
	* @param state - state to set/reset
	*/
	void DoStateOperation(bool set, int state);


public:
	/**
	* Creates a new flag object
	*/
	EnFlags();

	/**
	* Creates a new flag object, initialized with list of state
	*/
	EnFlags(vector<int> states);

	/**
	* Creates a new flag object, initialized with one state
	*/
	EnFlags(int state);

	/**
	* Creates a new flag object, initialized with two states
	*/
	EnFlags(int state1, int state2);

	/**
	* Creates a new flag object, initialized with three states
	*/
	EnFlags(int state1, int state2, int state3);

	/**
	* Creates a new flag object, initialized with four states
	*/
	EnFlags(int state1, int state2, int state3, int state4);

	/**
	* Creates a new flag object, initialized with five states
	*/
	EnFlags(int state1, int state2, int state3, int state4, int state5);

	EnFlags(const EnFlags& obj);

	~EnFlags();

	/**
	* Gets list of all states (flags)
	*/
	vector<int> GetAllStates() const;

	/**
	* Returns true, if a state (flag) is set
	*/
	bool HasState(int state) const;

	/**
	* Sets the state
	*/
	void SetState(int state){
		return DoStateOperation(true, state);
	}

	/**
	* Switches values of two states
	*/
	void SwitchState(int state1, int state2);

	/**
	* Resets the state
	*/
	void ResetState(int state){
		return DoStateOperation(false, state);
	}

	/**
	* Resets all states and sets one given state
	* @param st1 state to set
	*/
	EnFlags& operator=(const int& st1);

	/**
	* Resets all states and sets all states from given EnFlag object
	* @param st1 EnFlag object to set 
	*/
	EnFlags& operator=(const EnFlags& st1);

	/**
	* Compares state machine with one state 
	*/
	bool operator==(int st1);

	bool operator==(const EnFlags& st1);

	bool operator!=(int st1);

	bool operator!=(const EnFlags& st1);

	/**
	* Adds a state to the state machine and returns a new object
	* @param st1 state to set
	*/
	EnFlags operator+(int st1);

	/**
	* Removes a state from the state machine and returns a new object
	* @param st1 state to reset
	*/
	EnFlags operator-(int st2);

	/**
	* Adds a state to the state machine
	* @param st1 state to set
	*/
	EnFlags& operator+=(int st2);

	/**
	* Renoves a state from the state machine
	* @param st1 state to reset
	*/
	EnFlags& operator-=(int st2);

	/**
	* Returns true, if all states from the given object are set in this object
	*/
	bool Contains(EnFlags& other) const;

	/**
	* Returns true, if at least one state from the given object is set in this object
	*/
	bool ContainsAtLeastOne(EnFlags& other) const;

};



#endif
