#include "EnFlags.h"
#include <vector>



void EnFlags::DoStateOperation(bool set, int state){
	int index = GetStateIndex(state);
	int offset = GetStateOffset(state);
	int binary = 1 << offset;

	if (index <= 3)
	{
		switch (index){
		case 0:  if (set)  (flags1 |= binary); else (flags1 ^= binary);
			return;
		case 1:  if (set) (flags2 |= binary); else (flags2 ^= binary);
			return;
		case 2:  if (set) (flags3 |= binary); else (flags3 ^= binary);
			return;
		case 3:  if (set) (flags4 |= binary); else (flags4 ^= binary);
			return;
		}
	}

	// index >3
	if (otherFlags == nullptr) otherFlags = new map<int, int>();

	if (set){
		if (!otherFlags->count(index)){
			(*otherFlags)[index] = binary;
		}
		else (*otherFlags)[index] |= binary;
	}
	else{
		if (otherFlags->count(index)){
			(*otherFlags)[index] ^= binary;
			// must be, because there can't be any null value
			if (((*otherFlags)[index]) == 0) otherFlags->erase(index);
		}
	}
}

EnFlags::EnFlags(){
	otherFlags = nullptr;
}

EnFlags::EnFlags(vector<int> states){
	otherFlags = nullptr;
	for (int i = 0; i < states.size(); i++) SetState(states[i]);
}

EnFlags::EnFlags(int state) : EnFlags(state, -1, -1, -1, -1){

}

EnFlags::EnFlags(int state1, int state2) : EnFlags(state1, state2, -1, -1, -1){

}

EnFlags::EnFlags(int state1, int state2, int state3) : EnFlags(state1, state2, state3, -1, -1){

}

EnFlags::EnFlags(int state1, int state2, int state3, int state4) : EnFlags(state1, state2, state3, state4, -1){

}

EnFlags::EnFlags(int state1, int state2, int state3, int state4, int state5){
	otherFlags = nullptr;
	if (state1 != -1) SetState(state1);
	if (state2 != -1) SetState(state2);
	if (state3 != -1) SetState(state3);
	if (state4 != -1) SetState(state4);
	if (state5 != -1) SetState(state5);
}

EnFlags::EnFlags(const EnFlags& obj){
	flags1 = obj.flags1;
	flags2 = obj.flags2;
	flags3 = obj.flags3;
	flags4 = obj.flags4;

	otherFlags = new map<int, int>();

	for (map<int, int>::iterator it = obj.otherFlags->begin(); it != obj.otherFlags->end(); ++it) {
		(*otherFlags)[it->first] = it->second;
	}
}

EnFlags::~EnFlags(){
	delete otherFlags;
}

vector<int> EnFlags::GetAllStates() const{
	vector<int> output;

	for (int i = 0; i < sizeof(int)*4; i++){
		if (HasState(i)) output.push_back(i);
	}

	if (otherFlags != nullptr){
		for (map<int, int>::iterator it = otherFlags->begin(); it != otherFlags->end(); ++it) {
			// get value
			int flag = it->second;
			if (flag != 0){
				for (int i = 0; i < sizeof(int); i++){
					// state = key*intsize + i
					if (HasState(it->first*sizeof(int)+i)) output.push_back(i);
				}
			}
		}
	}

	return output;
}

bool EnFlags::HasState(int state) const{
	int index = GetStateIndex(state);
	int offset = GetStateOffset(state);
	int binary = 1 << offset;

	if (index <= 3)
	{
		switch (index){
		case 0: return (flags1 & binary) == binary;
		case 1: return (flags2 & binary) == binary;
		case 2: return (flags3 & binary) == binary;
		case 3: return (flags4 & binary) == binary;
		}
	}

	// index > 3
	if (otherFlags != nullptr){
		return otherFlags->count(index) && (((*otherFlags)[index] & binary) == binary);
	}
	else return false;
}

EnFlags& EnFlags::operator=(const int& st1){
	delete otherFlags;
	flags1 = flags2 = flags3 = flags4 = 0;
	otherFlags = nullptr;

	SetState(st1);
	return *this;
}

EnFlags& EnFlags::operator=(const EnFlags& st1){
	flags1 = st1.flags1;
	flags2 = st1.flags2;
	flags3 = st1.flags3;
	flags4 = st1.flags4;

	delete otherFlags;

	if (st1.otherFlags != nullptr){
		(*otherFlags) = (*st1.otherFlags);
	}
	return *this;
}

bool EnFlags::operator==(int st2){
	return HasState(st2);
}

bool EnFlags::operator==(const EnFlags& st2){
	if (otherFlags != nullptr){
		if (st2.otherFlags == nullptr || otherFlags->size() != st2.otherFlags->size()) return false;

		for (map<int, int>::iterator iter = otherFlags->begin(); iter != otherFlags->end(); ++iter)
		{
			int key = iter->first;
			int value = iter->second;
			if ((*st2.otherFlags)[key] != value) return false;
		}
	}

	return flags1 == st2.flags1 && flags2 == st2.flags2 && flags3 == st2.flags3 && flags4 == st2.flags4;
}

bool EnFlags::operator!=(int st1){
	return !(*this == st1);
}

bool EnFlags::operator!=(const EnFlags& st1){
	return !(*this == st1);
}

EnFlags EnFlags::operator+(int st1){
	EnFlags copy(*this);
	copy.SetState(st1);
	return copy;
}

EnFlags EnFlags::operator-(int st1){
	EnFlags copy(*this);
	copy.ResetState(st1);
	return copy;
}

EnFlags& EnFlags::operator+=(int st1){
	SetState(st1);
	return *this;
}

EnFlags& EnFlags::operator-=(int st1){
	ResetState(st1);
	return *this;
}

bool EnFlags::Contains(EnFlags& other) const{
	vector<int> allStates = other.GetAllStates();
	for (int i = 0; i < allStates.size(); i++){
		if (!HasState(allStates[i])) return false;
	}
	return true;
}

bool EnFlags::ContainsAtLeastOne(EnFlags& other) const{
	vector<int> allStates = other.GetAllStates();
	for (int i = 0; i < allStates.size(); i++){
		if (HasState(allStates[i])) return true;
	}
	return false;
}
