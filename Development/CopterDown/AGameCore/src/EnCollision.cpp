#include "EnCollision.h"

EnCollision::EnCollision(int firstId, int secondId) : _firstId(firstId), _secondId(secondId){

}


int EnCollision::GetFirstId() const{
	return _firstId;
}

void EnCollision::SetFirstId(int val){
	_firstId = val;
}

int EnCollision::GetSecondId() const{
	return _secondId;
}

void EnCollision::SetSecondId(int val){
	_secondId = val;
}
