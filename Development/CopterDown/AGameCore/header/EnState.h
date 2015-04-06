
#ifndef ENSTATE_H
#define ENSTATE_H

#include <map>
using namespace std;

class State{
private:
	enum Operation { HAS, SET, RESET };
	map<int, int> states;
	int GetStateIndex(int state);
	int GetStateOffset(int state);

public:
	State(){

	}

	State(vector<int> states);

	State(int state);

	State(int state1, int state2);

	State(int state1, int state2, int state3);

	State(int state1, int state2, int state3, int state4);

	State(int state1, int state2, int state3, int state4, int state5);

	State(const State &obj);

	~State();

	vector<int> GetAllStates();

	bool HasState(int state);

	void SetState(int state);

	void ResetState(int state);

	bool DoStateOperation(Operation oper, int state);

	State& operator=(const State& st1);

	bool operator==(int st2);

	bool operator==(const State& st2);

	bool operator!=(int st2);

	bool operator!=(const State& st2);

	State& operator+(int st2);

	State& operator-(int st2);

	bool Contains(State other);

	bool ContainsAtLeastOne(State other);

};

#endif
