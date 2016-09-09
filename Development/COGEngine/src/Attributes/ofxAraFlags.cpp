#include "ofxAraFlags.h"


ofxAraFlags::ofxAraFlags(){
	Init();
}

ofxAraFlags::ofxAraFlags(vector<int> states){
	Init();
	for (size_t i = 0; i < states.size(); i++) SetState(states[i]);
}

ofxAraFlags::ofxAraFlags(int state){
	Init();
	if (state != -1) SetState(state);
}

ofxAraFlags::ofxAraFlags(int state1, int state2) {
	Init();
	if (state1 != -1) SetState(state1);
	if (state2 != -1) SetState(state2);
}

ofxAraFlags::ofxAraFlags(int state1, int state2, int state3) {
	Init();
	if (state1 != -1) SetState(state1);
	if (state2 != -1) SetState(state2);
	if (state3 != -1) SetState(state3);
}

ofxAraFlags::ofxAraFlags(int state1, int state2, int state3, int state4) {
	Init();
	if (state1 != -1) SetState(state1);
	if (state2 != -1) SetState(state2);
	if (state3 != -1) SetState(state3);
	if (state4 != -1) SetState(state4);
}

ofxAraFlags::ofxAraFlags(int state1, int state2, int state3, int state4, int state5){
	Init();
	if (state1 != -1) SetState(state1);
	if (state2 != -1) SetState(state2);
	if (state3 != -1) SetState(state3);
	if (state4 != -1) SetState(state4);
	if (state5 != -1) SetState(state5);
}

ofxAraFlags::ofxAraFlags(const ofxAraFlags& obj){
	Init();

	flags1 = obj.flags1;
	flags2 = obj.flags2;
	flags3 = obj.flags3;
	flags4 = obj.flags4;

	if (obj.otherFlags != nullptr){
		otherFlags = new map<int, int>();

		for (map<int, int>::iterator it = obj.otherFlags->begin(); it != obj.otherFlags->end(); ++it) {
			(*otherFlags)[it->first] = it->second;
		}
	}
}

ofxAraFlags::~ofxAraFlags(){
	delete otherFlags;
}

vector<int> ofxAraFlags::GetAllStates() const{
	vector<int> output;

	// iterate over integer flags
	for (int i = 0; i < sizeof(int)*4; i++){
		if (HasState(i)) output.push_back(i);
	}

	// oterate over map
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

bool ofxAraFlags::HasState(int state) const{
	int index = GetStateIndex(state);
	int offset = GetStateOffset(state);
	int binary = 1 << offset;

	// check integer flags
	if (index <= 3)
	{
		switch (index){
		case 0: return (flags1 & binary) == binary;
		case 1: return (flags2 & binary) == binary;
		case 2: return (flags3 & binary) == binary;
		case 3: return (flags4 & binary) == binary;
		}
	}

	// index > 3 - check map
	if (otherFlags != nullptr){
		return otherFlags->count(index) && (((*otherFlags)[index] & binary) == binary);
	}
	else return false;
}

void ofxAraFlags::SwitchState(int state1, int state2){
	bool hasState2 = HasState(state2);

	if (HasState(state1)) SetState(state2);
	else ResetState(state2);

	if (hasState2) SetState(state1);
	else ResetState(state1);
}

ofxAraFlags& ofxAraFlags::operator=(const int& st1){
	delete otherFlags;
	flags1 = flags2 = flags3 = flags4 = 0;
	otherFlags = nullptr;

	SetState(st1);
	return *this;
}

ofxAraFlags& ofxAraFlags::operator=(const ofxAraFlags& st1){
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

bool ofxAraFlags::operator==(int st2){
	return HasState(st2);
}

bool ofxAraFlags::operator==(const ofxAraFlags& st2){
	if (otherFlags != nullptr){
		// compare map
		if (st2.otherFlags == nullptr || otherFlags->size() != st2.otherFlags->size()) return false;

		for (map<int, int>::iterator iter = otherFlags->begin(); iter != otherFlags->end(); ++iter)
		{
			int key = iter->first;
			int value = iter->second;
			if ((*st2.otherFlags)[key] != value) return false;
		}
	}

	// compare integer flags
	return flags1 == st2.flags1 && flags2 == st2.flags2 && flags3 == st2.flags3 && flags4 == st2.flags4;
}

bool ofxAraFlags::operator!=(int st1){
	return !(*this == st1);
}

bool ofxAraFlags::operator!=(const ofxAraFlags& st1){
	return !(*this == st1);
}

ofxAraFlags ofxAraFlags::operator+(int st1){
	ofxAraFlags copy(*this);
	copy.SetState(st1);
	return copy;
}

ofxAraFlags ofxAraFlags::operator-(int st1){
	ofxAraFlags copy(*this);
	copy.ResetState(st1);
	return copy;
}

ofxAraFlags& ofxAraFlags::operator+=(int st1){
	SetState(st1);
	return *this;
}

ofxAraFlags& ofxAraFlags::operator-=(int st1){
	ResetState(st1);
	return *this;
}

bool ofxAraFlags::Contains(ofxAraFlags& other) const{
	// return first false
	vector<int> allStates = other.GetAllStates();
	for (size_t i = 0; i < allStates.size(); i++){
		if (!HasState(allStates[i])) return false;
	}
	return true;
}

bool ofxAraFlags::ContainsAtLeastOne(ofxAraFlags& other) const{
	// return first true
	vector<int> allStates = other.GetAllStates();
	for (size_t i = 0; i < allStates.size(); i++){
		if (HasState(allStates[i])) return true;
	}
	return false;
}

void ofxAraFlags::DoStateOperation(bool set, int state){
	int index = GetStateIndex(state);
	int offset = GetStateOffset(state);
	int binary = 1 << offset;

	// set integer flags
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

	// index >3 - set map
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
