
#ifndef ENSTATE_H
#define ENSTATE_H

#include <map>
using namespace std;


class EnFlags{
private:
	enum Operation { HAS, SET, RESET };
	int statePart1;
	int statePart2;
	int statePart3;
	int statePart4;
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

	bool DoStateOperation(Operation oper, int state);

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



int EnFlags::GetStateIndex(int state) const{
	return 0;
}
int EnFlags::GetStateOffset(int state) const{
	return 0;
}


EnFlags::EnFlags(){

	}

EnFlags::EnFlags(CIwArray<int> states){

}

EnFlags::EnFlags(int state){

}

EnFlags::EnFlags(int state1, int state2){

}

EnFlags::EnFlags(int state1, int state2, int state3){

}

EnFlags::EnFlags(int state1, int state2, int state3, int state4){

}

EnFlags::EnFlags(int state1, int state2, int state3, int state4, int state5){

}

EnFlags::EnFlags(const EnFlags& obj){

}

EnFlags::~EnFlags(){

}

CIwArray<int> EnFlags::GetAllStates() const{
	return CIwArray<int>();
}

bool EnFlags::HasState(int state) const{
	return false;
}

void EnFlags::SetState(int state){

}

void EnFlags::ResetState(int state){

}

bool EnFlags::DoStateOperation(Operation oper, int state){
	return false;
}

EnFlags& EnFlags::operator=(const EnFlags& st1){
	return EnFlags();
}

bool EnFlags::operator==(int st2){
	return false;
}

bool EnFlags::operator==(const EnFlags& st2){
	return false;
}

bool EnFlags::operator!=(int st2){
	return false;
}

bool EnFlags::operator!=(const EnFlags& st2){
	return false;
}

EnFlags& EnFlags::operator+(int st2){
	return EnFlags();
}

EnFlags& EnFlags::operator-(int st2){
	return EnFlags();
}

bool EnFlags::Contains(EnFlags& other) const{
	return false;
}

bool EnFlags::ContainsAtLeastOne(EnFlags& other) const{
	return false;
}

#endif
