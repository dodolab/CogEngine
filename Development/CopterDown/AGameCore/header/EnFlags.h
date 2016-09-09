
#ifndef ENSTATE_H
#define ENSTATE_H

#include <map>
#include "IwArray.h"
using namespace std;


class EnFlags{
private:
	int flags1;
	int flags2;
	int flags3;
	int flags4;
	map<int, int>* otherFlags;
	int GetStateIndex(int state) const;
	int GetStateOffset(int state) const;
	

public:
	EnFlags();

	EnFlags(CIwArray<int> states);

	EnFlags(int state);

	EnFlags(int state1, int state2);

	EnFlags(int state1, int state2, int state3);

	EnFlags(int state1, int state2, int state3, int state4);

	EnFlags(int state1, int state2, int state3, int state4, int state5);

	EnFlags(const EnFlags& obj);

	~EnFlags();

	CIwArray<int> GetAllStates() const;

	bool HasState(int state) const;

	void SetState(int state);

	void ResetState(int state);

	void DoStateOperation(bool set, int state);

	EnFlags& operator=(const int& st1);

	EnFlags& operator=(const EnFlags& st1);

	bool operator==(int st2);

	bool operator==(const EnFlags& st2);

	bool operator!=(int st2);

	bool operator!=(const EnFlags& st2);

	EnFlags& operator+(int st2);

	EnFlags& operator-(int st2);

	bool Contains(EnFlags& other) const;

	bool ContainsAtLeastOne(EnFlags& other) const;

};



#endif
