
#ifndef ENSTATE_H
#define ENSTATE_H

#include <map>
using namespace std;

class EnFlags{
private:
	enum Operation { HAS, SET, RESET };
	map<int, int>* states;
	int GetStateIndex(int state);
	int GetStateOffset(int state);

public:
	EnFlags();

	EnFlags(vector<int> states);

	EnFlags(int state);

	EnFlags(int state1, int state2);

	EnFlags(int state1, int state2, int state3);

	EnFlags(int state1, int state2, int state3, int state4);

	EnFlags(int state1, int state2, int state3, int state4, int state5);

	EnFlags(const EnFlags &obj);

	~EnFlags();

	vector<int> GetAllStates();

	bool HasState(int state);

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

	bool Contains(EnFlags other);

	bool ContainsAtLeastOne(EnFlags other);

};



int EnFlags::GetStateIndex(int state){
	return 0;
}
int EnFlags::GetStateOffset(int state){
	return 0;
}


EnFlags::EnFlags(){

	}

EnFlags::EnFlags(vector<int> states){

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

EnFlags::EnFlags(const EnFlags &obj){

}

EnFlags::~EnFlags(){

}

vector<int> EnFlags::GetAllStates(){
	return vector<int>();
}

bool EnFlags::HasState(int state){
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

bool EnFlags::Contains(EnFlags other){
	return false;
}

bool EnFlags::ContainsAtLeastOne(EnFlags other){
	return false;
}

#endif
